#include "iofits.h"
#include "constants.h"
#include "fa.h"
#include "tg.h"

#include "string.h"

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
