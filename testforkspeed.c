#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

// seems like parent fork call returns almost twice as fast !!!
// potential to improve transaction speed
int main(int argc, char *argv[]) {
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	pid_t pid = fork();
	gettimeofday(&tv2, NULL);
	if (pid == 0){
		printf ("Time for child = %f microseconds\n", (double) (tv2.tv_usec - tv1.tv_usec));
	} else {
		printf ("Time for parent = %f microseconds\n", (double) (tv2.tv_usec - tv1.tv_usec));
	}

}
