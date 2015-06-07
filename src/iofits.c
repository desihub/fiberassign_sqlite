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
    }
    int target_id[MAXFIBER];
    int desi_target[MAXFIBER];
    float ra[MAXFIBER];
    float dec[MAXFIBER];
    float x_focal[MAXFIBER];
    float y_focal[MAXFIBER];
    int potential_target_id[MAXFIBER*MAXTGT];
    int *ptid = potential_target_id;
    int tot_targets = 0;
    printf("start the loop\n");
    for (int i = Nplate*MAXFIBER; i < MAXFIBER; i++) {
        fiber_id[i] = i;
        positioner_id[i] = i;
        num_target[i] = fb[i].Ntgt;
        printf("%d ", fb[i].obs_tgt_id);
        switch (tg[fb[i].obs_tgt_id].type) {
            case 1:
                strcpy(objtype[i], "QSO");
                break;
            case 2:
                strcpy(objtype[i], "QSO");
                break;
            case 3:
                strcpy(objtype[i], "LRG");
                break;
            case 4:
                strcpy(objtype[i], "ELG");
                break;
            case 5:
                strcpy(objtype[i], "QSO");
                break;
            case 6:
                strcpy(objtype[i], "LRG");
                break;
            default:
                strcpy(objtype[i], "OTHER");
                break;
        }
        printf("%s ", objtype[i]);
        target_id[i] = fb[i].obs_tgt_id;
        desi_target[i] = 0;
        ra[i] = fb[i].obs_tgt_id == -1 ? 370.0 : tg[fb[i].obs_tgt_id].ra;
        dec[i] = fb[i].obs_tgt_id == -1 ? 370.0 : tg[fb[i].obs_tgt_id].dec;
        x_focal[i] = 0;
        y_focal[i] = 0;
        for (int j = 0; j < num_target[i]; j++) {
            *ptid = fb[i].targetID[j];
            ptid++;
        }
        tot_targets += num_target[i];
    }
    /**< write to fits file */
    int status;
    fitsfile *fptr;
    fits_create_file(&fptr, filename, &status);
    fits_report_error(stdout, status);
    /**< FiberMap table */
    char *ttype[] = {"fiber", "positioner", "numtarget", "objtype", "targetid", "desi_target0", "ra", "dec", "xfocal_design", "yfocal_design"};
    char *tform[10] = {"U", "U", "U", "8A", "J", "K", "E", "E", "E", "E"};
    char *tunit[10] = { "", "", "", "", "", "", "deg", "deg", "mm", "mm"};
    char extname[] = "FiberMap";
    fits_create_tbl(fptr, BINARY_TBL, 0, 10, ttype, tform, tunit, extname, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TINT, 1, 1, 1, MAXFIBER, fiber_id, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TINT, 2, 1, 1, MAXFIBER, positioner_id, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TINT, 3, 1, 1, MAXFIBER, num_target, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TSTRING, 4, 1, 1, MAXFIBER, ot_tmp, &status);
    fits_report_error(stdout, status);
    fits_write_col(fptr, TINT, 5, 1, 1, MAXFIBER, target_id, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TINT, 6, 1, 1, MAXFIBER, desi_target, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 7, 1, 1, MAXFIBER, ra, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 8, 1, 1, MAXFIBER, dec, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 9, 1, 1, MAXFIBER, x_focal, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TFLOAT, 10, 1, 1, MAXFIBER, y_focal, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    /**< PotentialFiberMap table */
    char *ttype2[] = {"potentialtargetid"};
    char *tform2[1] = {"V"}; 
    char *tunit2[10] = {""};
    char extname2[] = "PotentialFiberMap"; 
    fits_create_tbl(fptr, BINARY_TBL, 0, 1, ttype2, tform2, tunit2, extname2, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_write_col(fptr, TINT, 1, 1, 1, tot_targets, potential_target_id, &status);
    printf("#\n");
    fits_report_error(stdout, status);
    fits_close_file(fptr, &status);
    printf("#\n");
    fits_report_error(stdout, status);

    return;
}
