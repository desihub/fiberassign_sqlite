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
 */

int 
main (int argc, char **argv)
{
    fiber *fb = (fiber*)malloc(sizeof(fiber)*NFIBER);
    target *tg = (target*)malloc(sizeof(target)*NTARGET);
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    printf("reading fibers\n");
    int Nfb = read_fibers(fb, argv[1]);
    printf("Nfb = %d\n", Nfb);
    printf("reading targets\n");
    int Ntg = read_targets(tg, argv[2]);
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    stat_fibers(fb, Nfb);
    printf("sorting targets\n");
    for (int i = 0; i < Nfb; i++) {
        sort_fiber(fb+i, tg);
    }
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    printf("assigning targets\n");
    for (int i = 0; i < Nfb; i++) {
        assign_fiber(fb, tg, i);
    }
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    stat_targets(tg, Ntg);
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    stat_fibers(fb, Nfb);
    print_observed_targets(tg, Ntg, argv[3]); 
    for (int i = 0; i < 10; i++) {
        printf("%d ", tg[i].N);
    }
    printf("#\n");
    return 0;
}
