:- load_foreign_files(['../../part2/predicates_yap'], [], init_predicates).
:- consult(utils).

% Method renaming
% DIFFERENCE
difference(ConnName, Geometry1, Geometry2, DifferenceGeometry) :-
    write('START - difference...\n'),
    st_difference(ConnName, Geometry1, Geometry2, DifferenceGeometry),
    write('END - difference...\n').
% ROTATION
rotation(ConnHandler, Geometry, AngleRotation, RotatedGeometry) :-
    write('START - rotation...\n'),
    st_rotate(ConnHandler, Geometry, AngleRotation, RotatedGeometry),
    write('END - rotation...\n').
% TRANSLATION
translation(ConnHandler, Geometry, (DeltaX, DeltaY), TranslatedGeometry) :-
    write('START - translation...\n'),
    st_translate(ConnHandler, Geometry, [DeltaX|DeltaY], TranslatedGeometry),
    write('END - translation...\n').
% FLOAT FUNCTION - perform a query and return the output as float
query(ConnHandler, Query, Output) :-
    write('START - query...\n'),
    st_function(ConnHandler, Query, Output),
    write('END - query...\n').

% return the X/Y max/min coordsinate of the bounding rectangle of a geometry
st_bounding_rectangle_coordinate(ConnHandler, Shape, CoordianteName, Coordinate) :-
    write('START - st_bounding_rectangle_coordinate...\n'),
    geometry_to_string(Shape, ShapeString),
    st_query_list_helper(CoordianteName, [ShapeString], StringsList),
    st_query_f(ConnHandler, StringsList, Coordinate),
    write('END - st_bounding_rectangle_coordinate...\n').

st_xmax(ConnHandler, Shape, MaxX) :-
    write('START - st_xmax...\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMax', MaxX),
    write('END - st_xmax...\n').
st_ymax(ConnHandler, Shape, MaxY) :-
    write('START - st_ymax...\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMax', MaxY),
    write('END - st_ymax...\n').
st_xmin(ConnHandler, Shape, MinX) :-
    write('START - st_xmin...\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMin', MinX),
    write('END - st_xmin...\n').
st_ymin(ConnHandler, Shape, MinY) :-
    write('START - st_ymin...\n'),
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMin', MinY),
    write('END - st_ymin...\n').

% Check if GeomB is completely contained within GeomB
st_contain(ConnHandler, GeomA, GeomB, Output) :-
    write('START - st_contain...\n'),
    geometry_to_string(GeomA, GeomAString),
    geometry_to_string(GeomB, GeomBString),
    st_query_list_helper('ST_Contains', [GeomAString, GeomBString], StringsList),
    st_query(ConnHandler, StringsList, Tmp),
    string_to_boolean(Tmp, Output),
    write('END - st_contain...\n').

% ------ BEGINNING - QUERY UTILS------
% Predicates for execute a generic PostGIS function

% Perform a query and return the output as string
st_query(ConnHandler, StringList, Output) :-
    write('START - st_query...\n'),
    append(['SELECT '], StringList, StrLst),
    concatenate_strings(StrLst, Query),
    write(Query), nl,
    query(ConnHandler, Query, Output),
    write('END - st_query...\n').

% Perform a query and return the output as float
st_query_f(ConnHandler, StringList, Output) :-
    write('START - st_query_f...\n'),
    st_query(ConnHandler, StringList, Tmp),
    string_to_float(Tmp, Output),
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
    write('END - st_query_list_helper...\n').

% Transform a geometry in a string to be added in a query
geometry_to_string(Geometry, GeometryString) :-
    write('START - geometry_to_string...\n'),
    yap_predicate_to_WKT(Geometry, WKTGeometry),
    atom_string(WKTGeometry, WKTGeometryString),
    add_single_quotes(WKTGeometryString, WKTGeometryStringQuoted),
    st_query_list_helper('ST_GeomFromText',[WKTGeometryStringQuoted] , StringsList),
    concatenate_strings(StringsList, GeometryString),
    write('END - geometry_to_string...\n').