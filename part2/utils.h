#ifndef UTILS_H
#define UTILS_H

static bool finish_with_error(PGconn *conn, PGresult *res)
{
    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    if (res != NULL)
        PQclear(res);
    PQfinish(conn);
    return FALSE;
}

char* extract_geometry(PGconn *conn, char* origin_value) {
    char *cast_query=(char *)malloc(1024 * sizeof(char));
    sprintf(cast_query, "select st_astext('%s')", origin_value);
    PGresult *cast_res = PQexec(conn, cast_query);

    if (PQresultStatus(cast_res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", cast_query);
        finish_with_error(conn, cast_res);
    }
    return PQgetvalue(cast_res, 0, 0);
}



#endif