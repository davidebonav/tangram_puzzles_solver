CC=gcc
CFLAGS=-w -c -shared -fPIC 
POSTGFLAGS=-I $(shell pg_config --includedir) -L $(shell pg_config --libdir)
YAPFLAGS=-I/usr/local/include/Yap
# -I $(shell pg_config --includedir) -L $(shell pg_config --libdir)
# CFLAGS = -c -W -Wall -pedantic -shared -fPIC -I $(shell pg_config --includedir) -L $(shell pg_config --libdir)
LDFLAGS=-shared
LIBS=-lpq

all: clean yap2postgres.so

yap2postgres.so: yap2postgres.o
	$(CC) $(LDFLAGS) -o yap2postgres.so yap2postgres.o $(LIBS)

yap2postgres.o: yap2postgres.c
	$(CC) $(CFLAGS) -o yap2postgres.o yap2postgres.c $(POSTGFLAGS) $(YAPFLAGS)

clean:
	rm -f yap2postgres.o yap2postgres.so

.PHONY: all clean

#     gcc -w -c -shared -fPIC ../lesson4/ex2/yap2mysql.c `mysql_config --cflags --libs`; ld -shared -o yap2mysql.so yap2mysql.o
#     gcc -w -c -shared -fPIC yap2mysql.c `mysql_config --cflags --libs`; ld -shared -o yap2mysql.so yap2mysql.o `mysql_config --cflags --libs`
#         il -w permette di silenziare i warnings

#     per caricare i predicati
#         load_foreign_files(['yap2mysql'],[], init_predicates).
#consult(...)
     