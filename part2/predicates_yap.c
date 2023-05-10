#include "Yap/YapInterface.h"
#include <libpq-fe.h>
#include "postGIS_utils.h"

void init_predicates();
static int c_st_rotate(void);
static int c_st_difference(void);
static int c_st_translate(void);
static int yap_predicate_to_WTK(void);

void init_predicates()
{
    YAP_UserCPredicate("st_difference", c_st_difference, 4);
    YAP_UserCPredicate("st_rotate", c_st_rotate, 4);
    YAP_UserCPredicate("st_translate", c_st_translate, 4);
    YAP_UserCPredicate("yap_predicate_to_WTK", yap_predicate_to_WTK, 2);
}

/**
 * TEST st_difference -> consult(yap2postgres),db_open(localhost, postgres, 'gnazio', tangram_puzzle, conn),get_value(conn, ConnHandler),st_difference(ConnHandler, [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.5000000000000000,1.0000000000000000,0.0000000000000000,0.0000000000000000,0.0000000000000000]], [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.0000000000000000,0.2500000000000000,0.2500000000000000,0.0000000000000000,0.0000000000000000]], A).
 * TEST st_rotate -> consult(yap2postgres),db_open(localhost, postgres, 'gnazio', tangram_puzzle, conn),get_value(conn, ConnHandler),st_rotate(ConnHandler, [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.5000000000000000,1.0000000000000000,0.0000000000000000,0.0000000000000000,0.0000000000000000]], 90, RES).
 * TEST st_translate -> consult(yap2postgres),db_open(localhost, postgres, 'gnazio', tangram_puzzle, conn),get_value(conn, ConnHandler),st_translate(ConnHandler, [[0.0000000000000000,0.0000000000000000,0.5000000000000000,0.5000000000000000,1.0000000000000000,0.0000000000000000,0.0000000000000000,0.0000000000000000]], 0.25, 0.25, RES).
 */

// st_difference(ConnHandler, Geometry1, Geometry2, DifferenceGeometry).
static int c_st_difference(void)
{
    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);
    YAP_Term geometry1_yap = YAP_ARG2;
    YAP_Term geometry2_yap = YAP_ARG3;
    YAP_Term diff_res = YAP_ARG4;

    char *geometry1_WKT = extract_WKT_from_points(geometry1_yap);
    char *geometry2_WKT = extract_WKT_from_points(geometry2_yap);

    // execute_PostGIS_function(conn, "ST_NPoints", int_ring)
    char *geometry1 = execute_PostGIS_function(conn, "ST_GeomFromText", geometry1_WKT);
    char *geometry2 = execute_PostGIS_function(conn, "ST_GeomFromText", geometry2_WKT);

    free(geometry1_WKT);
    free(geometry2_WKT);

    char *geometry_diff = executeS_PostGIS_function(conn, "ST_Difference", geometry1, geometry2);
    int n_int_rings = atoi(execute_PostGIS_function(conn, "ST_NumInteriorRings", geometry_diff));

    YAP_Term geometry_diff_yap = extract_values(conn, geometry_diff, n_int_rings);

    if (!YAP_Unify(diff_res, geometry_diff_yap))
    {
        return FALSE;
    }
    return TRUE;
}

// st_rotate(ConnHandler, Geometry, RadiantsAngleRotation, RotatedGeometry).
static int c_st_rotate(void)
{
    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);

    YAP_Term geometry_yap = YAP_ARG2;
    int rads = YAP_IntOfTerm(YAP_ARG3);
    YAP_Term rot_res = YAP_ARG4;

    char *geometry_WKT = extract_WKT_from_points(geometry_yap);
    char *geometry = execute_PostGIS_function(conn, "ST_GeomFromText", geometry_WKT);

    free(geometry_WKT);

    char *geometry_rotation = executeN_PostGIS_function(conn, "ST_Rotate", geometry, rads);
    int n_int_rings = atoi(execute_PostGIS_function(conn, "ST_NumInteriorRings", geometry_rotation));

    YAP_Term res_term = extract_values(conn, geometry_rotation, n_int_rings);

    if (!YAP_Unify(rot_res, res_term))
    {
        return FALSE;
    }
    return TRUE;
}

// st_translate(ConnHandler, Geometry, (DeltaX, DeltaY), TranslatedGeometry).
static int c_st_translate(void)
{
    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);

    YAP_Term geometry_yap = YAP_ARG2;
    YAP_Term translation_vector = YAP_ARG3;
    YAP_Term trans_res = YAP_ARG4;

    if (!YAP_IsPairTerm(translation_vector) || !YAP_IsFloatTerm(YAP_HeadOfTerm(translation_vector)) ||
        !YAP_IsFloatTerm(YAP_TailOfTerm(translation_vector)))
    {
        YAP_Error(0, translation_vector, "Expected a pair of floats");
        return FALSE;
    }

    // int deltaX = YAP_IntOfTerm(YAP_ARG3);
    // int deltaY = YAP_IntOfTerm(YAP_ARG4);
    float deltaX = YAP_IntOfTerm(YAP_HeadOfTerm(translation_vector));
    float deltaY = YAP_IntOfTerm(YAP_TailOfTerm(translation_vector));

    char *geometry_WKT = extract_WKT_from_points(geometry_yap);
    char *geometry = execute_PostGIS_function(conn, "ST_GeomFromText", geometry_WKT);

    free(geometry_WKT);

    char *geometry_translate = executeFF_PostGIS_function(conn, "ST_Translate", geometry, deltaX, deltaY);
    int n_int_rings = atoi(execute_PostGIS_function(conn, "ST_NumInteriorRings", geometry_translate));

    YAP_Term res_term = extract_values(conn, geometry_translate, n_int_rings);

    if (!YAP_Unify(trans_res, res_term))
    {
        return FALSE;
    }
    return TRUE;
}

// yap_predicate_to_WTK(YapPointsList, WTKString).
static int yap_predicate_to_WTK(void)
{
    YAP_Term points_list_yap = YAP_ARG1;
    YAP_Term result = YAP_ARG2;

    char *geometry_WKT = extract_WKT_from_points(points_list_yap);
    YAP_Term WKTString_yap = YAP_MkAtomTerm(YAP_LookupAtom(geometry_WKT ? geometry_WKT : "NULL"));
    free(geometry_WKT);

    if (!YAP_Unify(result, WKTString_yap))
    {
        return FALSE;
    }
    return TRUE;
}