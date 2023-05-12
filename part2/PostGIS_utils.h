#ifndef UTILS_H
#define UTILS_H

#include <libpq-fe.h>
#include "Yap/YapInterface.h"
#include <string.h>

#define POLYG_OID 46115

void execute_PostGIS_function(PGconn *conn, char *function_name, char *geometry, char* output);
void executeN_PostGIS_function(PGconn *conn, char *function_name, char *geometry, int n, char* output);
void executeD_PostGIS_function(PGconn *conn, char *function_name, char *geometry, double d, char* output);
void executeS_PostGIS_function(PGconn *conn, char *function_name, char *geometry1, char *geometry2, char* output);
void executeFF_PostGIS_function(PGconn *conn, char *function_name, char *geometry1, double x, double y, char* output);
YAP_Term extract_points_from_ring(PGconn *conn, char *ring, int n_points);
YAP_Term extract_values(PGconn *conn, char *geometry, int n_int_rings);
char *extract_WKT_from_points(YAP_Term points_list);
void create_WKT_points_string(YAP_Term points_list, char *WKT_string);

#endif