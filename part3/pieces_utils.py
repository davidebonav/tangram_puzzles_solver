#!/usr/bin/python3

# import psycopg2
from utils import print_query_answer
import matplotlib.pyplot as plt
# import shapely

def get_all_pieces(conn):
    # Create a cursor to execute queries
    cur = conn.cursor()
    # Execute a query
    cur.execute("SELECT * FROM pieces")
    return cur

def print_pieces_table(conn):
    cur = get_all_pieces(conn)
    print_query_answer(cur)

    # Close the cursor
    cur.close()

'''
def plot_pieces_table(conn):
    cur = get_all_pieces(conn)
    rows = cur.fetchall()
    row_num = len(rows)

    # Create subplots
    # fig, axs = plt.subplots(nrows=4, ncols=row_num//4+1)
    # axs[i,j]
    fig, axs = plt.subplots(row_num)

    i = 0
    for ax in axs:
        # print(rows[i])
        patch = plt.Polygon(list(rows[i][3].exterior.coords), facecolor=rows[i][2], edgecolor='black', alpha=0.9)
        ax.add_patch(patch)
        ax.autoscale_view()
        ax.set_title(f"id={rows[i][0]}, name={rows[i][1]}")
        i+=1
    plt.tight_layout()  # Adjust spacing between subplots
    plt.show()
'''

def plot_pieces_table(conn):
    cur = get_all_pieces(conn)
    rows = cur.fetchall()

    
    for row in rows:
        fig, ax = plt.subplots()
        patch = plt.Polygon(list(row[3].exterior.coords), facecolor=row[2], edgecolor='black', alpha=0.9)
        ax.add_patch(patch)
        ax.autoscale_view()
        ax.set_title(f"id={row[0]}, name={row[1]}")
        plt.tight_layout()  # Adjust spacing between subplots
        plt.show()
    

def tmp():
    from shapely.geometry import Polygon    

    # Create a Shapely Polygon
    polygon = Polygon([(0, 0), (1, 0), (1, 1), (7, 1)])

    # Convert the Polygon to a set of x and y coordinates
    x, y = polygon.exterior.xy

    # Plot the Polygon using Matplotlib
    plt.plot(x, y)

    # Show the plot
    plt.show()

def tmp2():
    import matplotlib.pyplot as plt
    import numpy as np
    from matplotlib.animation import FuncAnimation
    from shapely.geometry import Point

    # Create an empty scatter plot
    fig, ax = plt.subplots()
    scat = ax.scatter([], [])

    # Define a function to update the scatter plot with new data
    def update(frame):
        # Generate new data
        x = np.random.rand(50)
        y = np.random.rand(50)
        points = [Point(xy) for xy in zip(x, y)]

        # Update the scatter plot
        scat.set_offsets([(p.x, p.y) for p in points])

        # Return the updated scatter plot
        return scat,

    # Create an animation object that repeatedly calls the update function every 100 milliseconds
    anim = FuncAnimation(fig, update, interval=100)

    # Display the animation
    plt.show()

def tmp3():
    import matplotlib.pyplot as plt

    # Define the x and y values and colors
    x = [1, 2, 3, 4, 5]
    y = [10, 5, 8, 3, 6]
    colors = ['r', 'g', 'b', 'm', 'c']  # red, green, blue, magenta, cyan

    # Plot the points with colors
    plt.scatter(x, y, c=colors)

    # Set the title and labels
    plt.title('Scatter plot with colors')
    plt.xlabel('X axis')
    plt.ylabel('Y axis')

    # Show the plot
    plt.show()

def tmp4():
    import matplotlib.pyplot as plt
    from shapely.geometry import Polygon

    poligono = Polygon([(0, 0), (1, 1), (2, 0), (1, -1)])

    fig, ax = plt.subplots()
    ax.add_patch(plt.Polygon(list(poligono.exterior.coords), facecolor='red', edgecolor='black', alpha=0.9))

    ax.autoscale_view()

#     # Set the limits of the plot
#     ax.set_xlim(-0.5, 1.5)
#     ax.set_ylim(-0.5, 1.5)

    plt.show()

def tmp5():
    import matplotlib.pyplot as plt
    import numpy as np

    plt.ion()
    for i in range(50):
        y = np.random.random([10,1])
        plt.plot(y)
        plt.draw()
        plt.pause(0.0001)
        plt.clf()
    return 
    import matplotlib.pyplot as plt
    import numpy as np

    fig, ax = plt.subplots()
    x = np.linspace(0, 2*np.pi, 100)
    y = np.sin(x)
    line, = ax.plot(x, y)

    while True:
        input("Press enter to update plot")
        y = np.roll(y, 1)
        line.set_ydata(y)
        plt.draw()

