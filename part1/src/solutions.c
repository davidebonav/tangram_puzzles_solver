#include <solutions.h>

// SQL
const char *create_solutions_table_sql = "CREATE TABLE IF NOT EXISTS \
      solutions( \
          puzzle_id INT, \
          piece_id INT, \
          piece_coordinates GEOMETRY(POLYGON,3763), \
          PRIMARY KEY (puzzle_id, piece_id), \
          CONSTRAINT fk_solutions_puzzle_id FOREIGN KEY (puzzle_id) REFERENCES puzzles (id) ON DELETE CASCADE, \
          CONSTRAINT fk_solutions_pieces_id FOREIGN KEY (piece_id) REFERENCES pieces (id) ON DELETE CASCADE \
          );";
const char *insert_solutions_sql = "INSERT INTO solutions(puzzle_id, piece_id, piece_coordinates) VALUES \
      \
      (1,1,'POLYGON((-0.25 1.25, 0.50 1.25, 0.50 0.50, -0.25 1.25))'), \
      (1,2,'POLYGON((1.25 1.25, 0.50 1.25, 0.50 0.50, 1.25 1.25))'), \
      (1,3,'POLYGON((0.75 0.75, 0.75 0.25, 0.25 0.25, 0.75 0.75))'), \
      (1,4,'POLYGON((0.75 0.75, 0.75 0.25, 1 0.5, 0.75 0.75))'), \
      (1,5,'POLYGON((0 0, 0.5 0, 0.25 0.25, 0 0))'), \
      (1,6,'POLYGON((0 0.5, 0.25 0.75, 0.5 0.5, 0.25 0.25, 0 0.5))'), \
      (1,7,'POLYGON((1 0, 0.5 0, 0.25 0.25, 0.75 0.25, 1 0))'), \
      \
      (2,1,'POLYGON((0 0, 0.5 0.5, 1 0, 0 0))'), \
      (2,2,'POLYGON((0.75 0.25, 0.25 0.75, 0.75 1.25, 0.75 0.25))'), \
      (2,3,'POLYGON((1.25 1.25, 0.75 1.25, 0.75 0.75, 1.25 1.25))'), \
      (2,4,'POLYGON((0.5 1, 0 1, 0.25 0.75, 0.5 1))'), \
      (2,5,'POLYGON((0.75 0.75, 0.75 0.25, 1 0.5, 0.75 0.75))'), \
      (2,6,'POLYGON((0 0.5, 0.25 0.75, 0.5 0.5, 0.25 0.25, 0 0.5))'), \
      (2,7,'POLYGON((-0.25 1.25, 0.25 1.25, 0.5 1, 0 1, -0.25 1.25))');";
const char *select_all_solutions_sql = "SELECT puzzle_name, piece_type, st_astext(piece_coordinates) \
    FROM solutions s, pieces pi, puzzles pu \
    WHERE s.puzzle_id=pu.id AND s.piece_id=pi.id";
const char *select_count_solutions_sql = "SELECT COUNT(*) from solutions";

// FUNCTIONS
int createSolutionsTable(PGconn *conn)
{
  int num_rows;
  num_rows = createTable(conn, create_solutions_table_sql);
  return num_rows;
}

int populateSolutionsTable(PGconn *conn)
{
  return insertIntoTable(conn, insert_solutions_sql);
}

int createAndPopulateSolutionsTable(PGconn *conn)
{
  if (createSolutionsTable(conn) != 0 || countRowSolutionsTable(conn) == 0)
    populateSolutionsTable(conn);
  else
    printf("Table solutions already populated, skipping...\n");
    
  return 0;
}

int printSolutionsTable(PGconn *conn)
{
  PGresult *res = sqlQuery(conn, select_all_solutions_sql);

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

int countRowSolutionsTable(PGconn *conn)
{
  PGresult *res = sqlQuery(conn, select_count_solutions_sql);
  int n = atoi(PQgetvalue(res, 0, 0));
  PQclear(res);
  return n;
}