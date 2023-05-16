:- consult(yap2postgres).
:- consult(predicates_yap).

:- db_open('192.168.175.132', davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
:- db_import(solutions, solutions, conn).

% Try
try_print(A) :-
    pieces(_,_,_,Shape), %yap_predicate_to_WKT(Shape, A).
    yap_predicate_to_WKT(Shape, A).

try_transalte(A) :-
    pieces(_,_,_,Shape), %yap_predicate_to_WKT(Shape, A).
    geom_translation(conn, Shape, (1.0, 1.0), A).

try_rotate(A, Shape) :-
    pieces(_,_,_,Shape),
    geom_rotation(conn, Shape, 90.0, A).

try_diff(Difference, ShapeGeometry1, ShapeGeometry2, IdGeometry2) :-
    pieces('1',_,_,ShapeGeometry1),
    pieces(IdGeometry2,_,_,ShapeGeometry2),
    geom_difference(conn, ShapeGeometry1, ShapeGeometry2, Difference).