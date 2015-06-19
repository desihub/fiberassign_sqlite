#include "fa.h"
#include "io.h"
#include "constants.h"

#include "stdlib.h"
#include "stdio.h"

/**
 * This program takes input arguments:
 * 1 -- name of binary file with fiber information.
 * 2 -- name of binary file with target information.
 * 3 -- name of output ASCII file to write ra, dec, red and target type of
 * observed targets.
 * All input arguments are required.
 * In this version the files 1 and 2 are overwritten towards the end
 * and any previous attempts at fiber assignment are wiped out.
 */

int 
main (int argc, char **argv)
{
    fiber *fb = (fiber*)malloc(sizeof(fiber)*NFIBER);
    target *tg = (target*)malloc(sizeof(target)*NTARGET);
    printf("reading fibers\n");
    int Nfb = read_fibers(fb, argv[1]);
    for (int i = 0; i < Nfb; i++) {
        wipe_fiber(fb+i);
    }
    printf("Nfb = %d\n", Nfb);
    printf("reading targets\n");
    int Ntg = read_targets(tg, argv[2]);
    for (int i = 0; i < Ntg; i++) {
        wipe_target(tg+i);
    }
    for (int i = 0; i < Ntg; i++) {
        if(tg->N > tg->nobs) printf("More1 ");
    }
    stat_fibers(fb, Nfb);
    printf("sorting targets\n");
    for (int i = 0; i < Nfb; i++) {
        sort_fiber(fb+i, tg);
    }
    for (int i = 0; i < Ntg; i++) {
        if(tg->N > tg->nobs) printf("More2 ");
    }
    printf("assigning targets\n");
    for (int i = 0; i < Nfb; i++) {
        assign_fiber(fb, tg, i);
    }
    for (int i = 0; i < Ntg; i++) {
        if(tg->N > tg->nobs) printf("More3 ");
    }
    stat_targets(tg, Ntg);
    stat_fibers(fb, Nfb);
    printf("switching fibers\n");
    switch_fiber(fb, tg, Nfb);
    for (int i = 0; i < Ntg; i++) {
        if(tg->N > tg->nobs) printf("More4 ");
    }
    stat_targets(tg, Ntg);
    print_observed_targets(tg, Ntg, argv[3]); 
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    write_targets(tg, Ntg, argv[2]);
    write_fibers(fb, Nfb, argv[1]);
    free(fb);
    free(tg);
    return 0;
}
