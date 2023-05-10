consult(../part2/yap2postgres).

init(ConnName) :-  
        db_open('192.168.175.132', davide, 'davide', tangram_puzzle, ConnName),
        db_import(pieces, pieces, ConnName),
        db_import(puzzles, puzzles, ConnName),
        db_import(solutions, solutions, ConnName).
 
solve_tangram(PuzzleId, Solution, ConnName) :- 
        get_value(ConnName, ConnHandler),
        loadData(PuzzleId, PuzzleShape, PiecesList),
        solve_tangram_helper(PiecesList, PuzzleShape, ConnHandler).

loadData(PuzzleId, PuzzleShape, PiecesList) :- 
        findall(PieceShape,(pieces(_, _, _, PieceShape)), PiecesList),
        puzzles(PuzzleId, _, PuzzleShape).

solve_tangram_helper([], _, ConnHandler).
solve_tangram_helper([Piece|Rest], Goal, ConnHandler) :-
    pieces(_, PieceName, PieceColor, PieceCoords),
    rotate_and_translate(PieceCoords, TransformedPieceCoords, ConnHandler),
    member(PieceCoords, TransformedPieceCoords),
    subtract(ConnHandler, Goal, PieceCoords, RemainingGoal),
    solve_tangram_helper(Rest, RemainingGoal,ConnHandler).

% Predicato per ruotare le coordinate del pezzo
rotate_piece(PieceCoords, RotatedPieceCoords, ConnHandler) :-
    findall(RotatedCoords, (
        member(Coords, PieceCoords),
        st_rotate(ConnHandler, Coords, 45, RotatedCoords)
    ), RotatedPieceCoords).

translate_piece(PieceCoords, TransformedPieceCoords, ConnHandler) :-
    findall(TranslatedCoords, (
        member(Coords, PieceCoords),
        st_translate(ConnHandler, Coords, 0.25, 0.25, TranslatedCoords)
    ), TransformedPieceCoords).
            
rotate_and_translate(PieceCoords, TransformedPieceCoords, ConnHandler) :-
    rotate_piece(PieceCoords, RotatedPieceCoords, ConnHandler),
    translate_piece(RotatedPieceCoords, TransformedPieceCoords, ConnHandler).