:- consult('../part2/yap2postgres').
:- consult(tangram_solver).

:- db_open('192.168.175.132', davide, 'davide', tangram_puzzle, conn).
:- db_import(pieces, pieces, conn).
:- db_import(puzzles, puzzles, conn).
% :- db_import(solutions, solutions, conn).

% 0 - zero log
% 1 - minimum log
% 2 - medium log
% 3 - maximum log
:- set_value(debugLevel, 1).

:- tangram_solver(conn, '1', _Solutions).

:- db_close(conn).