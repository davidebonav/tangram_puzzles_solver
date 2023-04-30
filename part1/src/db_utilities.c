#include <libpq-fe.h>

#include <stdlib.h>

#include <db_utilities.h>

void test_version()
{
    int lib_ver = PQlibVersion();
    printf("PQlibVersion: %d\n", lib_ver);
}

void finish_with_error(PGconn *conn, PGresult *res, char *errMsg)
{
    fprintf(stderr, "%s: %s\n", errMsg, PQerrorMessage(conn));
    if (res != NULL)
        PQclear(res);
    PQfinish(conn);
    exit(-1);
}

PGconn *db_connect()
{
    char *connectionInfo = "user=davide password=davide dbname=tangram_puzzle";
    // snprintf(prefix, sizeof(prefix), "%s: %s: %s", argv[0], cmd_argv[0], cmd_argv[1]);
    PGconn *conn = PQconnectdb(connectionInfo);
    if (PQstatus(conn) != CONNECTION_OK)
        finish_with_error(conn, NULL, "Connection to database failed");
    printf("Connection successful...\n");
    return conn;
}
