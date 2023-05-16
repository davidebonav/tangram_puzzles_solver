#ifndef UTILS_H
#define UTILS_H

#include <libpq-fe.h>
#include "Yap/YapInterface.h"
#include <string.h>

#define POLYG_OID 46115

#include "postGIS_utils.h"

static bool finish_with_error(PGconn *conn, PGresult *res);
PGresult *exec_sql(PGconn *conn, char *sql);
void execute_PostGIS_function(PGconn *conn, char *sql, char *output);

// Extract YAP TERM
YAP_Term extract_point(PGconn *conn, char *point);
YAP_Term extract_ring(PGconn *conn, char *ring);
YAP_Term extract_polygon(PGconn *conn, char *polygon);
YAP_Term extract_geometry(PGconn *conn, char *geometry);

// EXTRACTY WKT
char *extract_WKT_from_points(YAP_Term points_list);
bool create_WKT_points_string(YAP_Term points_list, char *WKT_string);

#endif