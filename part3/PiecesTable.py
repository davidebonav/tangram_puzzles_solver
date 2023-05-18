#!/usr/bin/python3

from Table import Table
from utils import plot_polygon

class PiecesTable(Table):

    # PIECES TABLE COLUMNS NUMBER
    ID = 0
    PIECE_TYPE = 1
    COLOR = 2
    SHAPE = 3
    
    def __init__(self, conn):
        super().__init__(conn)

    # SQL query
    def select_star(self):
        return "SELECT * FROM pieces"
    
    @staticmethod
    def set_ax_title(ax,row):
        ax.set_title(f"id={row[PiecesTable.ID]}, name={row[PiecesTable.PIECE_TYPE]}")

    def set_ax_patches(self,ax,row):
        plot_polygon(ax, row[PiecesTable.SHAPE], 
            color=row[PiecesTable.COLOR],
            alpha=0.9)

    @staticmethod
    def set_ax_xylim(ax):
        ax.set_xlim(0, 1)
        ax.set_ylim(-0.25, 1)