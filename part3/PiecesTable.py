#!/usr/bin/python3

from Table import Table

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
        patch = Table.db_row_to_patch(
                    list(row[PiecesTable.SHAPE].exterior.coords), 
                    row[PiecesTable.COLOR],
                    alpha=0.9)
        ax.add_patch(patch)

    @staticmethod
    def set_ax_xylim(ax):
        ax.set_xlim(0, 1)
        ax.set_ylim(-0.25, 1)