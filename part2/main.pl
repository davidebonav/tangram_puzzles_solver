:- consult(yap2postgres).
:- consult(predicates_yap).

:- db_open('192.168.175.132', davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
:- db_import(solutions, solutions, conn).

:- nl,write('To print the terms:\n'),
    write('\t-. pieces(Id, Name, Color, Shape).\n'),
    write('\t-. puzzles(Id, Name, Shape).\n'),
    write('\t-. puzzles(PuzzleId, PieceId, Shape).\n'),
    nl.

:- nl,write('To test the predicate:\n'),
    write('\t-. run test_translate(WKTOriginalShape, WKTTranslatedShape) to test the translate predicate.\n'),
    write('\t-. run test_rotate(WKTOriginalShape, WKTRotatedShape) to test the rotate predicate.\n'),
    write('\t-. run test_difference(ShapeGeometry1, ShapeGeometry2, IdGeometry2, Difference) to test the difference predicate.\n'),
    write('\t-. run test_parser(PieceName, PieceColor, PieceShape) to test the parser.\n'),
    nl.

% Tests
test_parser(PieceName, PieceColor, PieceShape) :-
    write('Testing the parser printing the shape of all the pieces...\n'),
    pieces(_, PieceName, PieceColor, Shape),
    yap_predicate_to_WKT(Shape, PieceShape).

test_translate(WKTOriginalShape, WKTTranslatedShape) :-
    write('Testing translation t_vector=(1.0, 1.0)...\n'),
    pieces(_,_,_,OriginalShape),
    geom_translation(conn, OriginalShape, (1.0, 1.0), TranslatedShape),
    yap_predicate_to_WKT(OriginalShape, WKTOriginalShape), 
    yap_predicate_to_WKT(TranslatedShape, WKTTranslatedShape).

test_rotate(WKTOriginalShape, WKTRotatedShape) :-
    write('Testing rotation r_angle=90°...\n'),
    pieces(_,_,_,OriginalShape),
    geom_rotation(conn, OriginalShape, 90.0, RotatedShape),
    yap_predicate_to_WKT(OriginalShape, WKTOriginalShape), 
    yap_predicate_to_WKT(RotatedShape, WKTRotatedShape).

test_difference(WKTShapeGeometry1, WKTShapeGeometry2, IdGeometry2, WKTDifference) :-
    write('Testing testing the difference of the first pieces with all other pieces...\n'),
    pieces('1',_,_,ShapeGeometry1),
    pieces(IdGeometry2,_,_,ShapeGeometry2),
    geom_difference(conn, ShapeGeometry1, ShapeGeometry2, Difference),
    yap_predicate_to_WKT(ShapeGeometry1, WKTShapeGeometry1), 
    yap_predicate_to_WKT(ShapeGeometry2, WKTShapeGeometry2),
    yap_predicate_to_WKT(Difference, WKTDifference).