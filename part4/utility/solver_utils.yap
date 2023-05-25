% :- consult(utils).
:- consult(st_utils).
% :- load_foreign_files(['plot_predicate'], [], init_predicates).

% Generate all the rotation angles that can be used
generate_rotation_angles_list(OutputList) :-
    print_log('START - generate_rotation_angles_list...', 3),
    findall(Degree, (between(0, 7, X), Degree is X * 45), OutputList),
    print_log('END - generate_rotation_angles_list...', 3).

% Generate a list of all number N : 0 <= N <= Number and N is multiple of 0.25 
generate_multiples(Number, List) :-
    print_log('START - generate_multiples...', 3),
    UpperBound is floor(Number * 4),
    findall(Value, (between(0, UpperBound, X), Value is X / 4), List),
    print_log('END - generate_multiples...',3).

% Generate a list containing a list with all possible translation vector that can be used
generate_translation_vector_list(MaxX, MaxY, OutputList) :-
    print_log('START - generate_translation_vector_list...',3),
    generate_multiples(MaxX, XList),
    generate_multiples(MaxY, YList),
    print_log(('(LogXList, YList): ', (XList, YList)), 3),
    generate_combinations(XList, YList, OutputList),
    print_log(('Log - (OutputList): ', OutputList), 3),
    print_log('END - generate_translation_vector_list...',3).

% generate the list of all possibile combination beteen all possible rotation and all possible combination
generate_rotation_translation_list(PuzzleShape, PieceShape, Combinations) :-
    print_log('START - generate_rotation_translation_list...',3),
    flatten_list(PuzzleShape, FlatPuzzle),
    flatten_list(PieceShape, FlatPiece),
    generate_combinations(FlatPuzzle, FlatPiece, CombinationPoints),
    print_log(('Log - CombinationPoints: ', CombinationPoints), 3),
    % transform_list(CombinationPoints, point_to_translation_vector, TranslationList),
    TranslationList = CombinationPoints,
    generate_rotation_angles_list(RotationList),
    generate_combinations(RotationList, TranslationList, Combinations),
    print_log(('Output - Combinations: ', Combinations), 3),
    print_log('END - generate_rotation_translation_list...',3).

point_to_translation_vector([[X1|Y1]|[X2|Y2]], [Tx|Ty]) :-
    print_log('START - point_to_translation_vector...',3),
    Tx is X1 - X2,
    Ty is Y1 - Y2,
    print_log(('Log - translation (Tx,Ty): ', (Tx, Ty)), 3),
    print_log('END - point_to_translation_vector...',3).

% Apply the transformation to the input piece
transform_piece(ConnHandler, Shape, [RotAng|[[X1|Y1]|[X2|Y2]]], ResultShape) :-
    print_log('START - transform_piece...',3),
    print_log(('Log - translation (X1,Y1): ', (X1,Y1)), 3),
    print_log(('Log - translation (X2,Y2): ', (X2,Y2)), 3),
    print_log(('Log - translation RotAng: ', RotAng), 3),
    point_to_translation_vector([[X1|Y1]|[X2|Y2]], [Tx|Ty]),
    translation(ConnHandler, Shape, (Tx, Ty), TraslShape),
    rotation(ConnHandler, TraslShape, RotAng, (X1, Y1), ResultShape),
    print_log('END - transform_piece...',3).

% Check if a piece can be inserted in the puzzle or not
can_insert((ConnHandler, Piece, Puzzle), Elem) :-
    print_log('START - can_insert...',3),
    print_log(('(Elem): ', Elem), 3),
    transform_piece(ConnHandler, Piece, Elem, TPiece),
    yap_predicate_to_WKT(Puzzle, WKTPuzzle), print_log(('Log - WKTPuzzle: ', WKTPuzzle), 3),
    yap_predicate_to_WKT(Piece, WKTPiece), print_log(('Log - WKTPiece: ', WKTPiece), 3),
    yap_predicate_to_WKT(TPiece, WKTTPiece), print_log(('Log - WKTTPiece: ', WKTTPiece), 2),
    st_contain(ConnHandler, Puzzle, TPiece, Output),
    Output = true,
    print_log('END - can_insert...',3).

% check the area of the pieces and of the puzzle and understand if the pieces can be added or not
can_fit(ConnHandler, PuzzleShape, PieceShape) :-
    print_log('START - can_fit...',3),
    geometry_to_string(PuzzleShape, PuzzleString),
    geometry_to_string(PieceShape, PieceString),
    st_query_list_helper('ST_Area', [PuzzleString], PuzzleArea),
    st_query_list_helper('ST_Area', [PieceString], PieceArea),
    concatenate(PuzzleArea, [' > '], Tmp1),
    concatenate(Tmp1, PieceArea, SQLList),
    print_log(('Log - SQLList: ', SQLList), 3),
    st_query(ConnHandler, SQLList, Tmp2),
    string_to_boolean(Tmp2, Output),
    print_log(('Log - Output: ', Output), 3),
    Output = true,
    print_log('END - can_fit...',3).