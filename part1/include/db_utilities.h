#ifndef DB_UTILITIES_H
#define DB_UTILITIES_H

#include <libpq-fe.h>

void test_version();
void finish_with_error(PGconn *conn, PGresult *res);
PGconn *db_connect();
int createTable(PGconn *conn, const char *create_table_sql);
int insertIntoTable(PGconn *conn, const char *insert_sql);
PGresult *sqlQuery(PGconn *conn, const char *sql_query);

#endif /* DB_UTILITIES_H */