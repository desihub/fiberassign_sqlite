#include "iofits.h"
#include "constants.h"
#include "fa.h"
#include "tg.h"
#include "io.h"

/**
 * This program takes input arguments:
 * 1 -- name of binary file with fiber information.
 * 2 -- name of binary file with target information.
 * 3 -- integer telling which plate to write.
 * 4 -- name of output fits file.
 * It will write the information about fiber assignment on a plate to a fits
 * file.
 */

int
main (int argc, char **argv)
{
    fiber *fb = (fiber*)malloc(sizeof(fiber)*NFIBER);
    target *tg = (target*)malloc(sizeof(target)*NTARGET);
    read_fibers(fb, argv[1]);
    read_targets(tg, argv[2]);
    fa_write(argv[4], fb, tg, atoi(argv[3]));
    return 0;
}
