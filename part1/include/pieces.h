#ifndef PIECES_H
#define PIECES_H

int createPiecesTable(PGconn *conn);
int populatePiecesTable(PGconn *conn);
int testPiecesTable(PGconn *conn); 
int countPiecesTable(PGconn *conn);

#endif    /* PIECES_H */