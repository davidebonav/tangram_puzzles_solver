:- consult('./utility/solver_utils').

% Main term of the solver. Try to solve with backtracking 
% the puzzle with id PuzzleId and put all the founded solution
% in Solutions 
% INPUT : ConnName, PuzzleId 
% OUTPUT : Solutions
tangram_solver(ConnName, PuzzleId, Solutions) :- 
        get_value(ConnName, ConnHandler),
        loadData(PuzzleId, PuzzleShape, PiecesList),
        solve_tangram_helper(ConnHandler, PuzzleShape, PiecesList, Solutions).

% Load all the needed data
% INPUT : PuzzleId
% OUTPUT : PuzzleShape, PiecesList
loadData(PuzzleId, PuzzleShape, PiecesList) :-
    loadPiecesList(PiecesList),
    loadPuzzleShape(PuzzleId, PuzzleShape).

% Generate a list of the shape and the color of all pieces
% INPUT : 
% OUTPUT : PiecesList
loadPiecesList(PiecesList) :-
    findall((PieceColor,PieceShape),(pieces(_, _, PieceColor, PieceShape)), PiecesList).

% Load the shape of the puzzle to be solved
% INPUT : PuzzleId
% OUTPUT : PuzzleShape
loadPuzzleShape(PuzzleId, PuzzleShape) :-
    puzzles(PuzzleId, _, PuzzleShape).

% Recursive term that try to find all the solution of a puzzle
% using backtracking
% INPUT : ConnHandler, PuzzleShape, PiecesList
% OUTPUT : Solutions
solve_tangram_helper(_, _, [], _).
solve_tangram_helper(ConnHandler, PuzzleShape, PiecesList, [X|Tail]) :-
    shift_to_origin(ConnHandler, PuzzleShape, ShiftedPuzzleShape),
    select((PieceColor, PieceShape), PiecesList, RemainPiecesList),
    insert_piece(ConnHandler, ShiftedPuzzleShape, PieceShape, ResultPieceShape),
    difference(ConnHandler, ShiftedPuzzleShape, ResultPieceShape, RemainingPuzzleShape),
    X = (PieceColor, ResultPieceShape),
    solve_tangram_helper(ConnHandler, RemainingPuzzleShape, RemainPiecesList, Tail).

% Shift the shape of the geometru so that the lower left corner is at the origin
shift_to_origin(ConnHandler, Shape, ShiftedShape) :-
    st_xmin(ConnHandler, Shape, DeltaX),
    st_ymin(ConnHandler, Shape, DeltaY),
    st_translate(ConnHandler, Shape, [-DeltaX|-DeltaY], ShiftedShape).

% Try to insert the piece in the puzzle
% INPUT : ConnHandler, PuzzleShape, PieceShape 
% OUTPUT : ResultPieceShape
insert_piece(ConnHandler, PuzzleShape, PieceShape, ResultPieceShape) :-
    shift_to_origin(ConnHandler, PieceShape, SPieceShape),
    can_fit(ConnHandler, PuzzleShape, SPieceShape),
    generate_rotation_translation_list(PuzzleShape, Combinations),
    find_first(can_insert((ConnHandler, SPieceShape, PuzzleShape)), Combinations, [RotAng|[Tx|Ty]]),
    transform_piece(ConnHandler, SPieceShape, RotAng, (Tx, Ty), ResultPieceShape).