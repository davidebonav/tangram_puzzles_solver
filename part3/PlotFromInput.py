#!/usr/bin/python3

from matplotlib.patches import Polygon
from utils import plot_polygon
from shapely.wkt import loads
import matplotlib.pyplot as plt

class PlotFromInput:
    @staticmethod
    def plot_polygon(wkt_string):
        # Load the WKT string into a Shapely geometry
        # geometry = Polygon() # if wkt_string.startswith('POLYGON') else MultiPolygon()
        # geometry = geometry.__class__(wkt_string)
        geometry = loads(wkt_string)

        plt.rcParams["figure.autolayout"] = True

        fix, ax = plt.subplots()
        fix.set_size_inches(15, 8)

        ax.clear()
        ax.set_title(f"Plot of input geometry")

        if isinstance(geometry, Polygon):
            plot_polygon(ax, geometry, 
                color='gray',
                alpha=0.9
            )
        else:
            for polygon in geometry.geoms:
                plot_polygon(ax, polygon, 
                    color='gray',
                    alpha=0.9
                )

        ax.set_xlim(0, 1.75)
        ax.set_ylim(0, 1.75)
        ax.set_aspect("equal")

        plt.show()