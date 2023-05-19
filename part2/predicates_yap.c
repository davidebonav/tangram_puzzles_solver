#include "Yap/YapInterface.h"
#include <libpq-fe.h>
#include "postGIS_utils.h"

void init_predicates();
static int c_st_rotate_v1(void);
static int c_st_rotate_v2(void);
static int c_st_difference(void);
static int c_st_translate(void);
static int c_yap_predicate_to_WKT(void);
static int c_st_function(void);

void init_predicates()
{
  YAP_UserCPredicate("st_difference", c_st_difference, 4);
  YAP_UserCPredicate("st_rotate", c_st_rotate_v1, 4);
  YAP_UserCPredicate("st_rotate", c_st_rotate_v2, 5);
  YAP_UserCPredicate("st_translate", c_st_translate, 4);
  YAP_UserCPredicate("yap_predicate_to_WKT", c_yap_predicate_to_WKT, 2);
  YAP_UserCPredicate("st_function", c_st_function, 3);
}

static int c_st_difference(void)
{
  PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);
  YAP_Term geometry1_yap = YAP_ARG2;
  YAP_Term geometry2_yap = YAP_ARG3;
  YAP_Term diff_res = YAP_ARG4;

  char *geometry1_WKT = extract_WKT_from_points(geometry1_yap);
  char *geometry2_WKT = extract_WKT_from_points(geometry2_yap);

  char geometry1[1024], geometry2[1024], sql[1024];

  sprintf(sql, "SELECT ST_GeomFromText('%s')", geometry1_WKT);
  execute_PostGIS_function(conn, sql, geometry1);
  sprintf(sql, "SELECT ST_GeomFromText('%s')", geometry2_WKT);
  execute_PostGIS_function(conn, sql, geometry2);

  free(geometry1_WKT);
  free(geometry2_WKT);

  char geometry_diff[1024];

  sprintf(sql, "SELECT ST_Difference('%s', '%s')", geometry1, geometry2);
  execute_PostGIS_function(conn, sql, geometry_diff);

  YAP_Term geometry_diff_yap = extract_geometry(conn, geometry_diff);

  if (!YAP_Unify(diff_res, geometry_diff_yap))
    return FALSE;

  return TRUE;
}

static int c_st_rotate_v1(void)
{
  PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);

  YAP_Term geometry_yap = YAP_ARG2;
  double rads = YAP_FloatOfTerm(YAP_ARG3);
  YAP_Term rot_res = YAP_ARG4;

  char *geometry_WKT = extract_WKT_from_points(geometry_yap);

  char geometry[1024], sql[1024];
  sprintf(sql, "SELECT ST_GeomFromText('%s')", geometry_WKT);
  execute_PostGIS_function(conn, sql, geometry);

  free(geometry_WKT);

  char geometry_rotation[1024];
  sprintf(sql, "SELECT ST_Rotate('%s', radians(%f), ST_Centroid('%s'))", geometry, rads, geometry);
  execute_PostGIS_function(conn, sql, geometry_rotation);

  YAP_Term res_term = extract_geometry(conn, geometry_rotation);
  if (!YAP_Unify(rot_res, res_term))
    return FALSE;
  return TRUE;
}

static int c_st_rotate_v2(void)
{
  PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);

  YAP_Term geometry_yap = YAP_ARG2;
  double rads = YAP_FloatOfTerm(YAP_ARG3),
         x = YAP_FloatOfTerm(YAP_HeadOfTerm(YAP_ARG4)),
         y = YAP_FloatOfTerm(YAP_TailOfTerm(YAP_ARG4));
  YAP_Term rot_res = YAP_ARG5;

  char *geometry_WKT = extract_WKT_from_points(geometry_yap);

  char geometry[1024], sql[1024];
  sprintf(sql, "SELECT ST_GeomFromText('%s')", geometry_WKT);
  execute_PostGIS_function(conn, sql, geometry);

  free(geometry_WKT);

  char geometry_rotation[1024];
  sprintf(sql, "SELECT ST_Rotate('%s', radians(%f), 'POINT(%f %f)')", geometry, rads, x, y);
  execute_PostGIS_function(conn, sql, geometry_rotation);

  YAP_Term res_term = extract_geometry(conn, geometry_rotation);
  if (!YAP_Unify(rot_res, res_term))
    return FALSE;
  return TRUE;
}

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
  char geometry[1024], sql[1024];
  sprintf(sql, "SELECT ST_GeomFromText('%s')", geometry_WKT);
  execute_PostGIS_function(conn, sql, geometry);

  free(geometry_WKT);

  char geometry_translate[1024];
  sprintf(sql, "SELECT ST_Translate('%s', %f, %f)", geometry, deltaX, deltaY);
  execute_PostGIS_function(conn, sql, geometry_translate);

  YAP_Term res_term = extract_geometry(conn, geometry_translate);

  if (!YAP_Unify(trans_res, res_term))
    return FALSE;
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
    return FALSE;

  return TRUE;
}

static int c_st_function(void)
{
  PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG1);
  char *sql = YAP_AtomName(YAP_AtomOfTerm(YAP_ARG2));
  YAP_Term function_res = YAP_ARG3;
  char output[512];

  execute_PostGIS_function(conn, sql, output);

  YAP_Term res_term = YAP_MkAtomTerm(YAP_LookupAtom(output));
  if (!YAP_Unify(function_res, res_term))
    return FALSE;
  return TRUE;
}

void print_term_type(YAP_Term t)
{
  if (YAP_IsAtomTerm(t))
  {
    printf("Atom\n");
  }
  else if (YAP_IsVarTerm(t))
  {
    printf("Variable\n");
  }
  else if (YAP_IsIntTerm(t))
  {
    printf("Integer\n");
  }
  else if (YAP_IsFloatTerm(t))
  {
    printf("Float\n");
  }
  else if (YAP_IsPairTerm(t))
  {
    printf("Pair\n");
  }
  else if (YAP_IsAtomTerm(t))
  {
    printf("Atomic\n");
  }
  else if (YAP_IsCompoundTerm(t))
  {
    printf("Compound\n");
  }
  else
  {
    printf("Unknown\n");
  }
}