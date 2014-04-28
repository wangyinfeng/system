/*====================================================================
 * Description: using timer to print some sysinfo. know how to reguster
 * signal handler.
 * DATE: 2014/04/28
 * Modify:
 * Conclusion:
===================================================================*/
/* includes */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/kernel.h>
//#include <linux/sys.h>
#include <sys/sysinfo.h>
#include <signal.h>
#include <fcntl.h>

/* Macros */
#define false           0
#define true            1

void timer_handler(int signum)
{
    static int i = 0;
    struct sysinfo si;
    /* get system info */
    sysinfo(&si);

    printf("%d: System uptime %d\tTotal RAM %f\tFree RAM %fMB\tProcess count %d\n", 
            ++i, si.uptime, si.totalram, si.freeram, si.procs);
}

int
main(int argc, char **argv)
{
    struct sigaction sa;
    struct itimerval timer;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    /* specify the action to be associated with a specific signal */
    sigaction(SIGVTALRM, &sa, NULL);

    /* start 3 seconds later */
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;

    /* time interval is 1 second */
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    while(1);
    return 0;
}

