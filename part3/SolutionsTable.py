#!/usr/bin/python3

from Table import Table
from utils import plot_polygon
from shapely import wkb

class SolutionsTable(Table):

    # PIECES TABLE COLUMNS NUMBER
    PUZZLE_ID = 0

    def __init__(self, conn):
        super().__init__(conn)

        # create the list of all colors
        cur = self.conn.cursor()
        cur.execute('SELECT color FROM pieces ORDER BY id;')
        rows = cur.fetchall()
        cur.close()

        self.colors = [row[0] for row in rows]

    # SQL query
    def select_star(self):
        cur = self.conn.cursor()
        # pieces number
        cur.execute('SELECT COUNT(DISTINCT id) FROM pieces')
        pieces_number = cur.fetchall()[0][0]
        # solutions number
        cur.execute('SELECT COUNT(DISTINCT puzzle_id) FROM solutions')
        puzzles_number = cur.fetchall()[0][0]
        cur.close()

        sql_query =f""
        for puzzle in range(1, puzzles_number+1):
            sql_query += f"SELECT {puzzle} AS puzzle"

            for piece in range(1,pieces_number+1):
                sql_query += f", MAX(CASE WHEN piece_id={piece} THEN piece_coordinates ELSE NULL END)::GEOMETRY AS piece{piece}"
            
            sql_query += f" FROM solutions WHERE puzzle_id={puzzle}"
            if puzzle!=puzzles_number:
                sql_query += " UNION "
        return sql_query 

    @staticmethod
    def set_ax_title(ax,row):
        ax.set_title(f"puzzle {row[SolutionsTable.PUZZLE_ID]} solution")

    @staticmethod
    def set_ax_xylim(ax):
        ax.set_xlim(-0.3, 1.3)
        ax.set_ylim(-0.1, 1.3)
    

    def set_ax_patches(self,ax,row):
        for i in range(1,len(row)):
            row2 = [wkb.loads(row[idx], hex=True) if idx != 0 else row[idx] for idx, val in enumerate(row)]
            plot_polygon(ax, row2[i], 
                color=self.colors[i-1],
                alpha=0.9
            )
