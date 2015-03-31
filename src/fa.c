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
    /**< If objects have the same priority pick one randomly. */
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
 //       idp[i].random = rand();
        idp[i].dist = fb->target_dist[i];
        idp[i].pr = tg[fb->targetID[i]].pr;
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

void assign_fiber(fiber *fbr, target *tgt, int NN)
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
            break;
        }
    } 
    return;
}
