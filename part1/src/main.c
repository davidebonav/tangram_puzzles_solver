#include <libpq-fe.h>
#include <stdlib.h>
#include <db_utilities.h>
#include <pieces.h>

// int main(int argc, char **argv)
int main()
{
  PGconn *conn = db_connect();
  if (createPiecesTable(conn) != 0 || countPiecesTable(conn) == 0)
    populatePiecesTable(conn);
  testPiecesTable(conn);

  PQfinish(conn);
  return 0;
}