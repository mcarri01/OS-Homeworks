#include <stdio.h> 
#include <sys/times.h>   
#include <unistd.h> 
#include <limits.h> 
/////////////////////////////////////
// use the "times" system call to measure user and system time for a process. 
// WARNING: this is lower-resolution than other mechanisms due to "legacy code" 
// issues. getrusage is higher-resolution. 
////////////////////////////////////
   // struct tms {
   //   clock_t tms_utime;  /* user */
   //   clock_t tms_stime;  /* system */
   //   clock_t tms_cutime; /* user of children */
   //   clock_t tms_cstime; /* system of children */
   // };
   // typedef long int clock_t; // on IA64 */ 

   // clock_t times(struct tms *buf);
int main () { 
    struct tms buf; 
    clock_t sinceboot; 
    // chew up some CPU time
    int i,j; 
    for (i=0,j=0; i<100000000; i++) { j+=i*i; }     
    // measure elapsed time  
    sinceboot = times(&buf); 
    printf("elapsed: %ld", sinceboot);
    printf("user: %ld ticks\n", buf.tms_utime); 
    printf("system: %ld ticks\n", buf.tms_stime); 
    int ticks_per_second = sysconf(_SC_CLK_TCK); 
    printf("%d ticks per second\n",ticks_per_second); 
    double usec = (double)buf.tms_utime / (double) ticks_per_second; 
    double ssec = (double)buf.tms_stime / (double) ticks_per_second; 
    printf("user time = %g seconds\n", usec); 
    printf("system time = %g seconds\n", ssec); 
} 