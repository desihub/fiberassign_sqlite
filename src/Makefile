EXE_DIR=../bin/
CC=gcc
CFLAGS=-Wall -O3 --std=c11
LFLAGS=-lm -ldl -pthread -lgomp -lcfitsio
OBJECTS=tg.o fa.o db.o io.o iofits.o decals.o

all: $(EXE_DIR)fiber_reach.exe $(EXE_DIR)fiber_assign.exe $(EXE_DIR)init_data.exe $(EXE_DIR)write_plate.exe

test: $(EXE_DIR)test.exe

sqlite3: sqlite3.o

$(EXE_DIR)init_data.exe: init_data.o sqlite3.o $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXE_DIR)init_data.exe init_data.o sqlite3.o $(OBJECTS)

$(EXE_DIR)fiber_reach.exe: fiber_reach.o sqlite3.o $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXE_DIR)fiber_reach.exe fiber_reach.o sqlite3.o $(OBJECTS)

$(EXE_DIR)fiber_assign.exe: fiber_assign.o $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXE_DIR)fiber_assign.exe fiber_assign.o sqlite3.o $(OBJECTS)

$(EXE_DIR)write_plate.exe: write_plate.o sqlite3.o $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXE_DIR)write_plate.exe write_plate.o sqlite3.o $(OBJECTS)

$(EXE_DIR)test.exe: test.o $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXE_DIR)test.exe test.o sqlite3.o $(OBJECTS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

sqlite3.o:
	$(CC) -O3 -c sqlite3.c -DSQLITE_ENABLE_RTREE=1

test.o: test.c
	$(CC) $(CFLAGS) -c test.c 

clean:
	rm -rf init_data.o fiber_reach.o fiber_assign.o write_plate.o $(OBJECTS) $(EXE_DIR)*.exe

cleansqlite3:
	rm -rf sqlite3.o 
