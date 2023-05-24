#!/usr/bin/python3

from Table import Table
from utils import plot_polygon

class PuzzlesTable(Table):

    # PIECES TABLE COLUMNS NUMBER
    ID = 0
    PUZZLE_NAME = 1
    SHAPE = 2

    def __init__(self, conn):
        super().__init__(conn)
    
    # SQL query
    def select_star(self):
        return "SELECT * FROM puzzles"

    @staticmethod
    def set_ax_title(ax,row):
        ax.set_title(f"id={row[PuzzlesTable.ID]}, name={row[PuzzlesTable.PUZZLE_NAME]}")

    def set_ax_patches(self,ax,row):
        plot_polygon(ax, row[PuzzlesTable.SHAPE], 
            color='gray',
            alpha=0.9)

    @staticmethod
    def set_ax_xylim(ax):
        ax.set_xlim(-0.3, 1.3)
        ax.set_ylim(-0.1, 1.3)