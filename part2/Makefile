CC=gcc
CFLAGS=-w -c -shared -fPIC 
POSTGFLAGS=-I $(shell pg_config --includedir) -L $(shell pg_config --libdir) -I ./
YAPFLAGS=-I/usr/local/include/Yap
LDFLAGS=-shared
LIBS=-lpq
# gcc -shared -o yap2postgres.so bin/yap2postgres.o bin/PostGIS_utils.o -lpq
C_SRC=$(wildcard ./*.c)
OBJ=$(subst .c,.o,$(subst ./,./bin/,$(C_SRC)))
# SO_FILE=$(subst .c,.so,$(C_SRC))

SO_FILE=$(subst .c,.so,$(filter-out $(wildcard ./*util*), $(wildcard ./*.c)))

all: binFolder compile

compile: $(SO_FILE)

$(SO_FILE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ bin/$(@:.so=.o) bin/PostGIS_utils.o $(LIBS)

$(OBJ): $(C_SRC)
	$(CC) $(CFLAGS) -o $@ $(subst bin/,,$(@:.o=.c)) $(POSTGFLAGS) $(YAPFLAGS)

binFolder:
	@ mkdir -p ./bin/

clean:
	@ rm -f *.o *.so
	@ rm -rf ./bin/

.PHONY: all clean binFolder compile