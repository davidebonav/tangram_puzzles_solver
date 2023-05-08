#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <libpq-fe.h>

#include <stdio.h>
#include <stdlib.h>
#include <db_utilities.h>

int createSolutionsTable(PGconn *conn);
int populateSolutionsTable(PGconn *conn);
int createAndPopulateSolutionsTable(PGconn *conn);
int printSolutionsTable(PGconn *conn); 
int countRowSolutionsTable(PGconn *conn);

#endif    /* SOLUTIONS_H */