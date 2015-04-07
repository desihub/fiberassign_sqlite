#include "sqlite3.h"

#include "fa.h"
#include "io.h"
#include "constants.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

/**< This global variable keeps count of how many targets were selected */
int Ntg;

void
create_sql_file (char *filename, char *dbname, double *x, double *y, double *z, int N)
{

    FILE * sql_file = fopen(filename, "w");
    /** 
     * I'm assuming that the filename will end on .sql, in which case I'll
     * replace it by .db
     */
    fprintf(sql_file, "CREATE VIRTUAL TABLE targets USING rtree (id, minx, maxx, miny, maxy, minz, maxz);\n");
    fprintf(sql_file, "BEGIN TRANSACTION;\n");

    for (int i = 0; i < N; i++) {
        fprintf(sql_file, "INSERT INTO targets VALUES (%d, %le, %le, %le, %le, %le, %le);\n", i, x[i], x[i], y[i], y[i], z[i], z[i]);
    }

    fprintf(sql_file, "END TRANSACTION;\n");
    fprintf(sql_file, ".backup %s\n", dbname);
    fclose(sql_file); 
    return;
}

void
read_db (sqlite3 *db, char *file)
{
    sqlite3 *db_file;
    sqlite3_open(file, &db_file);
    /**< Copy database on file to in-memory database */
    sqlite3_backup *bak = sqlite3_backup_init(db, "main", db_file, "main");
    if(bak) {
        sqlite3_backup_step(bak, -1);
        sqlite3_backup_finish(bak);
    }
    
    sqlite3_close(db_file);

    return;
}

/**< This function is used by fiber_reach function below */
static int
find_targets (void *data, int argc, char **argv, char **colname)
{
    /* target coordinates */
    float tx, ty, tz;
    /* fiber coordinates */
    float fx, fy, fz;
    /* distance between fiber and a target */
    float dx, dy, dz;
    float r;

    /**< targets above MAXTGT will be ignored */
    if (Ntg >= MAXTGT) {
        printf("Error: exceeded MAXTGT, %d\n", Ntg);
        Ntg++;
        return 0;
    }
    fiber *fb = (fiber*)data;
    tx = atof(argv[1]);
    ty = atof(argv[3]);
    tz = atof(argv[5]);
    fx = fb->x;
    fy = fb->y;
    fz = fb->z;
    dx = tx - fx;
    dy = ty - fy;
    dz = tz - fz;
    r = sqrt(dx*dx + dy*dy + dz*dz);
    /* If within 6mm patrol radius */
    if (r < PATROL_RAD*MM_TO_RAD) {
        fb->targetID[Ntg] = atoi(argv[0]);
        fb->target_dist[Ntg] = r;
        Ntg++;
    }
    return 0;
}

int
fiber_reach (sqlite3 *db, float r, fiber *fb)
{
    Ntg = 0;
    char sql[2000];
    char *zErrMsg = 0;
    double x = fb->x;
    double y = fb->y;
    double z = fb->z;
    sprintf(sql, "SELECT * FROM targets WHERE minX > %f AND maxX < %f AND minY > %f AND maxY < %f AND minZ > %f AND maxZ < %f\n", x-r, x+r, y-r, y+r, z-r, z+r);
    sqlite3_exec(db, sql, find_targets, (void*)fb, &zErrMsg);
    if (Ntg > MAXTGT) Ntg = MAXTGT;
    fb->Ntgt = Ntg;
    return Ntg;
}
