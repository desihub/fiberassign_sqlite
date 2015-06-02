#include "fa.h"
#include "io.h"
#include "constants.h"
#include "iofits.h"

#include "stdio.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    const char ifib[2000] = "/scratch1/scratchdirs/lado/DATA/fb.dat";
//    const char ofib[2000] = "/scratch1/scratchdirs/lado/DATA/fib_rand.dmp";
    const char itgt[2000] = "/scratch1/scratchdirs/lado/DATA/tg.dat";
//    const char otgt[2000] = "/scratch1/scratchdirs/lado/tg_elg00.dmp";
    fiber *fb = malloc(sizeof(fiber)*NFIBER);
    int Nfb = read_fibers(fb, ifib);
    printf("%d\n", fb[0].obs_tgt_id);
    target *tg = malloc(sizeof(fiber)*NTARGET);
    Nfb = read_targets(tg, itgt);
    printf("read targets done\n");
    const char ffits[2000] = "/scratch1/scratchdirs/lado/test.fits";
    fa_write(ffits, fb, tg);
//    print_fibers(fb, Nfb, argv[2]);
//    printf("dumping fibers\n");
//    dump_fibers(ifib,ofib);
//    printf("dumping targets\n");
//    dump_targets(itgt,otgt);

    return 0;
}
