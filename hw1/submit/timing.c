/*
*	timing.c
*
*	Written by: Matthew Carrington-Fair
*
*	This program runs and times an executable provided
*	as a command line argument a given number of times (indicated
*	by the global variable PROC_NUM). It prints the wall clock
*	time for the processes, along with the user and system time for 
*	both the caller and child processes.
*
*/

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <time.h>
/* Number of times to run the provided process */
static int PROC_NUM = 1;
/* Prints the timing results once all child processes are complete */
void print_results(clock_t start, clock_t end, struct tms st_cpu, struct tms en_cpu);

int main(int argc, char *argv[]) {
	/* Start and end times and tms structs to store various times */
	clock_t start;
	clock_t end;
	struct tms st_cpu;
	struct tms en_cpu;
	/* Process ID used when forking */
	pid_t pid;
	/* Status of whether child process is complete */
	int status;
	/* Arguments for the new process */
	char *envp [] = { NULL };
	char *args [] = {argv[1], NULL};
	/* Check to ensure correct number of args */
	if (argc != 2) {
		fprintf(stdout, "Error - not enough arguments\n");
		exit(EXIT_FAILURE);
	}
	int i;
	/* Begins timer */
	start = times(&st_cpu);
	/* Creates new child process each iteration */
	for (i = 0; i < PROC_NUM; i++) {
		pid = fork();
		/* Child */
		if (pid == 0) {
			execve(args[0], args, envp);
		}
		/* Parent */
		else if (pid < 0) {
			fprintf(stderr, "error forking");
			exit(EXIT_FAILURE);
		}
		else {
			/* Added computation in parent process to increase caller times */
			volatile unsigned long long i;
			for (i = 0; i < 1000000000ULL; ++i);
			/* Waits until child is done, then continues in loop */
			waitpid(pid, &status, 0);
			if (status == 0) continue;
		}
	}
	/* After all children are complete, ends timer */
	end = times(&en_cpu);
	/* Displays times */
	print_results(start, end, st_cpu, en_cpu);
	exit(EXIT_SUCCESS);
}

/* Prints time results for the child processes */
void print_results(clock_t start, clock_t end, struct tms st_cpu, struct tms en_cpu){

	end = times(&en_cpu);
	int ticks_per_second = sysconf(_SC_CLK_TCK); 
	printf("\nTotal executable time: %f\n", (double)(end - start) / ticks_per_second);
	printf("****** Calling Process ******\n");
	printf("User time of calling process: %f\n", ((double)(en_cpu.tms_utime - st_cpu.tms_utime)) / ticks_per_second);
	printf("System time of calling process : %f\n", ((double)(en_cpu.tms_stime - st_cpu.tms_stime)) / ticks_per_second);
	printf("****** Child Process ******\n");
	printf("User time of child: %f\n",((double)(en_cpu.tms_cutime - st_cpu.tms_cutime)) / ticks_per_second);
	printf("System time of child: %f\n", ((double)(en_cpu.tms_cstime - st_cpu.tms_cstime)) / ticks_per_second);
	printf("\n****** End times ******\n");
}