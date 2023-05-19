% This predicate allow to find the first element of a list of element that satisfies 
% a input predicate
% 
% An example of use is the following
% test_good(PieceShape, (Rotation, [Tx|Ty])) :-
%     Sum is Rotation + Tx + Ty,
%     PieceShape =:= Sum.
% find_first(test_good(5), [(1,[1|1]),(1,[1|2]),(35,[1|1]),(5,[1|1]),(1,[25|2])], Result).
% 
% Recursively search for the first element in the remaining list that satisfies the predicate
find_first(Predicate, [_|Tail], Result) :-
  find_first(Predicate, Tail, Result).
%  call(Predicate, Result).
% 
% No element found, predicate fails
find_first(_, [], _) :-
  fail.
% 
% First element satisfies the predicate
find_first(Predicate, [X|_], X) :-
  call(Predicate, X), !. 

% select/2 predicate selects the first element from a list of elemetn
select(Tuple, [Tuple | Rest], Rest).
select(Tuple, [Other | Rest], [Other | Remaining]) :-
    select(Tuple, Rest, Remaining).

% implementation of member prolog operator
member_impl(X, [X|_]).
member_impl(X, [_|T]) :- member_impl(X, T).

% Generate a list of all the possibiles combination between two vector in input
generate_combinations(Vector1, Vector2, Combinations) :-
  findall([X|Y], (member_impl(X, Vector1), member_impl(Y, Vector2)), Combinations).

% Add the single quotes to the input string
add_single_quotes(String, Result) :-
    atom_concat('\'', String, Temp),
    atom_concat(Temp, '\'', Result).

% Create a string that contain all the string in a list of strings, separated by comma
separate_with_commas([], '') :- !.
separate_with_commas([X], X) :- !.
separate_with_commas([X|Xs], Result) :-
    separate_with_commas(Xs, Temp),
    atomic_list_concat([X, ', ', Temp], Result),
    !.

% allow to concatenate multiple string in a single string
concatenate_strings([], '').
concatenate_strings([String|Rest], Result) :-
    concatenate_strings(Rest, Temp),
    atom_concat(String, Temp, Result).

% Convert a string term to a float term
string_to_float(String, Float) :-
    atom(String), % Ensure the input is an atom or string
    atom_number(String, Number),
    Float is float(Number).

% convert t string to true term and f string to false term
string_to_boolean('f', false).
string_to_boolean('t', true).

% concatenate two different list in a single list
concatenate([], List, List).
concatenate([X|Xs], List, [X|Result]) :-
    concatenate(Xs, List, Result).

% convert integer term to float term
integer_to_float(Int, Float) :-
    Float is Int / 1.0.

% allow to print log considering the debug level
print_log(Text, LogLevel) :-
    get_value(debugLevel, DebugLevel),
    (LogLevel =< DebugLevel -> 
        (write(Text), nl) ; true).

% take in input a list of list (a list with more than 1 dimension) 
% and transform in a flat list
flatten_list([], []).
flatten_list([[]|T], FlatList) :-
  flatten_list(T, FlatList).
flatten_list([H|T], FlatList) :-
  is_list(H),
  flatten_list(H, FlatHead),
  flatten_list(T, FlatTail),
  concatenate(FlatHead, FlatTail, FlatList).
flatten_list([H|T], [H|FlatTail]) :-
  \+ is_list(H),
  flatten_list(T, FlatTail).

% transform a list in another list using the predicate passed in TransformRule
transform_list([], _, []). % Base case: Empty list transforms to an empty list.
transform_list([X|Xs], TransformRule, [Y|Ys]) :-
  call(TransformRule, X, Y),   % Call the transformation rule with X and obtain Y.
  transform_list(Xs, TransformRule, Ys). 

% -------------------- NOT USED --------------------

% Transform an angle in degree in an angle in radians
degrees_to_radians(Degrees, Radians) :-
    Radians is Degrees * pi / 180.

print_term_type(Term) :-
    (   var(Term)
    ->  write('Term is a variable')
    ;   atom(Term)
    ->  write('Term is an atom')
    ;   integer(Term)
    ->  write('Term is an integer')
    ;   float(Term)
    ->  write('Term is a float')
    ;   string(Term)
    ->  write('Term is a string')
    ;   compound(Term)
    ->  write('Term is a compound')
    ;   write('Term is of an unknown type')
    ),
    nl.