CC=gcc
CFLAGS=-w -c -shared -fPIC 
POSTGFLAGS=-I $(shell pg_config --includedir) -L $(shell pg_config --libdir)
YAPFLAGS=-I/usr/local/include/Yap
# -I $(shell pg_config --includedir) -L $(shell pg_config --libdir)
# CFLAGS = -c -W -Wall -pedantic -shared -fPIC -I $(shell pg_config --includedir) -L $(shell pg_config --libdir)
LDFLAGS=-shared
LIBS=-lpq

C_SRC=$(wildcard ./*.c)
OBJ=$(subst .c,.o,$(subst ./,./bin/,$(C_SRC)))
SO_FILE=$(subst .c,.so,$(C_SRC))

all: clean binFolder compile

binFolder:
	@ mkdir -p ./bin/

compile: $(SO_FILE)

$(SO_FILE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJ): $(C_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(POSTGFLAGS) $(YAPFLAGS)

clean:
	rm -f *.o *.so
	@ rm -rf ./bin/

.PHONY: all clean

#     gcc -w -c -shared -fPIC ../lesson4/ex2/yap2mysql.c `mysql_config --cflags --libs`; ld -shared -o yap2mysql.so yap2mysql.o
#     gcc -w -c -shared -fPIC yap2mysql.c `mysql_config --cflags --libs`; ld -shared -o yap2mysql.so yap2mysql.o `mysql_config --cflags --libs`
#         il -w permette di silenziare i warnings

#     per caricare i predicati
#         load_foreign_files(['yap2mysql'],[], init_predicates).
#consult(...)
     