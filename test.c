#include <stdio.h>
#include <sys/time.h>
#include "atomic_fork.h"

int main() {
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    if (transaction()){
        gettimeofday(&tv2, NULL);
        printf ("Total time = %f microseconds\n", (double) (tv2.tv_usec - tv1.tv_usec));
        printf("It Works!\n");
        transaction_rollback();
    } else {
        printf("Aborted... cleaning up\n");
    }
}
