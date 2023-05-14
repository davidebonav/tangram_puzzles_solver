:- load_foreign_files(['../part2/predicates_yap'], [], init_predicates).

% Transform an angle in degree in an angle in radians
degrees_to_radians(Degrees, Radians) :-
    Radians is Degrees * pi / 180.

% DIFFERENCE
difference(ConnName, Geometry1, Geometry2, DifferenceGeometry) :- 
    st_difference(ConnName, Geometry1, Geometry2, DifferenceGeometry).
% ROTATION
rotation(ConnHandler, Geometry, AngleRotation, RotatedGeometry) :- 
    st_rotate(ConnHandler, Geometry, AngleRotation, RotatedGeometry).
% TRANSLATION
translation(ConnHandler, Geometry, (DeltaX, DeltaY), TranslatedGeometry) :- 
    st_translate(ConnHandler, Geometry, [DeltaX|DeltaY], TranslatedGeometry).

% select/2 predicate selects the first element from a list of elemetn
select(Tuple, [Tuple | Rest], Rest).
select(Tuple, [Other | Rest], [Other | Remaining]) :-
    select(Tuple, Rest, Remaining).

% Generate all the rotation angles that can be used
generate_rotation_angles_list(OutputList) :-
  findall(Degree, (between(0, 7, X), Degree is X * 45), OutputList).

% Generate a list of all number N : 0 <= N <= Number and N is multiple of 0.25 
generate_multiples(Number, List) :-
  UpperBound is floor(Number * 4),
  findall(Value, (between(0, UpperBound, X), Value is X / 4), List).

% Generate a list of all the possibiles combination between two vector in input
generate_combinations(Vector1, Vector2, Combinations) :-
  findall([X|Y], (member(X, Vector1), member(Y, Vector2)), Combinations).

% Generate a list containing a list with all possible translation vector that can be used
generate_translation_vector_list(MaxX, MaxY, OutputList) :-
    generate_multiples(MaxX, XList),
	generate_multiples(MaxY, YList),
	generate_combinations(XList, YList, OutputList).

% generate the list of all possibile combination beteen all possible rotation and all possible combination
generate_rotation_translation_list(PuzzleShape, Combinations) :-
    generate_rotation_angles_list(RotationList),
    st_xmax(ConnHandler, PuzzleShape, MaxX),
    st_ymax(ConnHandler, PuzzleShape, MaxY),
    generate_translation_vector_list(MaxX, MaxY, TranslationList),
    generate_combinations(RotationList, TranslationList, Combinations).

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


% STILL TODO 
st_xmax(ConnHandler, Shape, MaxX), % TODO
st_ymax(ConnHandler, Shape, MaxY), % TODO
st_xmin(ConnHandler, Shape, DeltaX), % TODO
st_ymin(ConnHandler, Shape, DeltaY), % TODO
can_fit(ConnHandler, PuzzleShape, SPieceShape), % check the area of the pieces and of the puzzle and understand if the pieces can be added or not
can_insert((SPieceShape,PuzzleShape), [RotAng|[Tx|Ty]]). % TODO