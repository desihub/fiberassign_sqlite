/**
 * This program takes input arguments:
 * 1 -- name of file with sqlite3 database of targets.
 * 2 -- name of binary file with fiber information.
 * All arguments are required.
 * Please beware that file 2 will be overwritten.
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "fa.h"
#include "db.h"
#include "io.h"

#include "sqlite3.h"

int
main(int argc, char **argv) {
 
    char tg_sql[2000];
    strcpy(tg_sql, argv[1]);
    char fb_file[2000];
    strcpy(fb_file, argv[2]);

    fiber *fb;
    fb = (fiber*)malloc(sizeof(fiber)*NFIBER);

    printf("read database\n");
    sqlite3 *db;
    sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL);
    printf("reading database\n");
    printf("%s\n", sqlite3_errmsg(db));
    read_db(db, tg_sql);
    printf("%s\n", sqlite3_errmsg(db));
    printf("read fibers\n");
    int Nfb = read_fibers(fb, fb_file);
    printf("find fiber reach\n");
//    #pragma omp parallel for 
    for (int i = 0; i < Nfb; i++) {
        if (i%100000 == 0) printf("%d \n", i);
         fiber_reach(db, PATROL_RAD*MM_TO_RAD, fb+i);
    }
    write_fibers(fb, Nfb, fb_file);
    free(fb); 
    sqlite3_close(db);

    return 0;
}
