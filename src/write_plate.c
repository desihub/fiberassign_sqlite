#include "string.h"
#include "stdlib.h"

#include "iofits.h"
#include "constants.h"
#include "fa.h"
#include "tg.h"
#include "io.h"

/**
 * This program takes input arguments:
 * 1 -- name of binary file with fiber information.
 * 2 -- name of binary file with target information.
 * 3 -- name of output fits file.
 * It will write the information about fiber assignment for all nonempty plates
 * into series of fits files.
 */

int
main (int argc, char **argv)
{
    fiber *fb = (fiber*)malloc(sizeof(fiber)*NFIBER);
    target *tg = (target*)malloc(sizeof(target)*NTARGET);
    int Nfb = read_fibers(fb, argv[1]);
    read_targets(tg, argv[2]);
    printf("%d\n", Nfb/MAXFIBER);
    for (int i = 0; i < Nfb/MAXFIBER; i++) {
        char fitsfile[100];
        sprintf(fitsfile,"%s_%d.fits", argv[3], i);
        fa_write(fitsfile, fb, tg, i);
    }
    free(fb);
    free(tg);
    return 0;
}
