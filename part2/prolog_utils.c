#include "Yap/YapInterface.h"
#include <libpq-fe.h>
#include "utils.h"

void init_predicates()
{
     YAP_UserCPredicate("st_difference", c_st_difference, 4);
}

// st_diff(conn, geom1, geom2, res).

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
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }
    if (!YAP_Unify(diff_res, YAP_MkIntTerm((long)PQgetvalue(res, 0, 0))))
    {
        return FALSE;
    }
    return TRUE;
}


/* static int c_st_diff(void) 
{
    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);
    char *f1 = YAP_AtomName(YAP_AtomOfTerm(YAP_ARG2));
    char *f2 = YAP_AtomName(YAP_AtomOfTerm(YAP_ARG3));
    YAP_Term diff_res = YAP_ARG4;

    char sql[1024];
    sprintf(sql, "select ST_Difference(ST_GeomFromText('%s'), ST_GeomFromText('%s'))", f1, f2);
    PGresult *res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }
    if (!YAP_Unify(diff_res, YAP_MkIntTerm((long)PQgetvalue(res, 0, 0))))
    {
        return FALSE;
    }
    return TRUE;
} */