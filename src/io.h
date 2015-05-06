/**
 * Functions related to I/O are here.
 */

#ifndef _IO_
#define _IO_

#include "fa.h"
#include "tg.h"

/************************************************************************************
 * Reading and writing staff.
 ***********************************************************************************/

/**
 * Read targets from file -- file and initialize an array of targets -- tgt.
 * File should be a binary that has (int)Nobj, (float)ra[Nobj], (float)dec[Nobj],
 * (float)redshift[Nobj], and (int)type[Nobj] of target. This is how Martin stores his
 * simulation data in binary files. Returns total number of targets.
 */

int init_targets(target* tgt, char* file);

/**
 * This function will read a binary file with targets information and dump the
 * ra, dec, redshift onto an ASCII file. (For debugging purposes). It will
 * also write the type of target and the number of times it has been observed.
 */

void dump_targets(const char *ifile, const char *ofile);

/** 
 * This function will write array of targets -- tg of length -- N to a binary
 * file -- file
 */

void write_targets(target *tg, int N, char *file);

/**
 * This function will read array of targets -- tg from a binary file -- file
 * created by write_fibers. Returns total number of targets.
 */

int read_targets(target *tg, const char *file);

/**
 * This function will read the fiber (ffile) and plate center (pfile)
 * information from files and return a total number of fibers. It will
 * initialize fiber array -- fb.
 */

int init_fibers(fiber *fb, char *ffile, char *pfile);

/**
 * This function will read a binary file with fiber information and dump the
 * x, y, z coordinates onto an ASCII file. (For debugging purposes).
 */

void dump_fibers(const char *ifile, const char *ofile);

/** 
 * This function will write array of fibers -- fib of length -- N to a binary
 * file -- file
 */

void write_fibers(fiber *fb, int N, char *file);

/**
 * This function will read array of fibers -- fib from a binary file -- file
 * created by write_fibers. Returns total number of fibers.
 */

int read_fibers(fiber *fb, const char *file);

/************************************************************************************
 * Outputting results.
 ***********************************************************************************/

/**
 * This function will go through a target array -- tgt of length -- N and will
 * print out some basic statistics.
 */

void stat_targets (target *tgt, int N);

/**
 * This function will go through a fiber array -- fbr of length -- N and will
 * print out some basic statistics.
 */

void stat_fibers (fiber *fbr, int N);

/**
 * This function will create an ASCII file with all the targets (ra, dec, red,
 * type) that have been observed (assigned enough fibers). tg -- the array of
 * targets, N -- length of tg array, file -- name of the output file.
 */

void print_observed_targets (target *tgt, int N, char *file);

/**
 * This function will create an ASCII file -- file of x, y, z coordinates of all fibers
 * in array fb of size Nfb.
 * For testing purposes.
 */

void print_fibers (fiber *fb, int Nfb, char *file);

#endif /* _IO_ */
