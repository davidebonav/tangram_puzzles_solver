#ifndef PUZZLES_H
#define PUZZLES_H

#include <libpq-fe.h>

#include <stdio.h>
#include <stdlib.h>
#include <db_utilities.h>

int createPuzzlesTable(PGconn *conn);
int populatePuzzlesTable(PGconn *conn);
int createAndPopulatePuzzlesTable(PGconn *conn);
int printPuzzlesTable(PGconn *conn); 
int countRowPuzzlesTable(PGconn *conn);

#endif    /* PUZZLES_H */