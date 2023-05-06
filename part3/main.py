#!/usr/bin/python3

import psycopg2
from utils import register_geometry_type
from pieces_utils import *

def main():
    # Establish a connection to the database
    conn = psycopg2.connect(
        host="localhost",
        database="tangram_puzzle",
        user="davide",
        password="davide"
    )

    # print_pieces_table(conn)
    plot_pieces_table(conn)
    # tmp5()

    # Close the connection
    conn.close()

if __name__ == "__main__":
    register_geometry_type()
    main()