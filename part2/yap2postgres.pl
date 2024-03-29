:- load_foreign_files(['yap2postgres'], [], init_predicates).

%db_open/5.
%db_close/1.
%db_import/3.

db_open(Host, User, Passwd, Database, ConnName) :-
    db_connect(Host, User, Passwd, Database, ConnHandler),
    set_value(ConnName, ConnHandler).

db_close(ConnName) :- 
    get_value(ConnName, ConnHandler),
    db_disconnect(ConnHandler).

% db_import(RelationName, PredName, ConnName) :-
%     get_value(ConnName, ConnHandler),
%     db_assert(ConnHandler, RelationName, PredName).

db_import(RelationName, PredName, ConnName) :-
    get_value(ConnName, ConnHandler),
    db_arity(ConnHandler, RelationName, Arity),
    functor(F, PredName, Arity), % puts in F aterm with same functor and arity of random arguments eg pieces(10303,112810,19292,1019)
    F =..[_ | LArgs], % this gives as output the name of the predicate (PredName) which is ignored, and the random arguments generated by functor eg [191738,18128,..,]: https://www.dontveter.com/basisofai/12.html
    atom_concat('select * from ', RelationName, SQL),
    assert(
        ':-'(F,','(
                db_query(ConnHandler, SQL, ResultSet), 
                db_row(ResultSet, LArgs, ConnHandler)
            )
        )
    ).


% to test db import PIECES -> get_value(conn, ConnHandler), db_query(ConnHandler, 'select * from pieces', ResultSet), db_row(ResultSet, [A,B,C,D], ConnHandler).
% to test db import SOLUTIONS -> get_value(conn, ConnHandler), db_query(ConnHandler, 'select * from solutions', ResultSet), db_row(ResultSet, [A,B,C], ConnHandler).
% to test db import PUZZLES -> consult(yap2postgres), db_open(localhost, postgres, 'gnazio', tangram_puzzle, conn), get_value(conn, ConnHandler), db_query(ConnHandler, 'select * from puzzles', ResultSet), db_row(ResultSet, [A,B, C], ConnHandler).
% db_import(puzzles, puzzles, conn).
% db_import(solutions, solutions, conn).
% p(A, B).
% solutions(A, B, C).