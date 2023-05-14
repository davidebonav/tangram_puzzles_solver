:- load_foreign_files(['../../part2/predicates_yap'], [], init_predicates).
:- consult(utils).

% Method renaming
% DIFFERENCE
difference(ConnName, Geometry1, Geometry2, DifferenceGeometry) :- 
    st_difference(ConnName, Geometry1, Geometry2, DifferenceGeometry).
% ROTATION
rotation(ConnHandler, Geometry, AngleRotation, RotatedGeometry) :- 
    st_rotate(ConnHandler, Geometry, AngleRotation, RotatedGeometry).
% TRANSLATION
translation(ConnHandler, Geometry, (DeltaX, DeltaY), TranslatedGeometry) :- 
    st_translate(ConnHandler, Geometry, [DeltaX|DeltaY], TranslatedGeometry).
% FLOAT FUNCTION - perform a query and return the output as float
query(ConnHandler, Query, Output) :- 
    st_function(ConnHandler, Query, Output).

% return the X/Y max/min coordsinate of the bounding rectangle of a geometry
st_bounding_rectangle_coordinate(ConnHandler, Shape, CoordianteName, Coordinate) :-
    geometry_to_string(Shape, ShapeString),
    st_query_list_helper(CoordianteName, [ShapeString], StringsList),
    st_query_f(ConnHandler, StringsList, Coordinate).

st_xmax(ConnHandler, Shape, MaxX) :-
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMax', MaxX).
st_ymax(ConnHandler, Shape, MaxY) :-
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMax', MaxY).
st_xmin(ConnHandler, Shape, MinX) :-
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_XMin', MinX).
st_ymin(ConnHandler, Shape, MinY) :-
    st_bounding_rectangle_coordinate(ConnHandler, Shape, 'ST_YMin', MinY).

% Check if GeomB is completely contained within GeomB
st_contain(ConnHandler, GeomA, GeomB, Output) :-
    geometry_to_string(GeomA, GeomAString),
    geometry_to_string(GeomB, GeomBString),
    st_query_list_helper('ST_Contains', [GeomAString, GeomBString], StringsList),
    st_query(ConnHandler, StringsList, Tmp),
    string_to_boolean(Tmp, Output).

% ------ BEGINNING - QUERY UTILS------
% Predicates for execute a generic PostGIS function

% Perform a query and return the output as string
st_query(ConnHandler, StringList, Output) :-
    append(['SELECT '], StringList, StrLst),
    concatenate_strings(StrLst, Query),
    write(Query), nl,
    query(ConnHandler, Query, Output).

% Perform a query and return the output as float
st_query_f(ConnHandler, StringList, Output) :-
    st_query(ConnHandler, StringList, Tmp),
    string_to_float(Tmp, Output).

% Create the following string: FunctionName(Param1, Param2, ...)
st_query_list_helper(FunctionName, ParamsList, OutputList) :-
    separate_with_commas(ParamsList, ParamsString),
    OutputList = [
        FunctionName,
        '(',
        ParamsString,
        ')'
    ].

% Transform a geometry in a string to be added in a query
geometry_to_string(Geometry, GeometryString) :-
    yap_predicate_to_WKT(Geometry, WKTGeometry),
    atom_string(WKTGeometry, WKTGeometryString),
    add_single_quotes(WKTGeometryString, WKTGeometryStringQuoted),
    st_query_list_helper('ST_GeomFromText',[WKTGeometryStringQuoted] , StringsList),
    concatenate_strings(StringsList, GeometryString).