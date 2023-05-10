#ifndef UTILS_H
#define UTILS_H

#include <libpq-fe.h>
#include "Yap/YapInterface.h"
#include <string.h>

#define POLYG_OID 46115

YAP_Term extract_values(PGconn *conn, char* geometry, int n_int_rings);
char *execute_PostGIS_function(PGconn *conn, char *function_name, char *geometry);
char *extract_WKT_from_points(YAP_Term points_list);

#endif