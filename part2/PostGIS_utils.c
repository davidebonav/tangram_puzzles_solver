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

void execute_PostGIS_function(PGconn *conn, char *function_name, char *geometry, char *output)
{
    char sql[1024];
    PGresult *result_set;

    sprintf(sql, "SELECT %s('%s')", function_name, geometry);

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

void executeN_PostGIS_function(PGconn *conn, char *function_name, char *geometry, int n, char *output)
{
    char sql[1024];
    PGresult *result_set;

    sprintf(sql, "SELECT %s('%s', %d)", function_name, geometry, n);

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

void executeD_PostGIS_function(PGconn *conn, char *function_name, char *geometry, double d, char *output)
{
    char sql[1024];
    PGresult *result_set;

    sprintf(sql, "SELECT %s('%s', %f)", function_name, geometry, d);

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

void executeS_PostGIS_function(PGconn *conn, char *function_name, char *geometry1, char *geometry2, char *output)
{
    char sql[1024];
    PGresult *result_set;

    sprintf(sql, "SELECT %s('%s', '%s')", function_name, geometry1, geometry2);

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

void executeFF_PostGIS_function(PGconn *conn, char *function_name, char *geometry1, double x, double y, char *output)
{
    char sql[1024];
    PGresult *result_set;

    sprintf(sql, "SELECT %s('%s', %f, %f)", function_name, geometry1, x, y);

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

void execute_arbitrary_function(PGconn *conn, char *sql, char *output)
{
    PGresult *result_set;

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

/**
 * points[] array has n_points*2 size
 */
YAP_Term extract_points_from_ring(PGconn *conn, char *ring, int n_points)
{
    YAP_Term point_array[n_points];
    for (int i = 1; i <= n_points; i++)
    {
        char point[1024], xy[1024];

        executeN_PostGIS_function(conn, "ST_PointN", ring, i, point);

        execute_PostGIS_function(conn, "ST_X", point, xy);
        YAP_Term head = YAP_MkFloatTerm(atof(xy));
        execute_PostGIS_function(conn, "ST_Y", point, xy);
        YAP_Term tail = YAP_MkFloatTerm(atof(xy));

        point_array[i - 1] = YAP_MkPairTerm(head, tail);
    }
    return YAP_MkListFromTerms(point_array, n_points);
}

// PUBLIC FUNCTIONS
YAP_Term extract_values(PGconn *conn, char *geometry, int n_int_rings)
{
    YAP_Term rings_term[n_int_rings + 1];
    char ext_ring[1024], n_ext_points[16];

    execute_PostGIS_function(conn, "ST_ExteriorRing", geometry, ext_ring);
    execute_PostGIS_function(conn, "ST_NPoints", ext_ring, n_ext_points);
    rings_term[0] = extract_points_from_ring(conn, ext_ring, atoi(n_ext_points));

    for (int i = 1; i <= n_int_rings; i++)
    {
        char int_ring[1024], n_int_points_char[16];

        executeN_PostGIS_function(conn, "ST_InteriorRingN", geometry, i, int_ring);
        execute_PostGIS_function(conn, "ST_NPoints", int_ring, n_int_points_char);
        int n_int_points = atoi(n_int_points_char);
        rings_term[i] = extract_points_from_ring(conn, int_ring, n_int_points);
    }

    YAP_Term res = YAP_MkListFromTerms(rings_term, n_int_rings + 1);
    return res;
}

/**
 * geom: YAP_Term is an array of YAP_Terms, each of it is an array of double
 * returns: output buffer of size 1024 string of WKT representing the POLYGON
 */
char *extract_WKT_from_points(YAP_Term points_list)
{
    // POLYGON((0 0,0.5 0.5,1 0,0 0))
    char str[1024];

    memset(str, 0, sizeof(str));
    strcpy(str, "POLYGON");

    char *WKT_string = malloc(1024 * sizeof(char));

    if (!create_WKT_points_string(points_list, str))
        strcpy(WKT_string, "POLYGON EMPTY");
    else
        strcpy(WKT_string, str);

    return WKT_string;
}

bool create_WKT_points_string(YAP_Term points_list, char *WKT_string)
{
    char buffer[1024];
    int lenght = YAP_ListLength(points_list);

    if (lenght == 0)
        return false;

    YAP_Term head = YAP_HeadOfTerm(points_list),
             list = YAP_TailOfTerm(points_list);

    if (lenght == -1 && YAP_IsFloatTerm(head) && YAP_IsFloatTerm(list))
    {
        float x, y;
        x = YAP_FloatOfTerm(head);
        y = YAP_FloatOfTerm(list);

        sprintf(buffer, "%f %f", x, y);
        strcat(WKT_string, buffer);
    }
    else
    {
        strcat(WKT_string, "(");
        for (int i = 0; i < lenght; i++)
        {
            if (!create_WKT_points_string(head, WKT_string))
                return false;

            if (i != lenght - 1)
            {
                strcat(WKT_string, ",");

                head = YAP_HeadOfTerm(list);
                list = YAP_TailOfTerm(list);
            }
        }
        strcat(WKT_string, ")");
    }
    return true;
}