#ifndef _ATOMIC_FORK_H_
#define _ATOMIC_FORK_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define af_transaction if (!af_transaction_begin(0))
#define af_timed(timeout) if (!af_transaction_begin(timeout))

#define AF_ABORT    -1
#define AF_TIMEOUT  -2
#define AF_SIGNAL   -3
#define AF_ERROR    -4


static struct sigaction old_handler;

static void af_handle_commit(int signum, siginfo_t *info, void *context){
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

int af_transaction_begin(int timeout) {
    if (timeout < 0){
        printf("Negative timeout\n");
        exit(-1);
    }
    af_setup_term_handler();

    pid_t pid = fork();
    if (pid == 0) {
        // child
        return 0;
    } else if (pid > 0){
        // parent
        int status = 0;
        int count = 0;

        while (count++ < (timeout ? timeout : 1)) {
            if (!waitpid(pid, &status, timeout ? WNOHANG : 0)){
                sleep(1);
                continue;
            }
            if (WIFEXITED(status)) return AF_ABORT;
            if (WIFSIGNALED(status)) return AF_SIGNAL;
            sleep(1);
        }
        kill(pid, SIGKILL);
        return AF_TIMEOUT;
    } else {
        // failed
        perror("Could not fork:");
        return AF_ERROR;
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
