/**
 * Structures and functions that are related to fibers are defined here.
 */

#ifndef _FA_
#define _FA_

#include "constants.h"
#include "tg.h"

/** 
 * This structure describes a fiber. x, y, z -- coordinates on unit sphere.
 * plateID -- plate to which it belongs, ID - id of fiber (on plate), targetID
 * -- id's of targets that can be reached by this fiber (only keep info of first
 * MAXTGT. target_dist -- distances from fiber of targets (same order as in
 * targetID).  Ntgt is the total number of targets reachable by the fiber.
 * weight -- the (optional) weight of the object. obs_tgt_id -- the target to
 * which the fiber was assigned to.
 */

typedef struct 
{
    float x, y, z;
    int plateID;
    int ID;
    int targetID[MAXTGT];
    float target_dist[MAXTGT];
    int Ntgt;
    float weight;
    /* obs_tgt_id = -1 means no target assigned */
    int obs_tgt_id;
}fiber;

/**
 * This function will sort targetID and target_dist in a fiber -- fb so that
 * they are arranged in the order of priority and distance from fiber. I have
 * also to provide the array of targets -- tg.
 */

void sort_fiber(fiber *fb, target *tg);

/**
 * This function will assign a fiber to one of the targets. fb and tgt point at
 * the beginning of arrays, N-th fiber in in array fb will be assigned.
 */

void assign_fiber(fiber *fb, target *tgt, int N);

/**
 * Go through all fibers. Identify unassigned fibers. If possible, assign them
 * to a target and reassign the fiber that has previously been assigned to that
 * target to another. N is the number of fibers.
 */

void switch_fiber(fiber *fb, target *tgt, int N);

#endif /* _FA_ */
