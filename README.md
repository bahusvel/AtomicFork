# How does it work?
It uses fork() system call to quickly create another process (using copy-on-write) the transaction continues in the newly created process while the parent process is halted, if the operation done in the transaction succeeds (child calls af_commit()) the parent is terminated while the child continues. If the transaction is aborted either by child crashing or calling af_rollback() the child process terminates and the parent continues skipping the transaction code.

# How to use it?
Just include the atomic_fork header file into your project and it will work on POSIX compliant OS (MAC OS X, Linux, BSD, other *nix)
``` C
#include "atomic_fork.h"

...

af_transaction {
    // this will be executed as part of the transaction
    printf("It Works!\n"); // some dodgy code goes here
    af_commit(); // or af_rollback()
} else {
    printf("Aborted... cleaning up\n"); // handle the failed transaction if needed
}
```
Please keep in mind because of how fork() works the code running within the transaction should behave as if it was running in the parent, memory is referenced so everything from the parent will be available to the child, except for a few minor things (read the man page of fork()).

# Why?
Because let it crash! this way you don't have to care, or cleanup anything, the transaction is like a place for you to try things without breaking anything or having to do complicated state management and recovery. Here are some specific use cases:
* Prevent broken piece of code from crashing your application (assuming you cant fix it)
* Add stability to interpreters

# Issues
* If you are running the process from shell (or something that waits on the process) the parent will die as soon as child af_commits()
* The fork() in current configuration is slow, it can be sped up by executing the code in parent and using child as backup (may change this in the future, child will need to poll for parent using kil(ppid, 0))
