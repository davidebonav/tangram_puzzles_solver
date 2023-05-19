:- load_foreign_files(['../../part2/predicates_yap'], [], init_predicates).
:- consult(utils).

% Method renaming
% DIFFERENCE
difference(ConnName, Geometry1, Geometry2, DifferenceGeometry) :-
    print_log('START - difference...', 3),
    print_log(('Input - Geometry1: ', Geometry1), 3),
    print_log(('Input - Geometry2: ', Geometry2), 3),
    st_difference(ConnName, Geometry1, Geometry2, DifferenceGeometry),
    print_log(('Output - DifferenceGeometry: ', DifferenceGeometry), 3),
    print_log('END - difference...', 3).
% ROTATION
rotation(ConnHandler, Geometry, IAngleRotation, RotatedGeometry) :-
    print_log('START - rotation...', 3),
    print_log(('Input - Geometry: ', Geometry), 3),
    print_log(('Input - IAngleRotation: ', IAngleRotation), 3),
    integer_to_float(IAngleRotation, FAngleRotation),
    st_rotate(ConnHandler, Geometry, FAngleRotation, RotatedGeometry),
    print_log(('Output - RotatedGeometry: ', RotatedGeometry), 3),
    print_log('END - rotation...', 3).
rotation(ConnHandler, Geometry, IAngleRotation, (RotCenterX, RotCenterY), RotatedGeometry) :-
    print_log('START - rotation...', 3),
    print_log(('Input - Geometry: ', Geometry), 3),
    print_log(('Input - IAngleRotation: ', IAngleRotation), 3),
    integer_to_float(IAngleRotation, FAngleRotation),
    st_rotate(ConnHandler, Geometry, FAngleRotation, [RotCenterX|RotCenterY], RotatedGeometry),
    print_log(('Output - RotatedGeometry: ', RotatedGeometry), 3),
    print_log('END - rotation...', 3).
% TRANSLATION
translation(ConnHandler, Geometry, (DeltaX, DeltaY), TranslatedGeometry) :-
    print_log('START - translation...', 3),
    print_log(('Input - Geometry: ', Geometry), 3),
    print_log(('Input - (DeltaX, DeltaY): ', (DeltaX, DeltaY)), 3),
    st_translate(ConnHandler, Geometry, [DeltaX|DeltaY], TranslatedGeometry),
    print_log(('Output - TranslatedGeometry: ', TranslatedGeometry), 3),
    print_log('END - translation...', 3).
% FLOAT FUNCTION - perform a query and return the output as float
query(ConnHandler, Query, Output) :-
    print_log('START - query...', 3),
    print_log(('Input - Query: ', Query), 3),
    st_function(ConnHandler, Query, Output),
    print_log(('Output - Output: ', Output), 3),
    print_log('END - query...', 3).

% return the X/Y max/min coordsinate of the bounding rectangle of a geometry
st_bounding_rectangle_coordinate(ConnHandler, Shape, CoordinateName, Coordinate) :-
    print_log('START - st_bounding_rectangle_coordinate...', 3),
    print_log(('Input - Shape: ', Shape), 3),
    print_log(('Input - CoordinateName: ', CoordinateName), 3),
    geometry_to_string(Shape, ShapeString),
    st_query_list_helper(CoordinateName, [ShapeString], StringsList),
    st_query_f(ConnHandler, StringsList, Coordinate),
    print_log(('Output - Coordinate: ', Coordinate), 3),
    print_log('END - st_bounding_rectangle_coordinate...', 3).

st_xmax(ConnHandler, Shape, MaxX) :-
    print_log('START - st_xmax...', 3),
    print_log(('Input - Shape: ', Shape), 3),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMax', MaxX),
    print_log(('Output - MaxX: ', MaxX), 3),
    print_log('END - st_xmax...', 3).
st_ymax(ConnHandler, Shape, MaxY) :-
    print_log('START - st_ymax...', 3),
    print_log(('Input - Shape: ', Shape), 3),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMax', MaxY),
    print_log(('Output - MaxY: ', MaxY), 3),
    print_log('END - st_ymax...', 3).
st_xmin(ConnHandler, Shape, MinX) :-
    print_log('START - st_xmin...', 3),
    print_log(('Input - Shape: ', Shape), 3),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMin', MinX),
    print_log(('Output - MinX: ', MinX), 3),
    print_log('END - st_xmin...', 3).
st_ymin(ConnHandler, Shape, MinY) :-
    print_log('START - st_ymin...', 3),
    print_log(('Input - Shape: ', Shape), 3),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMin', MinY),
    print_log(('Output - MinY: ', MinY), 3),
    print_log('END - st_ymin...', 3).

% Check if GeomB is completely contained within GeomB
st_contain(ConnHandler, GeomA, GeomB, Output) :-
    print_log('START - st_contain...', 3),
    print_log(('Input - GeomA: ', GeomA), 3),
    print_log(('Input - GeomB: ', GeomB), 3),
    geometry_to_string(GeomA, GeomAString),
    geometry_to_string(GeomB, GeomBString),
    st_query_list_helper('ST_Contains', [GeomAString, GeomBString], StringsList),
    st_query(ConnHandler, StringsList, Tmp),
    string_to_boolean(Tmp, Output),
    print_log(('Output - Containment: ', Output), 3),
    print_log('END - st_contain...', 3).

% ------ BEGINNING - QUERY UTILS------
% Predicates for execute a generic PostGIS function

% Perform a query and return the output as string
st_query(ConnHandler, StringList, Output) :-
    print_log('START - st_query...', 3),
    concatenate_strings(StringList,StrLst),
    atom_concat('SELECT ',StrLst, Query),
    % print_log('Log - StrLst: ', StrLst), 3),
    % print_log('Log - Query: ', Query), 3),
    query(ConnHandler, Query, Output)
    % ,print_log('END - st_query...\n')
    .

% Perform a query and return the output as float
st_query_f(ConnHandler, StringList, Output) :-
    print_log('START - st_query_f...', 3),
    print_log(('Input - StringList: ', StringList), 3),
    st_query(ConnHandler, StringList, Tmp),
    string_to_float(Tmp, Output),
    print_log(('Output - Query Result (Float): ', Output), 3),
    print_log('END - st_query_f...', 3).

% Create the following string: FunctionName(Param1, Param2, ...)
st_query_list_helper(FunctionName, ParamsList, OutputList) :-
    print_log('START - st_query_list_helper...', 3),
    separate_with_commas(ParamsList, ParamsString),
    OutputList = [
        FunctionName,
        '(',
        ParamsString,
        ')'
    ],
    print_log(('Output - Query List: ', OutputList), 3),
    print_log('END - st_query_list_helper...', 3).

% Transform a geometry in a string to be added in a query
geometry_to_string(Geometry, GeometryString) :-
    print_log('START - geometry_to_string...', 3),
    print_log(('Input - Geometry: ', Geometry), 3),
    yap_predicate_to_WKT(Geometry, WKTGeometry),
    add_single_quotes(WKTGeometry, WKTGeometryQuoted),
    % atom_string(WKTGeometryQuoted, WKTGeometryQuotedString),
    WKTGeometryQuotedString = WKTGeometryQuoted, 
    st_query_list_helper('ST_GeomFromText',[WKTGeometryQuotedString] , StringsList),
    concatenate_strings(StringsList, GeometryString),
    print_log(('Output - GeometryString: ', GeometryString), 3),
    print_log('END - geometry_to_string...', 3).