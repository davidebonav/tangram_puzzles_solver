#!/usr/bin/python3

import psycopg2
from utils import register_geometry_type
from PiecesTable import *
from PuzzlesTable import *
from SolutionsTable import *

def main():
    # Establish a connection to the database
    conn = psycopg2.connect(
        host="localhost",
        database="tangram_puzzle",
        user="davide",
        password="davide"
    )

    # print("Pieces table: ")
    # PiecesTable.print_table(conn)
    # print("\n\nPuzzles table:")
    # PuzzlesTable.print_table(conn)
    # print("\n\nSolutions table:")
    # SolutionsTable.print_table(conn)

    pieces = PiecesTable(conn)
    pieces.plot_table()
    puzzles = PuzzlesTable(conn)
    puzzles.plot_table()
    solutions = SolutionsTable(conn)
    solutions.plot_table()

    # puzzle = PiecesTable(conn)
    # puzzle.solve_puzzle()

    # Close the connection
    conn.close()

if __name__ == "__main__":
    register_geometry_type()
    main()