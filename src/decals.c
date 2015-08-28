#include "decals.h"
#include "tg.h"

#include "stdio.h"
#include "string.h"
#include "math.h"

int
init_targets_decals(target *tgt, char *file)
{
    FILE* ifile = fopen(file, "r");
 
    int Nobj = 0;

    int dd, did; 
    float dnobs, dra, ddec, dpr;
    char dtype[10];
    printf("Start reading DECaLS\n");
    while (fscanf(ifile, "%d %d %f %f %f %f %s\n", &dd, &did, &dra, &ddec, &dpr,
           &dnobs, dtype) != EOF) {
        tgt[Nobj].ra = dra;
        tgt[Nobj].dec = ddec;
        tgt[Nobj].red = 0.0;
        tgt[Nobj].x = sin(ddec*DTOR - PI/2.0)*cos(dra*DTOR);
        tgt[Nobj].y = sin(ddec*DTOR - PI/2.0)*sin(dra*DTOR);
        tgt[Nobj].z = cos(ddec*DTOR - PI/2.0);
        if (strcmp(dtype, "LRG") == 0) tgt[Nobj].type = 3;
        else if(strcmp(dtype, "ELG") == 0) tgt[Nobj].type = 4;
        else if(strcmp(dtype, "QSO") == 0) tgt[Nobj].type = 2;
        else tgt[Nobj].type = 0;
        tgt[Nobj].pr = dpr;
        tgt[Nobj].nobs = dnobs;
        tgt[Nobj].N = 0;
        tgt[Nobj].weight = 1.0;
        tgt[Nobj].ID = Nobj;
        tgt[Nobj].targetID = did;
        for (int j = 0; j < MAXOBS; j++) {
            tgt[Nobj].fiberID[j] = 0;
        }       
        Nobj++;
    }
    fclose(ifile);

    return Nobj;
}
