:- consult('../part2/yap2postgres').
:- consult(tangram_solver).

:- db_open('192.168.175.132', davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
% :- db_import(solutions, solutions, conn).

:- tangram_solver(conn, '1', _Solutions).
