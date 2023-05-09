#!/usr/bin/python3

from utils import *
import numpy
import matplotlib.pyplot as plt
import abc

class Table(metaclass=abc.ABCMeta):
    def __init__(self, conn):
        self.conn = conn

        cur = self.get_records_cur()
        self.rows = cur.fetchall()
        cur.close()
    
    def get_records_cur(self):
        cur = self.conn.cursor()
        cur.execute(self.select_star())
        return cur
    
    def print_table(self):
        cur = self.get_records_cur()
        print_query_answer(cur)
        cur.close()

    def is_matrix(self,x):
        if not isinstance(x, numpy.ndarray):  # not a list
            return False
        if len(x) == 0 or not isinstance(x[0], numpy.ndarray):  # not a nested list
            return False
        return True
        # num_cols = len(x[0])
        # return all(len(row) == num_cols for row in x)  # all rows have the same length

    def plot_table(self):
        cur = self.get_records_cur()
        rows = cur.fetchall()
        cur.close()

        ncols=4
        nrows=len(rows)//4+1

        plt.rcParams["figure.autolayout"] = True

        fix, axes = plt.subplots(nrows=nrows, ncols=ncols)
        if not self.is_matrix(axes):
            axes = [axes,[]]

        for i in range(nrows):
            for j in range(ncols):
                if (i*ncols + j) >= len(rows):
                    break # Break out of the inner loop
                row = rows[i*ncols + j]

                axes[i][j].clear()
                self.set_ax_title(axes[i][j],row)
                self.set_ax_patches(axes[i][j],row)
                # axes[i][j].autoscale_view()
                self.set_ax_xylim(axes[i][j])
                axes[i][j].set_aspect("equal")

                plt.pause(0.01)
            else:
                continue  # Continue the outer loop if the inner loop didn't break
            break  # Break out of the outer loop if the inner loop did break
        plt.tight_layout()
        plt.show()

    @staticmethod
    def db_row_to_patch(points, color='gray', id=None,alpha=0.9):
        alpha = 0.5+id/14 if alpha==None else alpha
        return plt.Polygon(
            points, 
            facecolor=color, 
            edgecolor='black', 
            alpha=alpha
        )

    @staticmethod
    @abc.abstractmethod
    def set_ax_title(ax,row):
        pass

    @abc.abstractmethod
    def set_ax_patches(self,ax,row):
        pass

    @staticmethod
    @abc.abstractmethod
    def set_ax_xylim(ax):
        pass

    # SQL query
    @abc.abstractmethod
    def select_star():
        pass