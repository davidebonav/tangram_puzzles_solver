#!/usr/bin/python3

from utils import db_row_to_patch, plot_polygon
from SolutionsTable import SolutionsTable

import matplotlib.pyplot as plt

class PuzzlesAndSolution:
    def __init__(self, conn):
        self.conn = conn

        self.get_puzzles()
        self.get_solutions()

        cur = self.conn.cursor()
        cur.execute('SELECT color FROM pieces ORDER BY id;')
        rows = cur.fetchall()
        cur.close()
        self.colors = [row[0] for row in rows]

    def get_puzzles(self):
        sql = "SELECT * FROM puzzles ORDER BY id DESC"
        cur = self.conn.cursor()
        cur.execute(sql)
        self.puzzles = cur.fetchall()
        cur.close()

    def get_solutions(self):
        tmp = SolutionsTable(self.conn)
        sql = tmp.select_star()
        cur = self.conn.cursor()
        cur.execute(sql)
        self.solutions = cur.fetchall()
        cur.close()

    def plot_table(self):

        ncols=len(self.puzzles)
        nrows=2

        plt.rcParams["figure.autolayout"] = True

        fix, axes = plt.subplots(nrows=nrows, ncols=ncols)
        # if not self.is_matrix(axes):
        #     axes = [axes,[]]

        fix.set_size_inches(15, 8)

        # print all puzzle
        for i in range(ncols):
            puzzle = self.puzzles[i]
            ax = axes[0][i]

            ax.clear()
            ax.set_title(f"id={puzzle[0]}, name={puzzle[1]}")
            plot_polygon(ax, puzzle[2], 
                         color='gray',
                         alpha=0.9
                         )

            ax.set_xlim(-0.3, 1.3)
            ax.set_ylim(-0.1, 1.3)
            ax.set_aspect("equal")

            plt.pause(0.005)
        
        # print all solution
        for i in range(ncols):
            solution = self.solutions[i]
            ax = axes[1][i]

            ax.clear()
            ax.set_title(f"puzzle {solution[0]} solution")
            for k in range(1,len(solution)):
                plot_polygon(ax, solution[k], 
                         color=self.colors[k-1],
                         alpha=0.9
                         )
            ax.set_xlim(-0.3, 1.3)
            ax.set_ylim(-0.1, 1.3)
            ax.set_aspect("equal")

            plt.pause(0.005)

        plt.tight_layout()
        plt.show()