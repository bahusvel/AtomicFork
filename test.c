#include <stdio.h>
#include "atomic_fork.h"

int main() {
    if (transaction()){
        printf("It Works!\n");
        transaction_rollback();
    } else {
        printf("Aborted... cleaning up\n");
    }
}
