#include <pieces.h>

// SQL

const char *drop_pieces_if_exist = "DROP TABLE IF EXISTS pieces;";

const char *create_pieces_table_sql = "CREATE TABLE IF NOT EXISTS \
      pieces( \
          id SERIAL PRIMARY KEY, \
          piece_type VARCHAR(40), \
          color VARCHAR(20), \
          shape GEOMETRY(POLYGON,3763));";
/*
  • 2 large right triangles (hypotenuse 1, sides √2/2, area 1/4)
  • 1 medium right triangle (hypotenuse √2/2, sides 1/2, area 1/8)
  • 2 small right triangles (hypotenuse 1/2, sides √2/4, area 1/16)
  • 1 square (sides √2/4, area 1/8)
  • 1 parallelogram (sides of 1/2 and √2/4, height of 1/4, area 1/8)
*/
const char *insert_pieces_sql = "INSERT INTO pieces(piece_type, color, shape) \
      VALUES('large_right_triangle', 'red', 'POLYGON((0 0, 0.5 0.5, 1 0, 0 0))'), \
             ('large_right_triangle', 'orange', 'POLYGON((0 0, 0.5 0.5, 1 0, 0 0))'), \
             ('medium_right_triangle', 'cyan', 'POLYGON((0 0, 0.5 0, 0.5 0.5, 0 0))'), \
             ('small_right_triangle', 'fuchsia', 'POLYGON((0 0, 0.5 0, 0.25 0.25, 0 0))'), \
             ('small_right_triangle', 'indigo', 'POLYGON((0 0, 0.5 0, 0.25 0.25, 0 0))'), \
             ('square', 'yellow', 'POLYGON((0 0, 0.25 0.25, 0.5 0, 0.25 -0.25, 0 0))'), \
             ('parallelogram', 'green', 'POLYGON((0.25 0, 0.75 0, 0.5 0.25, 0 0.25, 0.25 0))');";
const char *select_all_pieces_sql = "SELECT id,piece_type, color, st_astext(shape) from pieces";
const char *select_count_pieces_sql = "SELECT COUNT(*) from pieces";

// FUNCTIONS
int createPiecesTable(PGconn *conn)
{
  int num_rows;
  num_rows = createTable(conn, create_pieces_table_sql, drop_pieces_if_exist);
  return num_rows;
}

int populatePiecesTable(PGconn *conn)
{
  return insertIntoTable(conn, insert_pieces_sql);
}

int createAndPopulatePiecesTable(PGconn *conn)
{
  if (createPiecesTable(conn) != 0 || countRowPiecesTable(conn) == 0)
    populatePiecesTable(conn);
  else
    printf("Table pieces already populated, skipping...\n");
    
  return 0;
}

int printPiecesTable(PGconn *conn)
{
  PGresult *res = sqlQuery(conn, select_all_pieces_sql);

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

  PQclear(res);
  return 0;
}

int countRowPiecesTable(PGconn *conn)
{
  PGresult *res = sqlQuery(conn, select_count_pieces_sql);
  int n = atoi(PQgetvalue(res, 0, 0));
  PQclear(res);
  return n;
}