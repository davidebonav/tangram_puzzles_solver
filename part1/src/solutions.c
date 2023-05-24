#include <solutions.h>

// SQL
const char *drop_solutions_if_exist = "DROP TABLE IF EXISTS solutions;";

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
      (2,7,'POLYGON((-0.25 1.25, 0.25 1.25, 0.5 1, 0 1, -0.25 1.25))'), \
      \
      (3,1,'POLYGON((-0.18 1.24, 0.53 1.24, 0.53 0.53, -0.18 1.24))'), \
      (3,2,'POLYGON((1.24 1.24, 0.53 1.24, 0.53 0.53, 1.24 1.24))'), \
      (3,3,'POLYGON((1.05 0, 0.70 0.35, 0.35 0, 1.05 0))'), \
      (3,4,'POLYGON((0.78 0.78, 1.03 0.53, 0.53 0.53, 0.78 0.78))'), \
      (3,5,'POLYGON((0.77 0.28, 1.03 0.53, 0.53 0.53, 0.77 0.28))'), \
      (3,6,'POLYGON((0.02 0.53, 0.277 0.782, 0.53 0.53, 0.28 0.28, 0.02 0.53))'), \
      (3,7,'POLYGON((0 0, 0.35 0, 0.70 0.35, 0.35 0.35,0 0))'), \
      \
      (4,1,'POLYGON((0 0, 0.509 0.495, 1.004 0, 0 0))'), \
      (4,2,'POLYGON((0.509 0.495, 1.004 0, 1.505 0.495, 0.509 0.495))'), \
      (4,3,'POLYGON((1.754 0.744, 1.754 0.249, 2.256 0.744, 1.754 0.744))'), \
      (4,4,'POLYGON((0.651 0, 1.004 0, 1.004 -0.354, 0.651 0))'), \
      (4,5,'POLYGON((1.004 0, 1.505 0, 1.258 0.25, 1.004 0))'), \
      (4,6,'POLYGON((1.258 0.25, 1.505 0, 1.754 0.249, 1.505 0.495, 1.258 0.25))'), \
      (4,7,'POLYGON((1.754 0.249, 1.505 0.495, 1.505 0.991, 1.754 0.744,1.754 0.249))'), \
      \
      (5,1,'POLYGON((0 0, 0 0.7, 0.705 0, 0 0))'), \
      (5,2,'POLYGON((0 0.7, 0.502 0.198, 0.502 1.202, 0 0.7))'), \
      (5,3,'POLYGON((0.502 1.202, 0.502 0.497, 0.852 0.852, 0.502 1.202))'), \
      (5,4,'POLYGON((1.205 1.351, 0.9555 1.602, 0.955 1.101, 1.205 1.351))'), \
      (5,5,'POLYGON((1.205 1.351, 1.454 1.602, 1.454 1.101, 1.205 1.351))'), \
      (5,6,'POLYGON((1.205 1.351, 1.454 1.101, 1.203 0.853, 0.955 1.101, 1.205 1.351))'), \
      (5,7,'POLYGON((0.502 1.202, 0.853 1.204, 1.203 0.853, 0.852 0.852, 0.502 1.202))');";
const char *select_all_solutions_sql = "SELECT puzzle_name, piece_type, st_astext(piece_coordinates) \
    FROM solutions s, pieces pi, puzzles pu \
    WHERE s.puzzle_id=pu.id AND s.piece_id=pi.id";
const char *select_count_solutions_sql = "SELECT COUNT(*) from solutions";

// FUNCTIONS
int createSolutionsTable(PGconn *conn)
{
  int num_rows;
  num_rows = createTable(conn, create_solutions_table_sql, drop_solutions_if_exist);
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