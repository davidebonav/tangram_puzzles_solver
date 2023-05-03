:- load_foreign_files(['yap2postgres'], [], init_predicates).
%:- consult(sqlcompiler).

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
    functor(F, PredName, Arity),
    F =..[_ | LArgs],
    atom_concat('select * from ', RelationName, SQL),
    assert(
        ':-'(F,','(
                db_query(ConnHandler, SQL, ResultSet), 
                db_row(ResultSet, LArgs)
            )
        )
    ).