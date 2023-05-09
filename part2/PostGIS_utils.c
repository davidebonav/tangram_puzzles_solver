#include "postGIS_utils.h"

static bool finish_with_error(PGconn *conn, PGresult *res)
{
    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    if (res != NULL)
        PQclear(res);
    PQfinish(conn);
    return FALSE;
}

PGresult *exec_sql(PGconn *conn, char *sql)
{
    PGresult *res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }

    return res;
}

char *execute_PostGIS_function(PGconn *conn, char *function_name, char *geometry)
{
    char sql[1024];
    PGresult *result_set;
    char *output;

    sprintf(sql, "SELECT %s('%s')", function_name, geometry);

    result_set = exec_sql(conn, sql);
    output = PQgetvalue(result_set, 0, 0);
    PQclear(result_set);
    return output;
}

char *executeN_PostGIS_function(PGconn *conn, char *function_name, char *geometry, int n)
{
    char sql[1024];
    PGresult *result_set;
    char *output;

    sprintf(sql, "SELECT %s('%s', %d)", function_name, geometry, n);

    result_set = exec_sql(conn, sql);
    output = PQgetvalue(result_set, 0, 0);
    PQclear(result_set);
    return output;
}

/**
 * points[] array has n_points*2 size
 */
YAP_Term extract_points_from_ring(PGconn *conn, char *ring, int n_points)
{
    YAP_Term point_array[n_points];
    for (int i = 1; i <= n_points; i++)
    {
        char *point;
        
        point = executeN_PostGIS_function(conn, "ST_PointN", ring, i);

        YAP_Term head = YAP_MkFloatTerm(atof(execute_PostGIS_function(conn, "ST_X", point)));
        YAP_Term tail = YAP_MkFloatTerm(atof(execute_PostGIS_function(conn, "ST_Y", point)));

        point_array[i-1] = YAP_MkPairTerm(head,tail);
    }
    return YAP_MkListFromTerms(point_array, n_points);
}

// PUBLIC FUNCTIONS
YAP_Term extract_values(PGconn *conn, char* geometry, int n_int_rings)
{
    YAP_Term rings_term[n_int_rings+1];
    char geom[1024];

    char *ext_ring = execute_PostGIS_function(conn, "ST_ExteriorRing", geometry);
    strcpy(geom, ext_ring);
    int n_ext_points = atoi(execute_PostGIS_function(conn, "ST_NPoints", geom));
    rings_term[0] = extract_points_from_ring(conn, geom, n_ext_points);

    for(int i=1; i<=n_int_rings; i++)
    {
        char *int_ring = executeN_PostGIS_function(conn, "ST_InteriorRingN", geometry,i);
        int n_int_points = atoi(execute_PostGIS_function(conn, "ST_NPoints", int_ring));
        rings_term[i] = extract_points_from_ring(conn, int_ring, n_int_points);
    }

    YAP_Term res = YAP_MkListFromTerms(rings_term, n_int_rings+1);
    return res;
}