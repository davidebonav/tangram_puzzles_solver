:- consult('./utility/solver_utils').

% Main term of the solver. Try to solve with backtracking 
% the puzzle with id PuzzleId and put all the founded solution
% in Solutions 
% INPUT : ConnName, PuzzleId 
% OUTPUT : Solutions
tangram_solver(ConnName, PuzzleId, Solutions) :-
    print_log(('START PuzzleId: ', PuzzleId), 3),
    get_value(ConnName, ConnHandler),
    loadData(PuzzleId, PuzzleShape, PiecesList),
    solve_tangram_helper(ConnHandler, PuzzleShape, PiecesList, Solutions),
    print_log('END - tangram_solver...', 3).

% Load all the needed data
% INPUT : PuzzleId
% OUTPUT : PuzzleShape, PiecesList
loadData(PuzzleId, PuzzleShape, PiecesList) :-
    print_log('START - loadData...', 3),
    loadPiecesList(PiecesList),
    loadPuzzleShape(PuzzleId, PuzzleShape),
    print_log('END - loadData...', 3).

% Generate a list of the shape and the color of all pieces
% INPUT : 
% OUTPUT : PiecesList
loadPiecesList(PiecesList) :-
    print_log('START - loadPiecesList...', 3),
    findall((PieceColor,PieceShape), (pieces(_, _, PieceColor, PieceShape)), PiecesList),
    print_log('END - loadPiecesList...', 3).

% Load the shape of the puzzle to be solved
% INPUT : PuzzleId
% OUTPUT : PuzzleShape
loadPuzzleShape(PuzzleId, PuzzleShape) :-
    print_log('START - loadPuzzleShape...', 3),
    puzzles(PuzzleId, _, PuzzleShape),
    print_log('END - loadPuzzleShape...', 3).

% Recursive term that try to find all the solution of a puzzle
% using backtracking
% INPUT : ConnHandler, PuzzleShape, PiecesList
% OUTPUT : Solutions
solve_tangram_helper(_, _, [], _).
solve_tangram_helper(ConnHandler, PuzzleShape, PiecesList, [X|Tail]) :-
    print_log('START - solve_tangram_helper...', 2),
    shift_to_origin(ConnHandler, PuzzleShape, ShiftedPuzzleShape),
    select((PieceColor, PieceShape), PiecesList, RemainPiecesList),
    insert_piece(ConnHandler, ShiftedPuzzleShape, PieceShape, ResultPieceShape),
    difference(ConnHandler, ShiftedPuzzleShape, ResultPieceShape, RemainingPuzzleShape),
    yap_predicate_to_WKT(RemainingPuzzleShape, Print),
    print_log(('Log - (RemainingPuzzleShape): ', Print), 1),
    X = (PieceColor, ResultPieceShape),
    solve_tangram_helper(ConnHandler, RemainingPuzzleShape, RemainPiecesList, Tail),
    print_log('END - solve_tangram_helper...', 2).

% Shift the shape of the geometru so that the lower left corner is at the origin
shift_to_origin(ConnHandler, Shape, ShiftedShape) :-
    print_log('START - shift_to_origin...', 3),
    print_log(('Input - Shape: ',Shape), 3),
    st_xmin(ConnHandler, Shape, DeltaX),
    st_ymin(ConnHandler, Shape, DeltaY),
    print_log(('Log - (DeltaX, DeltaY): ', (DeltaX, DeltaY)), 3),
    Dx is -1.0 * DeltaX, 
    Dy is -1.0 * DeltaY,
    print_log(('Log - (Dx, Dy): ', (Dx, Dy)), 3),
    translation(ConnHandler, Shape, (Dx, Dy), ShiftedShape),
    print_log(('Output - ShiftedShape: ', ShiftedShape), 3),
    print_log('END - shift_to_origin...', 3).

% Try to insert the piece in the puzzle
% INPUT : ConnHandler, PuzzleShape, PieceShape 
% OUTPUT : ResultPieceShape
insert_piece(ConnHandler, PuzzleShape, PieceShape, ResultPieceShape) :-
    print_log('START - insert_piece...', 3),
    print_log(('Input - PuzzleShape: ', PuzzleShape), 3),
    print_log(('Input - PieceShape: ', PieceShape), 3),
    shift_to_origin(ConnHandler, PieceShape, SPieceShape),
    can_fit(ConnHandler, PuzzleShape, SPieceShape),
    generate_rotation_translation_list(ConnHandler, PuzzleShape, Combinations),
    find_first(can_insert((ConnHandler, SPieceShape, PuzzleShape)), Combinations, [RotAng|[Tx|Ty]]),
    transform_piece(ConnHandler, SPieceShape, RotAng, (Tx, Ty), ResultPieceShape),
    print_log(('Output - ResultPieceShape: ', ResultPieceShape), 3),
    print_log('END - insert_piece...', 3).