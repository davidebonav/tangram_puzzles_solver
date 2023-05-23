import matplotlib.pyplot as plt
from matplotlib.path import Path
from matplotlib.patches import PathPatch
from matplotlib.collections import PatchCollection
import numpy as np

def register_geometry_type():
    # https://www.psycopg.org/docs/advanced.html#type-casting-of-sql-types-into-python-objects
    from psycopg2.extensions import new_type, register_type  
    from shapely import wkb

    # Define a function to convert the PostgreSQL binary representation to the custom type
    def geometry_converter(value, cur):
        return wkb.loads(value, hex=True) # return MyCustomType(value)
    
    # Register the custom type
    geometry_type_oid = 46115  # Choose a type code that is not used by any built-in PostgreSQL data type
    geometry_type_name = 'geometry'  # The name of the custom type in PostgreSQL
    geometry_typ = new_type((geometry_type_oid,), geometry_type_name, geometry_converter)
    register_type(geometry_typ)
    print("Tipo registrato...")

def print_query_answer(cur, rows = None):
    arity = len(cur.description)

    out_string = "{: <25} "*arity;

    # print column names
    col = [f"(ColName = {desc.name}, Oid = {desc.type_code})" for desc in cur.description]
    print(out_string.format(*col))

    # Process results
    rows = cur.fetchall() if rows is None else rows
    for row in rows:
        str_row = [f"{str(col)}," for col in row] 
        print(out_string.format(*str_row))
        # for i in range(arity):
        #     print(f"{row[i]}\t",end="")
        # print("")

def db_row_to_patch(points, color='gray', id=None,alpha=0.9):
        alpha = 0.5+id/14 if alpha==None else alpha
        return plt.Polygon(
            points, 
            facecolor=color, 
            edgecolor='black', 
            alpha=alpha
        )

# def reshape_array(arr, nrow, ncol): # array_to_matrix
#     # Split the array into a matrix
#     matrix = [[arr[i*ncol + j] for j in range(ncol)] for i in range(nrow)]
#     return matrix

def plot_polygon(ax, poly, **kwargs):
    path = Path.make_compound_path(
           Path(np.asarray(poly.exterior.coords)[:, :2]),
           *[
                Path(np.asarray(ring.coords)[:, :2]) 
                for ring in poly.interiors
            ])
 
    patch = PathPatch(path, **kwargs)
    collection = PatchCollection([patch], **kwargs)
    
    ax.add_collection(collection, autolim=True)
    ax.autoscale_view()
    return collection