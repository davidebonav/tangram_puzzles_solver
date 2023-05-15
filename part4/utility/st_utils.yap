:- load_foreign_files(['../../part2/predicates_yap'], [], init_predicates).
:- consult(utils).

% Method renaming
% DIFFERENCE
difference(ConnName, Geometry1, Geometry2, DifferenceGeometry) :-
    write('START - difference...\n'),
    write('Input - Geometry1: '), write(Geometry1), write('\n'),
    write('Input - Geometry2: '), write(Geometry2), write('\n'),
    st_difference(ConnName, Geometry1, Geometry2, DifferenceGeometry),
    write('Output - DifferenceGeometry: '), write(DifferenceGeometry), write('\n'),
    write('END - difference...\n').
% ROTATION
rotation(ConnHandler, Geometry, IAngleRotation, RotatedGeometry) :-
    write('START - rotation...\n'),
    write('Input - Geometry: '), write(Geometry), write('\n'),
    write('Input - IAngleRotation: '), write(IAngleRotation), write('\n'),
    integer_to_float(IAngleRotation, FAngleRotation),
    st_rotate(ConnHandler, Geometry, FAngleRotation, RotatedGeometry),
    write('Output - RotatedGeometry: '), write(RotatedGeometry), write('\n'),
    write('END - rotation...\n').
% TRANSLATION
translation(ConnHandler, Geometry, (DeltaX, DeltaY), TranslatedGeometry) :-
    write('START - translation...\n'),
    write('Input - Geometry: '), write(Geometry), write('\n'),
    write('Input - (DeltaX, DeltaY): '), write((DeltaX, DeltaY)), write('\n'),
    st_translate(ConnHandler, Geometry, [DeltaX|DeltaY], TranslatedGeometry),
    write('Output - TranslatedGeometry: '), write(TranslatedGeometry), write('\n'),
    write('END - translation...\n').
% FLOAT FUNCTION - perform a query and return the output as float
query(ConnHandler, Query, Output) :-
    write('START - query...\n'),
    write('Input - Query: '), write(Query), write('\n'),
    st_function(ConnHandler, Query, Output),
    write('Output - Output: '), write(Output), write('\n'),
    write('END - query...\n').

% return the X/Y max/min coordsinate of the bounding rectangle of a geometry
st_bounding_rectangle_coordinate(ConnHandler, Shape, CoordianteName, Coordinate) :-
    write('START - st_bounding_rectangle_coordinate...\n'),
    write('Input - Shape: '), write(Shape), write('\n'),
    write('Input - CoordinateName: '), write(CoordinateName), write('\n'),
    geometry_to_string(Shape, ShapeString),
    st_query_list_helper(CoordianteName, [ShapeString], StringsList),
    st_query_f(ConnHandler, StringsList, Coordinate),
    write('Output - Coordinate: '), write(Coordinate), write('\n'),
    write('END - st_bounding_rectangle_coordinate...\n').

st_xmax(ConnHandler, Shape, MaxX) :-
    write('START - st_xmax...\n'),
    % write('Input - Shape: '), write(Shape), write('\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMax', MaxX),
    % write('Output - MaxX: '), write(MaxX), write('\n'),
    write('END - st_xmax...\n').
st_ymax(ConnHandler, Shape, MaxY) :-
    write('START - st_ymax...\n'),
    % write('Input - Shape: '), write(Shape), write('\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMax', MaxY),
    % write('Output - MaxY: '), write(MaxY), write('\n'),
    write('END - st_ymax...\n').
st_xmin(ConnHandler, Shape, MinX) :-
    write('START - st_xmin...\n'),
    % write('Input - Shape: '), write(Shape), write('\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMin', MinX),
    % write('Output - MinX: '), write(MinX), write('\n'),
    write('END - st_xmin...\n').
st_ymin(ConnHandler, Shape, MinY) :-
    write('START - st_ymin...\n'),
    % write('Input - Shape: '), write(Shape), write('\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMin', MinY),
    % write('Output - MinY: '), write(MinY), write('\n'),
    write('END - st_ymin...\n').

% Check if GeomB is completely contained within GeomB
st_contain(ConnHandler, GeomA, GeomB, Output) :-
    write('START - st_contain...\n'),
    write('Input - GeomA: '), write(GeomA), write('\n'),
    write('Input - GeomB: '), write(GeomB), write('\n'),
    geometry_to_string(GeomA, GeomAString),
    geometry_to_string(GeomB, GeomBString),
    st_query_list_helper('ST_Contains', [GeomAString, GeomBString], StringsList),
    st_query(ConnHandler, StringsList, Tmp),
    string_to_boolean(Tmp, Output),
    write('Output - Containment: '), write(Output), write('\n'),
    write('END - st_contain...\n').

% ------ BEGINNING - QUERY UTILS------
% Predicates for execute a generic PostGIS function

% Perform a query and return the output as string
st_query(ConnHandler, StringList, Output) :-
    write('START - st_query...\n'),
    concatenate_strings(StringList,StrLst),
    atom_concat('SELECT ',StrLst, Query),
    % write('Log - StrLst: '), write(StrLst), write('\n'),
    % write('Log - Query: '), write(Query), write('\n'),
    query(ConnHandler, Query, Output),
    write('END - st_query...\n').

% Perform a query and return the output as float
st_query_f(ConnHandler, StringList, Output) :-
    write('START - st_query_f...\n'),
    write('Input - StringList: '), write(StringList), write('\n'),
    st_query(ConnHandler, StringList, Tmp),
    string_to_float(Tmp, Output),
    write('Output - Query Result (Float): '), write(Output), write('\n'),
    write('END - st_query_f...\n').

% Create the following string: FunctionName(Param1, Param2, ...)
st_query_list_helper(FunctionName, ParamsList, OutputList) :-
    write('START - st_query_list_helper...\n'),
    separate_with_commas(ParamsList, ParamsString),
    OutputList = [
        FunctionName,
        '(',
        ParamsString,
        ')'
    ],
    % write('Output - Query List: '), write(OutputList), write('\n'),
    write('END - st_query_list_helper...\n').

% Transform a geometry in a string to be added in a query
geometry_to_string(Geometry, GeometryString) :-
    write('START - geometry_to_string...\n'),
    write('Input - Geometry: '), write(Geometry), write('\n'),
    yap_predicate_to_WKT(Geometry, WKTGeometry),
    add_single_quotes(WKTGeometry, WKTGeometryQuoted),
    % atom_string(WKTGeometryQuoted, WKTGeometryQuotedString),
    WKTGeometryQuotedString = WKTGeometryQuoted, 
    st_query_list_helper('ST_GeomFromText',[WKTGeometryQuotedString] , StringsList),
    concatenate_strings(StringsList, GeometryString),
    write('Output - GeometryString: '), write(GeometryString), write('\n'),
    write('END - geometry_to_string...\n').