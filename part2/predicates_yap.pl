:- load_foreign_files(['predicates_yap'], [], init_predicates).

%geom_difference/4.
%geom_rotation/4.
%geom_translation/4.
%geom_translation/5.
%yap_predicate_to_WKT/2.

% yap_predicate_to_WKT(YapPointsList, WKTString).

degrees_to_radians(Degrees, Radians) :-
    Radians is Degrees * pi / 180.

% DIFFERENCE
geom_difference(ConnName, Geometry1, Geometry2, DifferenceGeometry) :- 
    get_value(ConnName, ConnHandler),
    st_difference(ConnHandler, Geometry1, Geometry2, DifferenceGeometry).

% ROTATION
geom_rotation(ConnName, Geometry, DegreesAngleRotation, RotatedGeometry) :- 
    get_value(ConnName, ConnHandler),
    degrees_to_radians(DegreesAngleRotation, RadiantsAngleRotation),
    st_rotate(ConnHandler, Geometry, RadiantsAngleRotation, RotatedGeometry).

% TRANSLATION
geom_translation(ConnName, Geometry, (DeltaX, DeltaY), TranslatedGeometry) :- 
    get_value(ConnName, ConnHandler),
    st_translate(ConnHandler, Geometry, [DeltaX|DeltaY], TranslatedGeometry).

geom_translation(ConnName, Geometry, DeltaX, DeltaY, TranslatedGeometry) :-
    st_translate(ConnName, Geometry, (DeltaX, DeltaY), TranslatedGeometry).