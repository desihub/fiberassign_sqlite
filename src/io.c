#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "fa.h"
#include "constants.h"
#include "io.h"

int 
init_targets(target* tgt, char* file)
{
    FILE* ifile = fopen(file, "rb");
 
    int Nobj;
    float *ra, *dec, *red, *priority;
    int *id, *Nobs;

    fread(&Nobj, sizeof(int), 1, ifile);
 
    ra = (float*) malloc(Nobj*sizeof(float));
    dec = (float*) malloc(Nobj*sizeof(float));
    red = (float*) malloc(Nobj*sizeof(float));
    priority = (float*) malloc(Nobj*sizeof(float));
    id = (int*) malloc(Nobj*sizeof(int));
    Nobs = (int*) malloc(Nobj*sizeof(int));

    fread(ra, sizeof(float), Nobj, ifile);
    fread(dec, sizeof(float), Nobj, ifile);
    fread(red, sizeof(float), Nobj, ifile);
    fread(id, sizeof(int), Nobj, ifile);
    fread(priority, sizeof(float), Nobj, ifile);
    fread(Nobs, sizeof(int), Nobj, ifile);

    fclose(ifile);

    for (int i = 0; i < Nobj; i++) {
        tgt[i].ra = ra[i];
        tgt[i].dec = dec[i];
        tgt[i].red = red[i];
        tgt[i].x = sin(dec[i]*DTOR - PI/2.0)*cos(ra[i]*DTOR);
        tgt[i].y = sin(dec[i]*DTOR - PI/2.0)*sin(ra[i]*DTOR);
        tgt[i].z = cos(dec[i]*DTOR - PI/2.0);
        tgt[i].type = id[i]; 
        tgt[i].pr = priority[i];
        tgt[i].nobs = Nobs[i];
        tgt[i].N = 0;
        tgt[i].weight = 1.0;
        tgt[i].ID = i;
        for (int j = 0; j < MAXOBS; j++) {
            tgt[i].fiberID[j] = 0;
        }
    }

    return Nobj;
}

void 
dump_targets(const char* ifile, const char* ofile)
{
    FILE * ff = fopen(ofile, "w");
    target *tgt = (target*)malloc(sizeof(target)*NTARGET);
    int Ntgt = read_targets(tgt, ifile);
    for (int i = 0; i < Ntgt; i++) {
        fprintf(ff, "%le %le %le\n", tgt[i].x, tgt[i].y, tgt[i].z);
    }
    fclose(ff);
}
 
void 
write_targets(target *tg, int N, char *file)
{
    FILE *ofile = fopen(file, "wb");
    fwrite(&N, sizeof(int), 1, ofile);
    fwrite(tg, sizeof(target), N, ofile);
    fclose(ofile);
    return;
}

int 
read_targets (target* obj, const char* file)
{
    FILE *ifile = fopen(file, "rb");
    int N;
    fread(&N, sizeof(int), 1, ifile);
    fread(obj, sizeof(target), N, ifile);
    fclose(ifile);
    return N;
}

int 
init_fibers (fiber *fib, char *fiber_file, char *plate_file)
{
    FILE *ffile = fopen(fiber_file, "r");
    FILE *pfile = fopen(plate_file, "r");
    /**< Read fiber x, y, z with respect to plate center */
    int fID[MAXFIBER];
    float fx[MAXFIBER];
    float fy[MAXFIBER];
    float fz[MAXFIBER];
    printf("Read fiber file\n");
    /**< Remove first 5 lines of comments */
    char comment[2000];
    for (int i = 0; i < 5; i++) {
        fgets(comment, 2000, ffile);
    }

    int dd;
    for (int i = 0; i < MAXFIBER; i++) {
        fscanf(ffile, "%d %d %d %f %f %f\n", fID+i, &dd, &dd, fx+i, fy+i, fz+i);
    }

    /**< Read plate center information */
    int Nplate = 0;
    int di;
    int in_desi; /**< 1 if the tile is in DESI footprint */
    float ra_tile, dec_tile; 
    float df;
    float *ra = (float *)malloc(sizeof(float)*NPLATE);
    float *dec = (float *)malloc(sizeof(float)*NPLATE);
    printf("Read plate file\n");
    /**< Remove first 12 lines of comments */
    for (int i = 0; i < 12; i++) {
        fgets(comment, 2000, pfile);
    }
    /**< Until you get desired number of plates in the observed area */
    while (Nplate < NPLATE) {
        fscanf(pfile, "STRUCT1 %d %f %f %d %d %f %f %f\n", &di, &ra_tile,
               &dec_tile, &di, &in_desi, &df, &df, &df);
        if (in_desi == 1) {
            ra[Nplate] = ra_tile;
            dec[Nplate] = dec_tile;
            Nplate++;
        }
    }
    printf("done reading plate file\n");
    /**< x, y, z on unit sphere for each fiber */
    int Nfib = 0;
    float pra, pdec;
    for (int i = 0; i < Nplate; i++) {
        for (int j = 0; j < MAXFIBER; j++) {
            fib[Nfib].plateID = i;
            fib[Nfib].ID = j;
            for (int k = 0; k < MAXTGT; k++) {
                fib[Nfib].targetID[k] = 0;
                fib[Nfib].target_dist[k] = 0.0;
            }
            fib[Nfib].Ntgt = 0;
            fib[Nfib].weight = 1.0;
            /**< Not completely sure about this part. Check later!!! */
            /**
             * Start from plate center pointing towards x axis with correct
             * orientation, which means x->z, z->-y, y->x.
             */
            pra = ra[i]*DTOR + fx[j]*MM_TO_RAD;
            pdec = -3.14159/2.0;
            pdec += dec[i]*DTOR;
            pdec += -fy[j]*MM_TO_RAD;
            fib[Nfib].x = sin(pdec)*cos(pra);
            fib[Nfib].y = sin(pdec)*sin(pra);
            fib[Nfib].z = cos(pdec);
            Nfib++;
        }
    }

    free(ra);
    free(dec);
    fclose(ffile);
    fclose(pfile);

    return Nfib;
}

void 
dump_fibers(const char* ifile, const char* ofile)
{
    printf("dump_fibers\n"); 
    FILE *ff = fopen(ofile, "w");
    printf("assign fibers\n");
    fiber *fb = (fiber*)malloc(sizeof(fiber)*NFIBER);
    printf("reading fibers\n");
    int Nfb = read_fibers(fb, ifile);
    printf("Nfb = %d\n", Nfb);
    for (int i = 0; i < Nfb; i++) {
        fprintf(ff, "%le %le %le\n", fb[i].x, fb[i].y, fb[i].z);
    }
    fclose(ff);
}

void
write_fibers (fiber *fb, int N, char *file)
{
    FILE *ofile = fopen(file, "wb");
    fwrite(&N, sizeof(int), 1, ofile);
    fwrite(fb, sizeof(fiber), N, ofile);
    fclose(ofile);
    return;
}

int
read_fibers (fiber *fb, const char *file)
{
    FILE *ifile = fopen(file, "rb");
    int N;
    fread(&N, sizeof(int), 1, ifile);
    fread(fb, sizeof(fiber), N, ifile);
    fclose(ifile);
    return N;
}
void 
stat_targets (target *tgt, int N)
{
    /* T -- total, O -- observed */
    int Tqsoa = 0;
    int Oqsoa = 0;
    int Tqso = 0;
    int Oqso = 0;
    int Tlrg = 0;
    int Olrg = 0;
    int Telg = 0;
    int Oelg = 0;
 
    for (int i = 0; i < N; i++) {
        switch (tgt[i].type) {
            case 1: /**< QSO-alpha */
                Tqsoa++;
                if (tgt[i].nobs == tgt[i].N) Oqsoa++;
                break;
            case 2: /**< QSO */
                Tqso++;
                if (tgt[i].nobs == tgt[i].N) Oqso++;
                break;
            case 3: /**< LRG */
                Tlrg++;
                if (tgt[i].nobs == tgt[i].N) Olrg++;
                break;
            case 4: /**< ELG */
                Telg++;
                if (tgt[i].nobs == tgt[i].N) Oelg++;
                break;
            default:
                break;
        }
    }

    printf("%f per cent of QSOa (%d/%d) observed\n", 100.0*(double)Oqsoa/Tqsoa,
           Oqsoa, Tqsoa);
    printf("%f per cent of QSO (%d/%d) observed\n", 100.0*(double)Oqso/Tqso,
           Oqso, Tqso);
    printf("%f per cent of LRG (%d/%d) observed\n", 100.0*(double)Olrg/Tlrg,
           Olrg, Tlrg);
    printf("%f per cent of ELG (%d/%d) observed\n", 100.0*(double)Oelg/Telg,
           Oelg, Telg);

    return;
}



void
stat_fibers (fiber *fbr, int N)
{
    /**< histogram of number of targets per fiber. The last bin is for 20+ */
    int H[20];
    for (int i = 0; i < 20; i++) {
        H[i] = 0.0;
    }

    for (int i = 0; i < N; i++) {
        int Ntgt = fbr[i].Ntgt;
        if (Ntgt >= 20) H[19]++;
        else H[Ntgt]++;
    }
  
    for (int i = 0; i < 20; i++) {
        printf("%d %d\n", i, H[i]);
    }

    return;
}

void
print_observed_targets (target *tgt, int N, char *file)
{
    FILE *ofile = fopen(file, "w");
    
    for (int i = 0; i < N; i++) {
        if (tgt[i].N == tgt[i].nobs /**< if observed sufficient number of times */) { 
            fprintf(ofile, "%e %e %e %d\n", tgt[i].ra, tgt[i].dec, tgt[i].red, tgt[i].type);
        }
    }

    fclose(ofile);

    return;
}

void
print_fibers (fiber *fb, int Nfb, char *file)
{
    FILE *ifile = fopen(file, "w");
    for (int i = 0; i < Nfb; i++) {
        fprintf(ifile, "%e %e %e\n", fb[i].x, fb[i].y, fb[i].z);
    }
    fclose(ifile);
    return;
}
