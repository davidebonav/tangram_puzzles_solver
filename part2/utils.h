#ifndef UTILS_H
#define UTILS_H

#define GEOM_OID 39689 //25637//38625// 3614
#define POLYG_OID 25637 

static bool finish_with_error(PGconn *conn, PGresult *res)
{
    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    if (res != NULL)
        PQclear(res);
    PQfinish(conn);
    return FALSE;
}


PGresult* exec_sql(PGconn *conn, char *sql) 
{
     PGresult *res = PQexec(conn, sql);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }

    return res;
}

char* extract_geometry(PGconn *conn, char* origin_value) {
    char cast_query[1024];
    sprintf(cast_query, "select st_astext('%s')", origin_value);
    PGresult *cast_res = PQexec(conn, cast_query);

    if (PQresultStatus(cast_res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", cast_query);
        finish_with_error(conn, cast_res);
    }
    return PQgetvalue(cast_res, 0, 0);
}

double** extract_values(PGconn *conn, char* geometry, int n_int_rings)
{
    char sql[1024];
    
    sprintf(sql, "SELECT ST_ExteriorRing('%s')", geometry);
    char *ext_ring = PQgetvalue(exec_sql(conn, sql), 0, 0); 
    sprintf(sql, "SELECT ST_NPoints('%s')", ext_ring);
    int n_ext_points = atoi(PQgetvalue(exec_sql(conn, sql), 0, 0));

    double **rings = malloc((n_int_rings +1) * sizeof(double*));
    
    rings[0] = malloc(n_ext_points*2*sizeof(double));
    extract_points_from_ring(conn, ext_ring, n_ext_points, rings[0]);
    for(int i=1; i<=n_int_rings; i++)
    {
        sprintf(sql, "SELECT ST_InteriorRingN('%s', %d)", geometry, i);
        char *int_ring = PQgetvalue(exec_sql(conn, sql), 0, 0); 
        sprintf(sql, "SELECT ST_NPoints('%s')", int_ring);
        int n_int_points = atoi(PQgetvalue(exec_sql(conn, sql), 0, 0));

        rings[i] = malloc(n_int_points*2*sizeof(double));
        extract_points_from_ring(conn, int_ring, n_int_points, rings[i]);
    }
   
    return rings;
}

/**
 * points[] array has n_points*2 size
*/
void extract_points_from_ring(PGconn *conn, char* ring, int n_points, double* points)
{
    char sql[1024];

    for(int i=0; i<n_points; i++)
    {
        int j = i*2;
        sprintf(sql, "SELECT ST_PointN('%s', %d)", ring, i+1);
        char *point = PQgetvalue(exec_sql(conn, sql), 0, 0);
        sprintf(sql, "SELECT ST_X('%s')", point);
        points[j] = atof(PQgetvalue(exec_sql(conn, sql), 0, 0));
        sprintf(sql, "SELECT ST_Y('%s')", point);
        points[j+1] = atof(PQgetvalue(exec_sql(conn, sql), 0, 0));
    }

}


#endif