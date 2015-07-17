#include "iofits.h"
#include "constants.h"
#include "fa.h"
#include "tg.h"

#include "string.h"
#include "math.h"

void
fa_write(const char *filename, fiber *fb, target *tg, int Nplate)
{
    /**< initialize arrays */
    int fiber_id[MAXFIBER];
    int positioner_id[MAXFIBER];
    int num_target[MAXFIBER];
    char objtype[MAXFIBER][8];
    char *ot_tmp[MAXFIBER];
    for (int i = 0; i < MAXFIBER; i++) {
        ot_tmp[i] = objtype[i];
        //printf("%s\n", ot_tmp[i]);
    }
    int target_id[MAXFIBER];
    int desi_target[MAXFIBER];
    float ra[MAXFIBER];
    float dec[MAXFIBER];
    float x_focal[MAXFIBER];
    float y_focal[MAXFIBER];
    int potential_target_id[MAXFIBER*MAXTGT];
    int ptid = 0;
    /**< Total number of targets on all fibers of a tile */
    int tot_tgt = 0;
    /**< Index on plate */
    int iop = 0;
    for (int i = Nplate*MAXFIBER; i < (Nplate + 1)*MAXFIBER; i++) {
        fiber_id[iop] = i;
        positioner_id[iop] = i;
        num_target[iop] = fb[i].Ntgt;
        tot_tgt += num_target[iop];
        if (tg[fb[i].obs_tgt_id].type == 1) 
            strcpy(objtype[iop], "QSO");
        else if (tg[fb[i].obs_tgt_id].type == 2)
            strcpy(objtype[iop], "QSO");
        else if (tg[fb[i].obs_tgt_id].type == 3)
            strcpy(objtype[iop], "LRG");
        else if (tg[fb[i].obs_tgt_id].type == 4)
            strcpy(objtype[iop], "ELG");
        else if (tg[fb[i].obs_tgt_id].type == 5)
            strcpy(objtype[iop], "QSO");
        else if (tg[fb[i].obs_tgt_id].type == 6)
            strcpy(objtype[iop], "LRG");
        else
            strcpy(objtype[iop], "OTHER");
        target_id[iop] = fb[i].obs_tgt_id;
        desi_target[iop] = 0;
        ra[iop] = fb[i].obs_tgt_id == -1 ? 370.0 : tg[fb[i].obs_tgt_id].ra;
        dec[iop] = fb[i].obs_tgt_id == -1 ? 370.0 : tg[fb[i].obs_tgt_id].dec;
        x_focal[iop] = 0;
        y_focal[iop] = 0;
        for (int j = 0; j < num_target[iop]; j++) {
            potential_target_id[ptid] = fb[i].targetID[j];
            ptid++;
            if (ptid > MAXFIBER*MAXTGT) printf("ptid problem\n");
        }
        iop++;
    }
    /**< If the tile is empty */
    if (tot_tgt == 0) {
        printf("Empty plate\n");
        return;
    }
    if (tot_tgt < 0) {
       printf("%d targets?\n", tot_tgt);
       return;
    }
    /**< write to fits file */
    int status;
    fitsfile *fptr;
    fits_create_file(&fptr, filename, &status);
    fits_report_error(stdout, status);
    /**< FiberMap table */
    char *ttype[] = {"fiber", "positioner", "numtarget", "objtype", "targetid", "desi_target0", "ra", "dec", "xfocal_design", "yfocal_design"};
    char *tform[10] = {"K", "K", "K", "8A", "K", "K", "E", "E", "E", "E"};
    char *tunit[10] = { "", "", "", "", "", "", "deg", "deg", "mm", "mm"};
    char extname[] = "FiberMap";
    fits_create_tbl(fptr, BINARY_TBL, 0, 10, ttype, tform, tunit, extname, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TLONGLONG, 1, 1, 1, MAXFIBER, fiber_id, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TLONGLONG, 2, 1, 1, MAXFIBER, positioner_id, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TLONGLONG, 3, 1, 1, MAXFIBER, num_target, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TSTRING, 4, 1, 1, MAXFIBER, ot_tmp, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TLONGLONG, 5, 1, 1, MAXFIBER, target_id, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TLONGLONG, 6, 1, 1, MAXFIBER, desi_target, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 7, 1, 1, MAXFIBER, ra, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 8, 1, 1, MAXFIBER, dec, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 9, 1, 1, MAXFIBER, x_focal, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 10, 1, 1, MAXFIBER, y_focal, &status);
    fits_report_error(stdout, status);
    /**< PotentialFiberMap table */
    char *ttype2[] = {"potentialtargetid"};
    char *tform2[1] = {"K"}; 
    char *tunit2[10] = {""};
    char extname2[] = "PotentialFiberMap"; 
    fits_create_tbl(fptr, BINARY_TBL, 0, 1, ttype2, tform2, tunit2, extname2, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TLONGLONG, 1, 1, 1, tot_tgt, potential_target_id, &status);
    fits_report_error(stdout, status);
    fits_close_file(fptr, &status);
    fits_report_error(stdout, status);

    return;
}

int
tg_read (const char *filename, target *tgt)
{
    fitsfile *fptr;
    long int Ntgt;
    int status, hdutype, anynull, hdunum;
    char strnull[10];
    strcpy(strnull, " ");
    printf("opening input fits file\n");
    fits_open_file(&fptr, filename, READONLY, &status);
    printf("moving to the right header\n");
    fits_get_num_hdus(fptr, &hdunum, &status);
    printf("%d\n", hdunum);
    fits_get_hdu_num(fptr, &hdunum);
    printf("%d\n", hdunum);
    fits_movabs_hdu(fptr, 2, &hdutype, &status);
    printf("%d\n", hdutype);
    printf("getting number of rows\n");
    fits_get_num_rows(fptr, &Ntgt, &status);
    printf("Ntgt = %ld\n", Ntgt);
    int *id = (int*) malloc(Ntgt*sizeof(int));
    int *tid = (int*) malloc(Ntgt*sizeof(int));
    float *ra = (float*) malloc(Ntgt*sizeof(float));
    float *dec = (float*) malloc(Ntgt*sizeof(float));
    float *pr = (float*) malloc(Ntgt*sizeof(float));
    int *nobs = (int*) malloc(Ntgt*sizeof(int));
    char *type[Ntgt];
    for (int i = 0; i < Ntgt; i++) type[i] = (char*)malloc(8*sizeof(char));
    fits_read_col(fptr, TSHORT, 1, 1, 1, Ntgt, strnull,  id,
                    &anynull, &status);
    fits_read_col(fptr, TSHORT, 2, 1, 1, Ntgt, strnull,  tid,
                    &anynull, &status);
    fits_read_col(fptr, TDOUBLE, 3, 1, 1, Ntgt, strnull,  ra,
                    &anynull, &status);
    fits_read_col(fptr, TDOUBLE, 4, 1, 1, Ntgt, strnull,  dec,
                    &anynull, &status);
    fits_read_col(fptr, TDOUBLE, 5, 1, 1, Ntgt, strnull,  pr,
                    &anynull, &status);
    fits_read_col(fptr, TDOUBLE, 6, 1, 1, Ntgt, strnull,  nobs,
                    &anynull, &status);
    fits_read_col(fptr, TSTRING, 7, 1, 1, Ntgt, strnull,  type,
                    &anynull, &status);
    for (int i = 0; i < Ntgt; i++) {
        tgt[i].ra = ra[i];
        tgt[i].dec = dec[i];
        tgt[i].red = 0.0;
        tgt[i].x = sin(dec[i]*DTOR - PI/2.0)*cos(ra[i]*DTOR);
        tgt[i].y = sin(dec[i]*DTOR - PI/2.0)*sin(ra[i]*DTOR);
        tgt[i].z = cos(dec[i]*DTOR - PI/2.0);
        if (strcmp(type[i], "LRG") == 0) tgt[i].type = 3;
        else if(strcmp(type[i], "ELG") == 0) tgt[i].type = 4;
        else if(strcmp(type[i], "QSO") == 0) tgt[i].type = 2;
        else tgt[i].type = 0;
        tgt[i].pr = pr[i];
        tgt[i].nobs = nobs[i];
        tgt[i].N = 0;
        tgt[i].weight = 1.0;
        tgt[i].ID = i;
        tgt[i].targetID = tid[i];
        for (int j = 0; j < MAXOBS; j++) {
            tgt[i].fiberID[j] = 0;
        }       
    }
    free(id);
    free(tid);
    free(ra);
    free(dec);
    free(pr);
    free(nobs);
    for (int i = 0; i < Ntgt; i++) free(type[i]);
    fits_close_file(fptr, &status);
    return Ntgt;
}
