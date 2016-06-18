# How does it work?
It uses fork() system call to quickly create another process (using copy-on-write) the transaction continues in the newly created process while the parent process is halted, if the operation done in the transaction succeeds (child calls transaction_commit()) the parent is terminated while the child continues. If the transaction is aborted either by child crashing or calling transaction_rollback() the child process terminates and the parent continues skipping the transaction code.

# How to use it?
Compile it into a library (dynamic or static) for your operating system, link it with your code/binary and use as follows:
```
#include "atomic_fork.h"

...

if (transaction()){
    // this will be executed as part of the transaction
    printf("It Works!\n"); // some dodgy code goes here
    transaction_rollback(); // or transaction_commit()
} else {
    printf("Aborted... cleaning up\n"); // handle the failed transaction if needed
}
```
Please keep in mind because of how fork() works the code running within the transaction should behave as if it was running in the parent, memory is referenced so everything from the parent will be available to the child, except for a few minor things (read the man page of fork()).

# Why?
Because let it crash! this way you don't have to care, or cleanup anything, the transaction is like a place for you to try things without breaking anything or having to do complicated state management and recovery.
