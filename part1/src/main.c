#include <libpq-fe.h>
#include <stdlib.h>

#include <db_utilities.h>
#include <pieces.h>
#include <puzzles.h>

// int main(int argc, char **argv)
int main()
{
  PGconn *conn = db_connect();

  createAndPopulatePiecesTable(conn);
  printPiecesTable(conn);

  createAndPopulatePuzzlesTable(conn);
  printPuzzlesTable(conn);

  PQfinish(conn);
  return 0;
}