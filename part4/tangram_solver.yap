:- consult('./utility/solver_utils').

% Main term of the solver. Try to solve with backtracking 
% the puzzle with id PuzzleId and put all the founded solution
% in Solutions 
% INPUT : ConnName, PuzzleId 
% OUTPUT : Solutions
tangram_solver(ConnName, PuzzleId, Solutions) :- 
    write('START - tangram_solver...\n'),
    get_value(ConnName, ConnHandler),
    loadData(PuzzleId, PuzzleShape, PiecesList),
    % write(('PIECES = \n', PiecesList, '\n')),
    % write(('PUZZLE = \n', PuzzleId, PuzzleShape, '\n')),
    solve_tangram_helper(ConnHandler, PuzzleShape, PiecesList, Solutions),
    write('END - tangram_solver...\n').

% Load all the needed data
% INPUT : PuzzleId
% OUTPUT : PuzzleShape, PiecesList
loadData(PuzzleId, PuzzleShape, PiecesList) :-
    write('START - loadData...\n'),
    loadPiecesList(PiecesList),
    loadPuzzleShape(PuzzleId, PuzzleShape),
    write('END - loadData...\n').

% Generate a list of the shape and the color of all pieces
% INPUT : 
% OUTPUT : PiecesList
loadPiecesList(PiecesList) :-
    write('START - loadPiecesList...\n'),
    findall((PieceColor,PieceShape), (pieces(_, _, PieceColor, PieceShape)), PiecesList),
    write('END - loadPiecesList...\n').

% Load the shape of the puzzle to be solved
% INPUT : PuzzleId
% OUTPUT : PuzzleShape
loadPuzzleShape(PuzzleId, PuzzleShape) :-
    write('START - loadPuzzleShape...\n'),
    puzzles(PuzzleId, _, PuzzleShape),
    write('END - loadPuzzleShape...\n').

% Recursive term that try to find all the solution of a puzzle
% using backtracking
% INPUT : ConnHandler, PuzzleShape, PiecesList
% OUTPUT : Solutions
solve_tangram_helper(_, _, [], _).
solve_tangram_helper(ConnHandler, PuzzleShape, PiecesList, [X|Tail]) :-
    write('START - solve_tangram_helper...\n'),
    shift_to_origin(ConnHandler, PuzzleShape, ShiftedPuzzleShape),
    write(('Shift puzzle', PuzzleShape, ShiftedPuzzleShape, '\n')),
    select((PieceColor, PieceShape), PiecesList, RemainPiecesList),
    insert_piece(ConnHandler, ShiftedPuzzleShape, PieceShape, ResultPieceShape),
    difference(ConnHandler, ShiftedPuzzleShape, ResultPieceShape, RemainingPuzzleShape),
    X = (PieceColor, ResultPieceShape),
    solve_tangram_helper(ConnHandler, RemainingPuzzleShape, RemainPiecesList, Tail),
    write('END - solve_tangram_helper...\n').

% Shift the shape of the geometru so that the lower left corner is at the origin
shift_to_origin(ConnHandler, Shape, ShiftedShape) :-
    write('START - shift_to_origin...\n'),
    st_xmin(ConnHandler, Shape, DeltaX),
    st_ymin(ConnHandler, Shape, DeltaY),
    st_translate(ConnHandler, Shape, [-DeltaX|-DeltaY], ShiftedShape),
    write('END - shift_to_origin...\n').

% Try to insert the piece in the puzzle
% INPUT : ConnHandler, PuzzleShape, PieceShape 
% OUTPUT : ResultPieceShape
insert_piece(ConnHandler, PuzzleShape, PieceShape, ResultPieceShape) :-
    write('START - insert_piece...\n'),
    shift_to_origin(ConnHandler, PieceShape, SPieceShape),
    can_fit(ConnHandler, PuzzleShape, SPieceShape),
    generate_rotation_translation_list(PuzzleShape, Combinations),
    find_first(can_insert((ConnHandler, SPieceShape, PuzzleShape)), Combinations, [RotAng|[Tx|Ty]]),
    transform_piece(ConnHandler, SPieceShape, RotAng, (Tx, Ty), ResultPieceShape),
    write('END - insert_piece...\n').