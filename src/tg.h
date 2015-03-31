/**
 * Structures that are related to targets are defined here.
 */

#ifndef _TG_
#define _TG_

#include "constants.h"

/**
 * This structure describes targets. type -- (QSOa, QSO, LRG, ERG, ...); x, y, z
 * of a target on a unit sphere; ra, dec, redshift of a target; fiberID -- id's
 * of fibers that have been assigned to this target; pr -- priority (for some
 * reason in Martin's files high number means low priority); nobs -- number of
 * fiber assignments required for redshift determination; N -- number of fibers
 * actually assigned; weight -- for random points may incorporate things like
 * completeness, etc.
 */

typedef struct 
{
    int type;
    float x, y, z;
    float ra, dec, red;
    float pr;
    int nobs;
    int ID;
    int fiberID[MAXOBS];
    int N;
    float weight;
}target;

#endif /* _TG_ */
