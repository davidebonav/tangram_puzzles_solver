#include <libpq-fe.h>

#include <stdlib.h>

#include <db_utilities.h>

void test_version()
{
    int lib_ver = PQlibVersion();
    printf("PQlibVersion: %d\n", lib_ver);
}

void finish_with_error(PGconn *conn, PGresult *res)
{
    fprintf(stderr, "%s\n", PQerrorMessage(conn));
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
    {
        fprintf(stderr, "Connection to database failed...\n");
        finish_with_error(conn, NULL);
    }
    printf("Connection successful...\n");
    return conn;
}

int createTable(PGconn *conn, const char *create_table_sql, const char *drop_if_exist_sql)
{
    printf("Creating table...\n");

    PGresult *res;
    int num_rows;

    // Create the 'tableName' table
    res = PQexec(conn, drop_if_exist_sql);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Table deletion failed... %s \n", create_table_sql);
        finish_with_error(conn, res);
    }
    res = PQexec(conn, create_table_sql);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Table creation failed... %s \n", create_table_sql);
        finish_with_error(conn, res);
    }

    num_rows = atoi(PQcmdTuples(res));
    // PQcmdTuples retrieve # of rows affected by the most recent SQL command.
    // num_rows == 0 ? Table already existed : Table created

    PQclear(res);
    return num_rows;
}

int insertIntoTable(PGconn *conn, const char *insert_sql)
{
    printf("Adding records into table...\n");

    PGresult *res;

    // Insert the data into the 'tableName' table
    res = PQexec(conn, insert_sql);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Data insertion failed... %s\n", insert_sql);
        finish_with_error(conn, res);
    }

    PQclear(res);
    return 0;
}

PGresult *sqlQuery(PGconn *conn, const char *sql_query)
{
    PGresult *res;

    // Execute the select query
    res = PQexec(conn, sql_query);

    // Check if the query was executed successfully
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql_query);
        finish_with_error(conn, res);
    }

    return res;
}