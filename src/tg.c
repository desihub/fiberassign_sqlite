#include "tg.h"
#include "constants.h"

#include "stdio.h"
#include "math.h"

int 
get_targets (char *filename, double *x, double *y, double *z)
{
    double ra;
    double dec;
    double pd;
    int pi;

    int N = 0;

    FILE *ifile = fopen(filename, "r");
    while (fscanf(ifile, "%lf %lf %lf %lf %d %lf %d\n", &ra, &dec, &pd, &pd,
	  &pi, &pd, &pi) != EOF) {
	dec *= DTOR;
	ra *= DTOR;
	x[N] = sin(dec - PI/2.0)*cos(ra);
	y[N] = sin(dec - PI/2.0)*sin(ra);
	z[N] = cos(dec - PI/2.0); 
	N++;
    }
    fclose(ifile);
    return N;
}
