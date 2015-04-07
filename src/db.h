/**
 * Functions that use sqlite3 databases are defined here.
 */

#ifndef _DB_
#define _DB_

#include "sqlite3.h"

#include "fa.h"

/**
* Create a file -- db, from an array of x, y, z of length N.
* The file will later be used by sqlite3 to create a database.
*/

void create_sql_file (char *filename, char *dbname, double *x, double *y, double *z, int N);

/**
 * Read an sqlite3 database -- db, from a file -- filname.
 */

void read_db (sqlite3 *db, char *filename);

/**
 * Find all targets (in db) that are within a cube of size -- r (side = 2*r)
 * around fiber -- fb. Return total number of such targets.
 */
int fiber_reach (sqlite3 *db, float r, fiber *fb);

#endif /* _DB_ */
