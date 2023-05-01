:- consult(yap2mysql).
:- db_open(localhost, davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
:- db_import(solutions, solutions, conn).

% :- get_value(conn, ConnHandler),db_arity(ConnHandler, pieces, Arity).
:- db_close(conn).
