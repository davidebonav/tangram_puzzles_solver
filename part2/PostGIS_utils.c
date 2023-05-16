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

void execute_PostGIS_function(PGconn *conn, char *sql, char *output)
{
    PGresult *result_set;

    result_set = exec_sql(conn, sql);
    strcpy(output, PQgetvalue(result_set, 0, 0));
    PQclear(result_set);
    // printf("-> %s\t%s", sql, output);
}

// Extract YAP TERM
YAP_Term extract_point(PGconn *conn, char *point)
{
    char tmp[1024], sql[1024];

    sprintf(sql, "SELECT ST_X('%s')", point);
    execute_PostGIS_function(conn, sql, tmp);
    YAP_Term head = YAP_MkFloatTerm(atof(tmp));

    sprintf(sql, "SELECT ST_Y('%s')", point);
    execute_PostGIS_function(conn, sql, tmp);
    YAP_Term tail = YAP_MkFloatTerm(atof(tmp));

    return YAP_MkPairTerm(head, tail);
}

YAP_Term extract_ring(PGconn *conn, char *ring)
{
    char c_nPoints[16], sql[1024];
    int i_nPoints;

    sprintf(sql, "SELECT ST_NPoints('%s')", ring);
    execute_PostGIS_function(conn, sql, c_nPoints);
    i_nPoints = atoi(c_nPoints);

    YAP_Term yap_points[i_nPoints];

    for (int i = 0; i < i_nPoints; i++)
    {
        char point[1024];
        sprintf(sql, "SELECT ST_PointN('%s', %d)", ring, i + 1);
        execute_PostGIS_function(conn, sql, point);
        yap_points[i] = extract_point(conn, point);
    }
    return YAP_MkListFromTerms(yap_points, i_nPoints);
}

YAP_Term extract_polygon(PGconn *conn, char *polygon)
{
    char c_numInteriorRings[16], sql[1024];
    int i_numInteriorRings;

    sprintf(sql, "SELECT ST_NumInteriorRings('%s')", polygon);
    execute_PostGIS_function(conn, sql, c_numInteriorRings);
    i_numInteriorRings = atoi(c_numInteriorRings);

    YAP_Term yap_rings[i_numInteriorRings + 1];

    // Extract the exterior ring
    char exteriorRing[1024];
    sprintf(sql, "SELECT ST_ExteriorRing('%s')", polygon);
    execute_PostGIS_function(conn, sql, exteriorRing);
    yap_rings[0] = extract_ring(conn, exteriorRing);

    // extract all the interior rings
    for (int i = 1; i <= i_numInteriorRings; i++)
    {
        char interiorRing[1024];
        sprintf(sql, "SELECT ST_InteriorRingN('%s', %d)", polygon, i);
        execute_PostGIS_function(conn, sql, interiorRing);
        yap_rings[i] = extract_ring(conn, interiorRing);
    }

    YAP_Term res = YAP_MkListFromTerms(yap_rings, i_numInteriorRings + 1);
    return res;
}

YAP_Term extract_geometry(PGconn *conn, char *geometry)
{

    char c_numGeometries[16], sql[512];
    int i_numGeometries;

    sprintf(sql, "SELECT ST_NumGeometries('%s')", geometry);
    execute_PostGIS_function(conn, sql, c_numGeometries);
    i_numGeometries = atoi(c_numGeometries);

    YAP_Term yap_polygons[i_numGeometries];
    char polygon[1024];

    for (int i = 0; i < i_numGeometries; i++)
    {
        sprintf(sql, "SELECT ST_GeometryN('%s', %d)", geometry, i + 1);
        execute_PostGIS_function(conn, sql, polygon);
        yap_polygons[i] = extract_polygon(conn, polygon);
    }

    YAP_Term res = YAP_MkListFromTerms(yap_polygons, i_numGeometries);
    return res;
}

// EXTRACTY WKT
char *extract_WKT_from_points(YAP_Term points_list)
{
    // POLYGON((0 0,0.5 0.5,1 0,0 0))
    char str[1024];

    memset(str, 0, sizeof(str));
    strcpy(str, "MULTIPOLYGON");

    char *WKT_string = malloc(1024 * sizeof(char));

    if (!create_WKT_points_string(points_list, str))
        strcpy(WKT_string, "MULTIPOLYGON EMPTY");
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