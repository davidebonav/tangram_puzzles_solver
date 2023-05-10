:- consult(yap2postgres).
:- consult(predicates_yap).

:- db_open('192.168.175.132', davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
:- db_import(solutions, solutions, conn).