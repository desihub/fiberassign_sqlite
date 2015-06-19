/**
 * This code will take input target catalogue and fiber positions and will
 * reformat them so that they are more convenient to use in later parts of the
 * pipeline. It will also create SQLite script files to make targets and fibers
 * into rstar sqlite databases.
 * Arguments:
 * 1 -- name of input binary target file.
 * 2 -- name of input ASCII fiberpos file.
 * 3 -- name of input ASCII plate center file.
 * 4 -- name of output target SQLite script file.
 * 5 -- name of output fiber SQLite script file.
 * 6 -- name of output target SQLite database.
 * 7 -- name of output fiber SQLite database.
 * 8 -- name of output binary target file.
 * 9 -- name of output binary fiber file.
 */

#include "constants.h"
#include "tg.h"
#include "fa.h"
#include "io.h"
#include "db.h"
#include "decals.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

int
main (int argc, char **argv)
{
    char tg_file[2000];
    strcpy(tg_file, argv[1]);
    char fb_file[2000];
    strcpy(fb_file, argv[2]);
    char pl_file[2000];
    strcpy(pl_file, argv[3]);
    char tg_sql[2000];
    strcpy(tg_sql, argv[4]);
    char fb_sql[2000];
    strcpy(fb_sql, argv[5]);
    char tg_db[2000];
    strcpy(tg_db, argv[6]);
    char fb_db[2000];
    strcpy(fb_db, argv[7]);
    char tg_bin[2000];
    strcpy(tg_bin, argv[8]);
    char fb_bin[2000];
    strcpy(fb_bin, argv[9]);

    target *tg = (target*)malloc(sizeof(target)*NTARGET);
    fiber *fb = (fiber*)malloc(sizeof(fiber)*NFIBER);

    printf("initialize targrets\n");
    int Ntg = init_targets_decals(tg, tg_file);
    printf("initialize fibers\n");
    int Nfb = init_fibers(fb, fb_file, pl_file);

    /*< Whichever is greater out of Ntg and Nfb */
    int NMAX = Ntg > Nfb ? Ntg : Nfb;

    double *x = (double*)malloc(sizeof(double)*NMAX);
    double *y = (double*)malloc(sizeof(double)*NMAX);
    double *z = (double*)malloc(sizeof(double)*NMAX);
   
    /*< Make target SQLite script */
    for (int i = 0; i < Ntg; i++) {
        x[i] = tg[i].x;
        y[i] = tg[i].y;
        z[i] = tg[i].z;
    }
    create_sql_file(tg_sql, tg_db, x, y, z, Ntg);
 
    /*< Make fibers SQLite script */
    for (int i = 0; i < Nfb; i++) {
        x[i] = fb[i].x;
        y[i] = fb[i].y;
        z[i] = fb[i].z;
    }
    create_sql_file(fb_sql, fb_db, x, y, z, Nfb);

    write_targets(tg, Ntg, tg_bin);
    write_fibers(fb, Nfb, fb_bin);

    return 0;
} 
