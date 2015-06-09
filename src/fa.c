#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#include "fa.h"
#include "constants.h"

/**< I will use this structure in sort_fiber.  */
 
typedef struct
{
    int ID;
    /**< random number for comp_tgt function. */
    int random;
    double dist;
    float pr;
    int Nobs;
    int type;
}tgt;

/**
 * I will use this comparison function in sort_fiber. Sort targets first by
 * priority and then either by closeness to the fiber or randomly.
 */

static int 
comp_tgt (const void *tgt1, const void *tgt2)
{
    tgt *t1 = (tgt*)tgt1;
    tgt *t2 = (tgt*)tgt2;
    /**< Not sure why high priority is lower number. Ask Stephen! */
    if (t1->pr < t2->pr) return -1;
    if (t1->pr > t2->pr) return 1;
    /**
     * If an object is QSOa or LRG that has been observed already give it
     * priority
     */
    if ((t1->type == 1 && t2->type == 1) || (t1->type == 3 && t2->type == 3)) {
        if (t1->Nobs != 0 && t2->Nobs == 0) return 1;
        if (t1->Nobs == 0 && t2->Nobs != 0) return -1;
    }
    /**< If nothing above applies pick one randomly. */
    if (t1->random <= t2->random) return -1; 
    if (t1->random > t2->random) return 1; 
    
    return 0;
}

void 
sort_fiber (fiber *fb, target *tg)
{
//    srand(time(NULL));
    int Ntgt = fb->Ntgt;
    if (Ntgt > MAXTGT) Ntgt = MAXTGT;
    tgt idp[Ntgt];
    /**< Copy relevant target information to auxiliary structure tgt */
    for (int i = 0; i < Ntgt; i++) {
        idp[i].ID = fb->targetID[i];
        idp[i].random = rand();
        idp[i].dist = fb->target_dist[i];
        idp[i].pr = tg[fb->targetID[i]].pr;
        idp[i].Nobs = tg[fb->targetID[i]].N;
        idp[i].type = tg[fb->targetID[i]].type;
    }
    /**< Sort using comp_tgt function */
    qsort((void*)idp, Ntgt, sizeof(tgt), comp_tgt);
    for (int i = 0; i < Ntgt; i++) {
        fb->targetID[i] = idp[i].ID;
        fb->target_dist[i] = idp[i].dist;
    }
    return;
} 

/**
 * Assign to the object of highest priority unless it's already been observed
 * sufficient number of times or has been assigned to another fiber on the same
 * plate. This version doesn't give a preference to an observed QSO or LRG with
 * respect to an unobserved one (I will fix that in the next version).  If
 * multiple targets have the same priority, one of them will be chosen randomly.
 * fbr and tgt point to the beginning of the two arrays.
 */

void 
assign_fiber(fiber *fbr, target *tgt, int NN)
{
    /**< These two point to specific fiber and target */
    fiber *fb = fbr + NN; 
    target *tr;
    /**< For each target that this fiber can potentially reach */
    int Ntgt = fb->Ntgt;
    if (Ntgt > MAXTGT) Ntgt = MAXTGT;
    for (int i = 0; i < Ntgt; i++) {
        tr = tgt + fb->targetID[i];
        /**
         * Check that the target has not already been observed by a fiber on the
         * same tile. If yes than skip it and go to the next.
         */
        for (int j = 0; j < tr->N; j++) {
            if (fbr[tr->fiberID[j]].plateID == fb->plateID) {
                continue;
            }
        }
        /** 
         * If the target hasn't been observed sufficient number of times
         * observe it.
         */
        if (tr->N < tr->nobs) {
            tr->fiberID[tr->N] = NN;
            tr->N = (tr->N + 1);
            fb->obs_tgt_id = tr->ID;
            break;
        }
    } 
    return;
}

void
switch_fiber(fiber *fb, target *tg, int N)
{
    fiber *fbr = fb;
    fiber *fbr_alt;
    int tgid;
    int tgid_alt;
    for (int i = 0; i < N; i++) {
        /**< If the fiber's unassigned */
        if (fbr->obs_tgt_id != -1) goto OUTER_LOOP;
        /**< For each potential target */
        int j_up = fbr->Ntgt;
        if (j_up > MAXTGT) j_up = MAXTGT;
        for (int j = 0; j < j_up; j++) {
            tgid = fbr->targetID[j];
            for (int k = 0; k < tg[tgid].N; k++) {
                /**< Does the fiber that has been assigned to that target */  
	        fbr_alt = fb + tg[tgid].fiberID[k];
                for (int l = 0; l < fbr_alt->Ntgt; l++) {
                    tgid_alt = fbr_alt->targetID[l];
                    /**< Have another unassigned target within its reach */
                    if (tg[tgid_alt].nobs != tg[tgid_alt].N) {
                        /**< If yes, switch targets */
                        fbr->obs_tgt_id = tgid;
                        tg[tgid].fiberID[k] = fbr->ID;
                        fbr_alt->obs_tgt_id = tgid_alt;
                        tg[tgid_alt].fiberID[tg[tgid_alt].N] = fbr_alt->ID; 
                        tg[tgid_alt].N = tg[tgid_alt].N + 1;
                        goto OUTER_LOOP;
                    } 
                }
            }
        }
        /** 
         * This is one of the exceptional cases where goto statement is
         * actually useful in a c code.
         */
        OUTER_LOOP: fbr++;
    }
    return;
}
