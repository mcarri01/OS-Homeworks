/*
*	sysproc.c
*	Written by: Matthew Carrington-Fair
*	
*	This program compares an increasingly complex group of procedure
*	functions to the system call getpid(). It loops each procedure 
*	1,000,000,000 times in order to have non-trivial data to compare.
*	
*	Analysis of performance is in analysis.txt
*
*
*/
/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
/* Measurement struct to keep times */
struct measurements {
	struct timespec start, end;
};
int PROC_COUNT = 0;
int BILLION = 1000000000;
/* System and procedure calls */
void system_call();
void procedureOne();
void procedureTwo();
void procedureThree();
void procedureFour();
void procedureFive();
/* Reports results from functions */
void print_times(struct measurements timers);
double diff(struct measurements timers);

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	system_call();
	procedureOne();
	procedureTwo();
	procedureThree();
	procedureFour();
	procedureFive();

}
/* Performs getpid() system call to get initial measurement */
void system_call() {
	int i;
	struct measurements timers;
	clock_gettime(CLOCK_MONOTONIC, &timers.start);
	for (i = 0; i < BILLION; ++i){
		getpid();
	}
	clock_gettime(CLOCK_MONOTONIC, &timers.end);
	print_times(timers);
}

/*Procedure simply loops the same amount as with getpid, w/o any computation*/
void procedureOne() {
	int i;
	struct measurements timers;
	clock_gettime(CLOCK_MONOTONIC, &timers.start);
	for (i = 0; i < BILLION; ++i);
	clock_gettime(CLOCK_MONOTONIC, &timers.end);
	print_times(timers);
}
/* Procedure adds variables init and conditional */
void procedureTwo() {
	int i;
	struct measurements timers;
	clock_gettime(CLOCK_MONOTONIC, &timers.start);
	for (i = 0; i < BILLION; ++i){
		int num = i;
		if (num == num) continue;
	}
	clock_gettime(CLOCK_MONOTONIC, &timers.end);
	print_times(timers);
}
/* Extra varialbe init and extra arithmetic in conditional */
void procedureThree() {
	int i;
	struct measurements timers;
	clock_gettime(CLOCK_MONOTONIC, &timers.start);
	for (i = 0; i < BILLION; ++i){
		int num = i;
		int num2 = 1;
		if (num == (num - num2) + 1) continue;
	}
	clock_gettime(CLOCK_MONOTONIC, &timers.end);
	print_times(timers);
}
/* Extra "+=" operation and conditional to Procedure Three */
void procedureFour() {
	int i;
	struct measurements timers;
	clock_gettime(CLOCK_MONOTONIC, &timers.start);
	for (i = 0; i < BILLION; ++i){
		int num = i;
		int num2 = 1;
		if (num == (num - num2) ) {
			continue;
		}
		else {
			num2 += num;
			if ((num + 1) == num2) continue;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &timers.end);
	print_times(timers);
	
}
/* Additional "+=" operation to Procedure Four */
void procedureFive() {
	int i;
	struct measurements timers;
	clock_gettime(CLOCK_MONOTONIC, &timers.start);
	for (i = 0; i < BILLION; ++i){
		int num = i;
		int num2 = 1;
		if (num == (num - num2) ) {
			continue;
		}
		else {
			num2 += num;
			if ((num + 1) == num2) 
				num++;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &timers.end);
	print_times(timers);
}
/* Reports times for each procedure */
void print_times(struct measurements timers) {
	/* Used to divide time once it has been multiplied by 1,000,000 */
	if (PROC_COUNT == 0) {
		printf("\nPrinting System Call (getpid)\n");
	}
	else {
		printf("\nPrinting Procedure: %d\n", PROC_COUNT);
	}
	double times = diff(timers);
	printf("Time: %g seconds\n", times);
	PROC_COUNT++;
}
double diff(struct measurements timers)
{
	/* Multiply seconds by a billion to incorporate nanoseconds */
	double start = (timers.start.tv_sec * BILLION) + timers.start.tv_nsec;
	double end = (timers.end.tv_sec * BILLION) + timers.end.tv_nsec;
	/* Then return difference divided by a billion to get precise seconds */
	return (end - start) / BILLION;
}