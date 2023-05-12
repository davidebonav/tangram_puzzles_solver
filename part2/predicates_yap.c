#include "Yap/YapInterface.h"
#include <libpq-fe.h>
#include "postGIS_utils.h"

void init_predicates();
static int c_st_rotate(void);
static int c_st_difference(void);
static int c_st_translate(void);
static int c_yap_predicate_to_WKT(void);

void init_predicates()
{
    YAP_UserCPredicate("st_difference", c_st_difference, 4);
    YAP_UserCPredicate("st_rotate", c_st_rotate, 4);
    YAP_UserCPredicate("st_translate", c_st_translate, 4);
    YAP_UserCPredicate("yap_predicate_to_WKT", c_yap_predicate_to_WKT, 2);
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

    char geometry1[1024], geometry2[1024];
    execute_PostGIS_function(conn, "ST_GeomFromText", geometry1_WKT,geometry1);
    execute_PostGIS_function(conn, "ST_GeomFromText", geometry2_WKT,geometry2);

    free(geometry1_WKT);
    free(geometry2_WKT);

    char geometry_diff[1024],n_int_rings[16];
    executeS_PostGIS_function(conn, "ST_Difference", geometry1, geometry2,geometry_diff);
    execute_PostGIS_function(conn, "ST_NumInteriorRings", geometry_diff, n_int_rings);

    YAP_Term geometry_diff_yap = extract_values(conn, geometry_diff, atoi(n_int_rings));

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
    double rads = YAP_FloatOfTerm(YAP_ARG3);
    YAP_Term rot_res = YAP_ARG4;

    char *geometry_WKT = extract_WKT_from_points(geometry_yap);
    char geometry[1024];
    execute_PostGIS_function(conn, "ST_GeomFromText", geometry_WKT, geometry);

    free(geometry_WKT);

    char geometry_rotation[1024], n_int_rings[16];
    executeD_PostGIS_function(conn, "ST_Rotate", geometry, rads,geometry_rotation);
    execute_PostGIS_function(conn, "ST_NumInteriorRings", geometry_rotation, n_int_rings);

    YAP_Term res_term = extract_values(conn, geometry_rotation, atoi(n_int_rings));

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

    // print_term_type(translation_vector);

    if (!YAP_IsPairTerm(translation_vector) || !YAP_IsFloatTerm(YAP_HeadOfTerm(translation_vector)) ||
        !YAP_IsFloatTerm(YAP_TailOfTerm(translation_vector)))
    {
        YAP_Error(0, translation_vector, "Expected a pair of floats");
        return FALSE;
    }

    // int deltaX = YAP_IntOfTerm(YAP_ARG3);
    // int deltaY = YAP_IntOfTerm(YAP_ARG4);
    float deltaX = YAP_FloatOfTerm(YAP_HeadOfTerm(translation_vector));
    float deltaY = YAP_FloatOfTerm(YAP_TailOfTerm(translation_vector));

    char *geometry_WKT = extract_WKT_from_points(geometry_yap);
    char geometry[1024]; 
    execute_PostGIS_function(conn, "ST_GeomFromText", geometry_WKT,geometry);

    free(geometry_WKT);

    char geometry_translate[1024], n_int_rings[16];
    executeFF_PostGIS_function(conn, "ST_Translate", geometry, deltaX, deltaY,geometry_translate);
    execute_PostGIS_function(conn, "ST_NumInteriorRings", geometry_translate, n_int_rings);

    YAP_Term res_term = extract_values(conn, geometry_translate,  atoi(n_int_rings));

    if (!YAP_Unify(trans_res, res_term))
    {
        return FALSE;
    }
    return TRUE;
}

// yap_predicate_to_WKT(YapPointsList, WKTString).
static int c_yap_predicate_to_WKT(void)
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

void print_term_type(YAP_Term t) {
  if (YAP_IsAtomTerm(t)) {
    printf("Atom\n");
  } else if (YAP_IsVarTerm(t)) {
    printf("Variable\n");
  } else if (YAP_IsIntTerm(t)) {
    printf("Integer\n");
  } else if (YAP_IsFloatTerm(t)) {
    printf("Float\n");
  } else if (YAP_IsPairTerm(t)) {
    printf("Pair\n");
  } else if (YAP_IsAtomTerm(t)) {
    printf("Atomic\n");
  } else if (YAP_IsCompoundTerm(t)) {
    printf("Compound\n");
  } else {
    printf("Unknown\n");
  }
}