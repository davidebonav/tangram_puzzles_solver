#!/usr/bin/python3

import psycopg2
from utils import register_geometry_type
from PiecesTable import *
from PuzzlesTable import *
from SolutionsTable import *
from PuzzlesAndSolution import *

ascii_art = """
 ______   ____  ____    ____  ____    ____  ___ ___     
|      | /    ||    \  /    ||    \  /    ||   |   |    
|      ||  o  ||  _  ||   __||  D  )|  o  || _   _ |    
|_|  |_||     ||  |  ||  |  ||    / |     ||  \_/  |    
  |  |  |  _  ||  |  ||  |_ ||    \ |  _  ||   |   |    
  |  |  |  |  ||  |  ||     ||  .  \|  |  ||   |   |    
  |__|  |__|__||__|__||___,_||__|\_||__|__||___|___|    
                                                        
 ____  __ __  _____  _____  _        ___                
|    \|  |  ||     ||     || |      /  _]               
|  o  )  |  ||__/  ||__/  || |     /  [_                
|   _/|  |  ||   __||   __|| |___ |    _]               
|  |  |  :  ||  /  ||  /  ||     ||   [_                
|  |  |     ||     ||     ||     ||     |               
|__|   \__,_||_____||_____||_____||_____|               
"""

menu_1 = """
-> Select an Option:
--> 1. pieces table
--> 2. puzzles table
--> 3. solutions table
--> 4. plot puzzles and solutions
--> 0. exit
-> """

menu_2 = """
-> Select an Option:
--> 1. print table
--> 2. plot table
--> 0. exit
-> """

def safe_exit(conn):
    print("Exting...")
    conn.close()
    exit(0)

def main():
    # Establish a connection to the database
    conn = psycopg2.connect(
        host="localhost",
        database="tangram_puzzle",
        user="davide",
        password="davide"
    )

    print(ascii_art)
    while True:
        table = None 

        choise = input(menu_1)
        if choise == '1':
            table = PiecesTable(conn)
        elif choise == '2':
            table = PuzzlesTable(conn)
        elif choise == '3':
            table = SolutionsTable(conn)
        elif choise == '4':
            tmp = PuzzlesAndSolution(conn)
            tmp.plot_table()
            continue
        elif choise == '0':
            safe_exit(conn)
        else:
            print("-> Please, select a valid option...")
            continue
        
        choise = input(menu_2)
        if choise == '1':
            table.print_table()
        elif choise == '2':
            table.plot_table()
        elif choise == '0':
            safe_exit(conn)
        else:
            print("-> Please, select a valid option...")
            continue

if __name__ == "__main__":
    register_geometry_type()
    main()