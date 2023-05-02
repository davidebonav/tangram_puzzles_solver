#include <libpq-fe.h>

#include <stdio.h>
#include <stdlib.h>

#include <pieces.h>
#include <db_utilities.h>

int createPiecesTable(PGconn *conn)
{
  int num_rows;
  const char *create_table_sql = "CREATE TABLE IF NOT EXISTS \
      pieces( \
          id SERIAL PRIMARY KEY, \
          piece_type VARCHAR(40), \
          color VARCHAR(20), \
          shape GEOMETRY);";

  num_rows = createTable(conn, create_table_sql);

  return num_rows;
}

int populatePiecesTable(PGconn *conn)
{
  const char *insert_sql = "INSERT INTO pieces(piece_type, color, shape) \
      VALUES('large_right_triangle', 'red', 'POLYGON((0 0, 0.5 0.5, 1 0, 0 0))'), \
             ('large_right_triangle', 'orange', 'POLYGON((0 0, 0.5 0.5, 1 0, 0 0))'), \
             ('medium_right_triangle', 'blue', 'POLYGON((0 0, 0.5 0, 0.5 0.5, 0 0))'), \
             ('small_right_triangle', 'viola', 'POLYGON((0 0, 0.5 0, 0.25 0.25, 0 0))'), \
             ('small_right_triangle', 'purple', 'POLYGON((0 0, 0.5 0, 0.25 0.25, 0 0))'), \
             ('square', 'yellow', 'POLYGON((0 0, 0.25 0.25, 0.5 0, 0.25 -0.25, 0 0))'), \
             ('parallelogram', 'green', 'POLYGON((0 0, 0.5 0, 0.75 0.25, 0.25 0.25, 0 0))');";
  /*
    • 2 large right triangles (hypotenuse 1, sides √2/2, area 1/4)
    • 1 medium right triangle (hypotenuse √2/2, sides 1/2, area 1/8)
    • 2 small right triangles (hypotenuse 1/2, sides √2/4, area 1/16)
    • 1 square (sides √2/4, area 1/8)
    • 1 parallelogram (sides of 1/2 and √2/4, height of 1/4, area 1/8)
  */

  return insertIntoTable(conn, insert_sql);
}

int createAndPopulatePiecesTable(PGconn *conn)
{
  if (createPiecesTable(conn) != 0 || countRowPiecesTable(conn) == 0)
    populatePiecesTable(conn);
  else
    printf("Table already populated, skipping...\n");
    
  return 0;
}

int printPiecesTable(PGconn *conn)
{
  const char *query = "SELECT id,piece_type, color, st_astext(shape) from pieces";
  PGresult *res = sqlQuery(conn, query);

  // Get the number of rows and columns in the result
  int n_rows = PQntuples(res);
  int n_cols = PQnfields(res);

  // Print the result
  printf("%d rows returned\n", n_rows);
  for (int i = 0; i < n_rows; i++)
  {
    for (int j = 0; j < n_cols; j++)
    {
      printf("%s\t", PQgetvalue(res, i, j));
    }
    printf("\n");
  }

  // Free the memory used by the result and close the connection
  PQclear(res);
  return 0;
}

int countRowPiecesTable(PGconn *conn)
{
  const char *query = "SELECT COUNT(*) from pieces";
  PGresult *res = sqlQuery(conn, query);

  int n = atoi(PQgetvalue(res, 0, 0));

  // Free the memory used by the result and close the connection
  PQclear(res);
  return n;
}