:- consult(yap2postgres).
:- db_open('192.168.175.132', davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
:- db_import(solutions, solutions, conn).

table_arity(ConnName,TableName,Arity) :-
    get_value(ConnName,ConnHandler),db_arity(ConnHandler,TableName,Arity).

% :- get_value(conn, ConnHandler),db_arity(ConnHandler, pieces, Arity).
% :- db_close(conn).
% :- halt.