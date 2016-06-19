#include <stdio.h>
#include <sys/time.h>
#include "atomic_fork.h"
#include <time.h>

int main() {
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    af_timed(5) {
        gettimeofday(&tv2, NULL);
        printf ("Total time = %f microseconds\n", (double) (tv2.tv_usec - tv1.tv_usec));
        printf("It Works!\n");
        while (1) {
            printf("child runs\n");
            sleep(1);
        }
        //af_rollback();
    } else {
        printf("Aborted... cleaning up\n");
    }
}
