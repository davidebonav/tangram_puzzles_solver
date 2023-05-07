#include "Yap/YapInterface.h"
#include <libpq-fe.h>
#include "utils.h"

void init_predicates();
static int c_st_rotate(void);
static int c_st_difference(void);

void init_predicates()
{
    YAP_UserCPredicate("st_difference", c_st_difference, 4);
    YAP_UserCPredicate("st_rotate", c_st_rotate, 4);
}

/**
 * TEST st_difference -> consult(yap2postgres),db_open(localhost, postgres, 'gnazio', tangram_puzzle, conn),get_value(conn, ConnHandler),st_difference(ConnHandler, [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.5000000000000000,1.0000000000000000,0.0000000000000000,0.0000000000000000,0.0000000000000000]], [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.0000000000000000,0.2500000000000000,0.2500000000000000,0.0000000000000000,0.0000000000000000]], A).
 * TEST st_rotate -> consult(yap2postgres),db_open(localhost, postgres, 'gnazio', tangram_puzzle, conn),get_value(conn, ConnHandler),st_rotate(ConnHandler, [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.5000000000000000,1.0000000000000000,0.0000000000000000,0.0000000000000000,0.0000000000000000]], 90, RES).
*/

// st_difference(conn, geom1, geom2, res).
static int c_st_difference(void) 
{
    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);
    YAP_Term f1 = YAP_ARG2;
    YAP_Term f2 = YAP_ARG3;
    YAP_Term diff_res = YAP_ARG4;

    char *f1_WKL = extract_WKT_from_points(conn, f1);
    char *f2_WKL = extract_WKT_from_points(conn, f2);
    char sql[1024];
    sprintf(sql, "select ST_Difference(ST_GeomFromText('%s'), ST_GeomFromText('%s'))", f1_WKL, f2_WKL);
    PGresult *res = PQexec(conn, sql);
    free(f1_WKL);
    free(f2_WKL);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }
    char *res_str = PQgetvalue(res, 0, 0);
    sprintf(sql, "SELECT ST_NumInteriorRings('%s')", res_str);
    int n_int_rings = atoi(PQgetvalue(exec_sql(conn, sql), 0, 0)); // num of interior rings, add 1 to get the total amount of rings
    YAP_Term res_term = extract_values(conn, res_str,n_int_rings);
    if (!YAP_Unify(diff_res, res_term))
    {
        return FALSE;
    }
    return TRUE;
}

// st_rotate(conn, geom1, rads, res).
static int c_st_rotate(void) 
{
    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);
    YAP_Term f1 = YAP_ARG2;
    int rads = YAP_IntOfTerm(YAP_ARG3);
    YAP_Term rot_res = YAP_ARG4;

    char *f1_WKL = extract_WKT_from_points(conn, f1);
    char sql[1024];
    sprintf(sql, "select ST_Rotate(ST_GeomFromText('%s'), %d)", f1_WKL, rads);
    PGresult *res = PQexec(conn, sql);
    free(f1_WKL);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }
    char *res_str = PQgetvalue(res, 0, 0);
    sprintf(sql, "SELECT ST_NumInteriorRings('%s')", res_str);
    int n_int_rings = atoi(PQgetvalue(exec_sql(conn, sql), 0, 0)); // num of interior rings, add 1 to get the total amount of rings
    YAP_Term res_term = extract_values(conn, res_str,n_int_rings);
    if (!YAP_Unify(rot_res, res_term))
    {
        return FALSE;
    }
    return TRUE;
}