/**
 * Functions related to I/O of fits files.
 */

#ifndef _IOFITS_
#define _IOFITS_

#include "fitsio.h"
#include "fa.h"
#include "tg.h"

/**
 * Output the information about fiber assignment for a plate. fb points to the
 * first fiber on a plate (assuming there are NFIBER number of fibers; filename
 * is the output FITS binary file. Nplate is the index of a plate we want to
 * write.
 */

void fa_write (const char *filename, fiber *fb, target *tg, int Nplate);

/**
 * Read target information from a single FITS file.
 */

int tg_read (const char *filename, target *tg);

#endif /* _IOFITS_ */
