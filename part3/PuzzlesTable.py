#!/usr/bin/python3

from utils import *
import numpy
import matplotlib.pyplot as plt
from shapely.affinity import rotate, translate, scale

class PuzzlesTable:

    # PIECES TABLE COLUMNS NUMBER
    ID = 0
    PUZZLE_NAME = 1
    SHAPE = 2

    # SQL query
    select_star = "SELECT * FROM puzzles"

    def __init__(self, conn):
        self.conn = conn

        cur = PuzzlesTable.get_records_cur(self.conn)
        self.rows = cur.fetchall()
        cur.close()
    
    @staticmethod
    def get_records_cur(conn):
        cur = conn.cursor()
        cur.execute(PuzzlesTable.select_star)
        return cur
    
    @staticmethod
    def print_puzzles(conn):
        cur = PuzzlesTable.get_records_cur(conn)
        print_query_answer(cur)
        cur.close()

    @staticmethod
    def plot_puzzles(conn):
        cur = PuzzlesTable.get_records_cur(conn)
        rows = cur.fetchall()
        cur.close()
        ncols=4
        nrows=len(rows)//4+1

        plt.rcParams["figure.autolayout"] = True

        fix, axes = plt.subplots(nrows=nrows, ncols=ncols)
        if isinstance(axes, numpy.ndarray):
            axes = [axes,[]]

        for i in range(nrows):
            for j in range(ncols):
                if (i*ncols + j) >= len(rows):
                    break
                row = rows[i*ncols + j]

                patch = db_row_to_patch(
                    points=list(row[PuzzlesTable.SHAPE].exterior.coords))

                axes[i][j].clear()
                axes[i][j].set_title(f"id={row[PuzzlesTable.ID]}, name={row[PuzzlesTable.PUZZLE_NAME]}")
                axes[i][j].add_patch(patch)
                # axes[i][j].autoscale_view()
            
                axes[i][j].set_xlim(-0.3, 1.3)
                axes[i][j].set_ylim(-0.1, 1.3)
                axes[i][j].set_aspect("equal")

                plt.pause(0.01)
            else:
                continue  # Continue the outer loop if the inner loop didn't break
            break  # Break out of the outer loop if the inner loop did break
        plt.tight_layout()
        plt.show()
    
    def solve_puzzle(self):
        self.count=0

        puzzle = {
            'print' : [False for row in self.rows],
            'colors' : [row[PuzzlesTable.COLOR] for row in self.rows],
            'polygons' : [row[PuzzlesTable.SHAPE] for row in self.rows],
            'patches' : [
                db_row_to_patch(
                        list(row[PuzzlesTable.SHAPE].exterior.coords), 
                        row[PuzzlesTable.COLOR],
                        id=row[PuzzlesTable.ID])
                for row in self.rows
            ]
        }

        fig, ax = plt.subplots()
        ax.clear()
        ax.set_title(f"Mosse fatte {self.count}")
        self.add_all_patch(ax, puzzle)
        ax.autoscale_view()

        # Rotate
        button_rotate_ax = fig.add_axes([0.2, 0.2, 0.1, 0.05])
        rotate_button = plt.Button(button_rotate_ax, "Rotate")
        rotate_button.on_clicked(lambda event, ax=ax: self.rotate_plot(fig,ax,puzzle))
        # Translate
        button_translate_ax = fig.add_axes([0.3, 0.2, 0.1, 0.05])
        translate_button = plt.Button(button_translate_ax, "Translate")
        translate_button.on_clicked(lambda event, ax=ax: self.translate_plot(fig,ax,puzzle))
        # Specular x
        button_specX_ax = fig.add_axes([0.4, 0.2, 0.1, 0.05])
        specX_button = plt.Button(button_specX_ax, "Specular X")
        specX_button.on_clicked(lambda event, ax=ax: self.specX_plot(fig,ax,puzzle))
        # Specular y
        button_sepcY_ax = fig.add_axes([0.5, 0.2, 0.1, 0.05])
        specY_button = plt.Button(button_sepcY_ax, "Specular Y")
        specY_button.on_clicked(lambda event, ax=ax: self.specY_plot(fig,ax,puzzle))
        # Add piece
        button_add_ax = fig.add_axes([0.6, 0.2, 0.1, 0.05])
        add_button = plt.Button(button_add_ax, "Add piece")
        add_button.on_clicked(lambda event, ax=ax: self.add_plot(fig,ax,puzzle))
        # Print All
        button_print_ax = fig.add_axes([0.7, 0.2, 0.1, 0.05])
        print_button = plt.Button(button_print_ax, "Print")
        print_button.on_clicked(lambda event, ax=ax: self.print_plot(puzzle))
                
        plt.show()

    def add_all_patch(self, ax, puzzle):

        for i in range(len(puzzle['patches'])):
            if puzzle['print'][i] == True:
                ax.add_patch(puzzle['patches'][i])

    def re_draw(self, fig, ax, puzzle):
        self.count+=1

        ax.clear()
        self.add_all_patch(ax, puzzle)
        ax.set_title(f"Mosse fatte {self.count}")
        ax.autoscale_view()
        # Redraw the plot
        fig.canvas.draw_idle()

    def rotate_plot(self,fig,ax, puzzle):
        piece_id = int(input("Select the id of the piece: "))
        rotation_angle = float(input("Enter the angle of rotation: "))

        rotate_polygon = rotate(puzzle['polygons'][piece_id], rotation_angle)
        puzzle['polygons'][piece_id] = rotate_polygon

        puzzle['patches'][piece_id] =  db_row_to_patch(
                        list(rotate_polygon.exterior.coords), 
                        puzzle['colors'][piece_id],
                        id=piece_id)
        
        self.re_draw(fig, ax, puzzle)


    def translate_plot(self,fig,ax, puzzle):
        piece_id = int(input("Select the id of the piece: "))
        x_vector = float(input("Enter the x coordinate of the translation vector: "))
        y_vector = float(input("Enter the y coordinate of the translation vector: "))

        translate_poligon = translate(puzzle['polygons'][piece_id], xoff=x_vector, yoff=y_vector)
        puzzle['polygons'][piece_id] = translate_poligon

        puzzle['patches'][piece_id] =  db_row_to_patch(
                        list(translate_poligon.exterior.coords), 
                        puzzle['colors'][piece_id],
                        id=piece_id)
        
        self.re_draw(fig, ax, puzzle)


    def specX_plot(self,fig,ax,puzzle):
        piece_id = int(input("Select the id of the piece: "))

        poly = puzzle['polygons'][piece_id]
        specular = scale(poly, xfact=-1, origin=(0,0))
        specular = translate(specular, poly.bounds[2], 0)
        
        puzzle['polygons'][piece_id] = specular
        puzzle['patches'][piece_id] =  db_row_to_patch(
                        list(specular.exterior.coords), 
                        puzzle['colors'][piece_id],
                        id=piece_id)
        
        self.re_draw(fig, ax, puzzle)

    def specY_plot(self,fig,ax,puzzle):
        piece_id = int(input("Select the id of the piece: "))

        poly = puzzle['polygons'][piece_id]
        specular = scale(poly, yfact=-1, origin=(0,0))
        specular = translate(specular, 0, poly.bounds[2])
        
        puzzle['polygons'][piece_id] = specular
        puzzle['patches'][piece_id] =  db_row_to_patch(
                        list(specular.exterior.coords), 
                        puzzle['colors'][piece_id],
                        id=piece_id)
        
        self.re_draw(fig, ax, puzzle)

    def add_plot(self,fig,ax,puzzle):
        piece_id = int(input("Select the id of the piece: "))
        puzzle['print'][piece_id] = True        
        self.re_draw(fig, ax, puzzle)

    def print_plot(self,puzzle):
        for p in puzzle['polygons']:
            print(p.wkt)

