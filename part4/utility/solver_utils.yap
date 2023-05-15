% :- consult(utils).
:- consult(st_utils).

% Generate all the rotation angles that can be used
generate_rotation_angles_list(OutputList) :-
    write('START - generate_rotation_angles_list...\n'),
    findall(Degree, (between(0, 7, X), Degree is X * 45), OutputList),
    write('END - generate_rotation_angles_list...\n').

% Generate a list of all number N : 0 <= N <= Number and N is multiple of 0.25 
generate_multiples(Number, List) :-
    write('START - generate_multiples...\n'),
    UpperBound is floor(Number * 4),
    findall(Value, (between(0, UpperBound, X), Value is X / 4), List),
    write('END - generate_multiples...\n').

% Generate a list containing a list with all possible translation vector that can be used
generate_translation_vector_list(MaxX, MaxY, OutputList) :-
    write('START - generate_translation_vector_list...\n'),
    generate_multiples(MaxX, XList),
    generate_multiples(MaxY, YList),
    generate_combinations(XList, YList, OutputList),
    write('END - generate_translation_vector_list...\n').

% generate the list of all possibile combination beteen all possible rotation and all possible combination
generate_rotation_translation_list(PuzzleShape, Combinations) :-
    write('START - generate_rotation_translation_list...\n'),
    generate_rotation_angles_list(RotationList),
    st_xmax(ConnHandler, PuzzleShape, MaxX),
    st_ymax(ConnHandler, PuzzleShape, MaxY),
    generate_translation_vector_list(MaxX, MaxY, TranslationList),
    generate_combinations(RotationList, TranslationList, Combinations),
    write('END - generate_rotation_translation_list...\n').

% Apply the transformation to the input piece
transform_piece(ConnHandler, Shape, RotAng, (Tx,Ty), ResultShape) :-
    write('START - transform_piece...\n'),
    rotation(ConnHandler, Shape, RotAng, RotatedShape),
    translation(ConnHandler, RotatedShape, (Tx, Ty), ResultShape),
    write('END - transform_piece...\n').

% Check if a piece can be inserted in the puzzle or not
can_insert((ConnHandler, Piece,Puzzle), [RotAng|[Tx|Ty]]) :-
    write('START - can_insert...\n'),
    transform_piece(ConnHandler, Piece, RotAng, (Tx,Ty), TPiece),
    st_contain(ConnHandler, TPiece, Puzzle, Output),
    Output = true,
    write('END - can_insert...\n').

% check the area of the pieces and of the puzzle and understand if the pieces can be added or not
can_fit(ConnHandler, PuzzleShape, PieceShape) :-
    write('START - can_fit...\n'),
    geometry_to_string(PuzzleShape, PuzzleString),
    geometry_to_string(PieceShape, PieceString),
    st_query_list_helper('ST_Area', [PuzzleString], PuzzleArea),
    st_query_list_helper('ST_Area', [PieceString], PieceArea),
    append(PuzzleArea, ['>'], Tmp1),
    append(Tmp1, PieceArea, SQLList),
    st_query(ConnHandler, SQLList, Tmp2),
    string_to_boolean(Tmp2, Output),
    Output = true,
    write('END - can_fit...\n').