#ifndef DB_UTILITIES_H
#define DB_UTILITIES_H

#include <libpq-fe.h>

void test_version();
void finish_with_error(PGconn *conn, PGresult *res, char *errMsg);
PGconn *db_connect();

#endif /* DB_UTILITIES_H */