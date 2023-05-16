#include "Yap/YapInterface.h"
#include <libpq-fe.h>
#include "PostGIS_utils.h"

void init_predicates();

static int c_db_connect(void);
static int c_db_disconnect(void);
// static int c_db_assert(void);
static int c_db_query(void);
static int c_db_arity(void);

static int c_db_row_start(void);
static int c_db_row_continue(void);
static int c_db_row_cut(void);

typedef struct
{
    YAP_Term current_row;
} c_db_row_type;

c_db_row_type *c_db_row_data;

void init_predicates()
{
    YAP_UserCPredicate("db_connect", c_db_connect, 5);
    YAP_UserCPredicate("db_disconnect", c_db_disconnect, 1);
    // YAP_UserCPredicate("db_assert" , c_db_assert, 3);
    YAP_UserCPredicate("db_query", c_db_query, 3);
    YAP_UserBackCutCPredicate("db_row", c_db_row_start, c_db_row_continue, c_db_row_cut, 3, 1);

    YAP_UserCPredicate("db_arity", c_db_arity, 3);
}

static int c_db_connect(void)
{
    YAP_Term
        arg_host = YAP_ARG1,
        arg_user = YAP_ARG2,
        arg_passwd = YAP_ARG3,
        arg_database = YAP_ARG4,
        arg_conn = YAP_ARG5;

    char
        *host = YAP_AtomName(YAP_AtomOfTerm(arg_host)),
        *user = YAP_AtomName(YAP_AtomOfTerm(arg_user)),
        *passwd = YAP_AtomName(YAP_AtomOfTerm(arg_passwd)),
        *database = YAP_AtomName(YAP_AtomOfTerm(arg_database));

    char connectionInfo[1024];
    snprintf(
        connectionInfo,
        sizeof(connectionInfo),
        "user=%s password=%s dbname=%s host=%s port=5432",
        user, passwd, database, host);

    PGconn *conn = PQconnectdb(connectionInfo);
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed...\n");
        finish_with_error(conn, NULL);
    }

    if (!YAP_Unify(arg_conn, YAP_MkIntTerm((long)conn)))
    {
        return FALSE;
    }
    return TRUE;
}

static int c_db_disconnect(void)
{
    YAP_Term arg_conn = YAP_ARG1;

    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);

    PQfinish(conn);
    return TRUE;
}

static int c_db_query(void)
{
    // printf("Entering in db_query...\n");

    YAP_Term
        arg_conn = YAP_ARG1,
        arg_sql = YAP_ARG2,
        arg_result_set = YAP_ARG3;

    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);
    char *sql = YAP_AtomName(YAP_AtomOfTerm(arg_sql));

    PGresult *result_set = PQexec(conn, sql);
    if (PQresultStatus(result_set) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, result_set);
    }

    if (!YAP_Unify(arg_result_set, YAP_MkIntTerm((long)result_set)))
    {
        return FALSE;
    }

    return TRUE;
}

static int c_db_row_start(void)
{
    // printf("Entering in db_row_start...\n");

    YAP_PRESERVE_DATA(c_db_row_data, c_db_row_type);
    c_db_row_data->current_row = YAP_MkIntTerm(0);

    return c_db_row_continue();
}

static int c_db_row_continue(void)
{
    // printf("Entering in db_row_continue...\n");

    PGconn *conn = (PGconn *)YAP_IntOfTerm(YAP_ARG3);
    PGresult *res_set;
    int i, arity, n_rows;
    char *value;

    YAP_PRESERVED_DATA(c_db_row_data, c_db_row_type);
    i = YAP_IntOfTerm(c_db_row_data->current_row);
    c_db_row_data->current_row = YAP_MkIntTerm(i + 1);

    res_set = (PGresult *)YAP_IntOfTerm(YAP_ARG1);

    arity = PQnfields(res_set);
    n_rows = PQntuples(res_set);

    if (i < n_rows)
    {
        YAP_Term head, list = YAP_ARG2;
        for (int j = 0; j < arity; j++)
        {
            head = YAP_HeadOfTerm(list);
            list = YAP_TailOfTerm(list);
            value = PQgetvalue(res_set, i, j);
            // printf("%s - %d\n", value, PQftype(res_set, j));

            YAP_Term out_term;
            if (PQftype(res_set, j) == (Oid)POLYG_OID)
                out_term = extract_geometry(conn, value);

            else
                out_term = YAP_MkAtomTerm(YAP_LookupAtom(value ? value : "NULL"));

            if (!YAP_Unify(head, out_term))
                return FALSE;
        }
        return TRUE;
    }

    YAP_cut_fail();
}

static int c_db_row_cut(void)
{
    // printf("entering in db_row_cut...\n");
    PGresult *res_set = (PGresult *)YAP_IntOfTerm(YAP_ARG1);
    PQclear(res_set);
    return TRUE;
}

// db_arity(ConnHandler, RelationName, Arity),
static int c_db_arity(void)
{
    YAP_Term
        arg_conn = YAP_ARG1,
        arg_rel_name = YAP_ARG2,
        arg_arity = YAP_ARG3;

    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);
    char
        *rel_name = YAP_AtomName(YAP_AtomOfTerm(arg_rel_name)),
        *sql = (char *)malloc(1024 * sizeof(char));
    int arity;

    sprintf(sql, "select * from %s limit 0", rel_name);
    PGresult *res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing query : %s\n", sql);
        finish_with_error(conn, res);
    }

    arity = PQnfields(res);

    if (!YAP_Unify(arg_arity, YAP_MkIntTerm((int)arity)))
    {
        return FALSE;
    }

    free(sql);
    PQclear(res);
    return TRUE;
}