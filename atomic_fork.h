#ifndef _ATOMIC_FORK_H_
#define _ATOMIC_FORK_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define af_transaction if (af_transaction_begin())

static struct sigaction old_handler;

static void af_handle_commit(int signum, siginfo_t *info, void *context){
    printf("Transaction commited\n");
    exit(0);
}

static void af_setup_term_handler(){
	static struct sigaction sa;

	sa.sa_sigaction = af_handle_commit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;

    // TODO make sure the handler is not set twice
	if (sigaction(SIGTERM, &sa, &old_handler) < 0){
		perror("Signal Handler Installation Failed:");
		abort();
	}
}

int af_transaction_begin() {
    af_setup_term_handler();

    pid_t pid = fork();
    if (pid == 0) {
        // child
        return 1;
    } else if (pid > 0){
        // parent
        int status = 0;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Transaction aborted gracefully\n");
        } else if (WIFSIGNALED(status)) {
            printf("Transaction aborted due to signal\n");
        }

        return 0;
    } else {
        // failed
        perror("Could not fork:");
        return 0;
    }
}

void af_commit() {
    // this will kill any parent... be careful...
    // TODO add check to only kill the monitor parent
    pid_t ppid = getppid();
    kill(ppid, SIGTERM);
}

void af_rollback() {
    exit(-1);
}

#endif //_ATOMIC_FORK_H_
