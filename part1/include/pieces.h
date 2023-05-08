#ifndef PIECES_H
#define PIECES_H

#include <libpq-fe.h>

#include <stdio.h>
#include <stdlib.h>
#include <db_utilities.h>

int createPiecesTable(PGconn *conn);
int populatePiecesTable(PGconn *conn);
int createAndPopulatePiecesTable(PGconn *conn);
int printPiecesTable(PGconn *conn); 
int countRowPiecesTable(PGconn *conn);

#endif    /* PIECES_H */