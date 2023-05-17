#include <puzzles.h>

// SQL
const char *create_puzzles_table_sql = "CREATE TABLE IF NOT EXISTS \
      puzzles( \
          id SERIAL PRIMARY KEY, \
          puzzle_name VARCHAR(40), \
          shape GEOMETRY(POLYGON,3763));";
/* const char *insert_puzzles_sql = "INSERT INTO puzzles(puzzle_name, shape) \
       VALUES('', 'first_puzzle', 'POLYGON((0.25 0.25,0 0.5,0.25 0.75,-0.25 1.25,0.5 1.25,1.25 1.25,0.75 0.75,1 0.5,0.75 0.25,1 0,0.5 0,0 0,0.25 0.25))'), \
              ('', 'second_puzzle', 'POLYGON((0 0.5,0.25 0.75,0 1,-0.25 1.25,0.25 1.25,0.5 1,0.75 1.25,1.25 1.25,0.75 0.75,1 0.5,0.75 0.25,1 0,0 0,0.25 0.25,0 0.5))'), \
              ('', '', '');";*/
const char *insert_puzzles_sql = "INSERT INTO puzzles(puzzle_name, shape) VALUES\
      ('first_puzzle', 'POLYGON((0.25 0.25,0 0.5,0.25 0.75,-0.25 1.25,0.5 1.25,1.25 1.25,0.75 0.75,1 0.5,0.75 0.25,1 0,0.5 0,0 0,0.25 0.25))'), \
      ('second_puzzle', 'POLYGON((0 0.5,0.25 0.75,0 1,-0.25 1.25,0.25 1.25,0.5 1,0.75 1.25,1.25 1.25,0.75 0.75,1 0.5,0.75 0.25,1 0,0 0,0.25 0.25,0 0.5))'), \
      ('third_puzzle', 'POLYGON((0 0,1.06 0,0.78 0.28,1.03 0.53,0.78 0.78,1.23 1.24,-0.18 1.24,0.27 0.78,0.02 0.53,0.27 0.28,0 0), (0.53 0.53, 0.70 0.35, 0.35 0.35, 0.53 0.53))');";
const char *select_all_puzzles_sql = "SELECT id, puzzle_name, st_astext(shape) from puzzles";
const char *select_count_puzzles_sql = "SELECT COUNT(*) from puzzles";

// FUNCTIONS
int createPuzzlesTable(PGconn *conn)
{
  int num_rows;
  num_rows = createTable(conn, create_puzzles_table_sql);
  return num_rows;
}

int populatePuzzlesTable(PGconn *conn)
{
  return insertIntoTable(conn, insert_puzzles_sql);
}

int createAndPopulatePuzzlesTable(PGconn *conn)
{
  if (createPuzzlesTable(conn) != 0 || countRowPuzzlesTable(conn) == 0)
    populatePuzzlesTable(conn);
  else
    printf("Table puzzles already populated, skipping...\n");
    
  return 0;
}

int printPuzzlesTable(PGconn *conn)
{
  PGresult *res = sqlQuery(conn, select_all_puzzles_sql);

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

int countRowPuzzlesTable(PGconn *conn)
{
  PGresult *res = sqlQuery(conn, select_count_puzzles_sql);
  int n = atoi(PQgetvalue(res, 0, 0));
  PQclear(res);
  return n;
}