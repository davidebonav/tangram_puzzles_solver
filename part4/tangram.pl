consult(../part2/yap2postgres).
init :-  db_open(localhost, postgres, 'gnazio', tangram_puzzle, ConnName),
            get_value(ConnName, ConnHandler),
            db_import(pieces, pieces, ConnHandler),
            db_import(puzzles, puzzles, ConnHandler),
            db_import(solutions, solutions, ConnHandler).

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

solve_tangram_helper([], _).
solve_tangram_helper([Piece|Rest], Goal, ConnHandler) :-
    pieces(PieceName, PieceColor, PieceCoords),
    rotate_and_translate(PieceCoords, TransformedPieceCoords, ConnHandler),
    member(PieceCoords, TransformedPieceCoords),
    subtract(ConnHandler, Goal, PieceCoords, RemainingGoal),
    solve_tangram_helper(Rest, RemainingGoal).

loadData(Pieces, Puzzle) :- findall((PieceType, Color, PieceShape), pieces(PieceType, Color, PieceShape), Pieces),
                            clause(puzzles(Id, PuzzleNname, PuzzleShape), true),  % find first fact
                            Puzzle = puzzles(Id, PuzzleNname, PuzzleShape).

solve_tangram :- loadData(Pieces, Puzzle, ConnName),
                    get_value(ConnName, ConnHandler),
                    solve_tangram_helper(Pieces, Puzzle, ConnHandler).







