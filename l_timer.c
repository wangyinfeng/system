/*====================================================================
 * Description: * To know the usage of timer.
 *              * The detail of itimerval structure.
 *              * How the signal handler work
 * DATE: 2013/11/16
 * Modify:
 * Conclusion:
===================================================================*/
/* include */
#include <string.h>
#include <stdio.h>  
#include <time.h>  
#include <sys/time.h>  
#include <stdlib.h>  
#include <signal.h>  

/* Macro definition */
#define false           0
#define true            1

static int count = 0;  
static struct itimerval oldtv;  
  
void set_timer()  
{  
    struct itimerval itv;  
    itv.it_interval.tv_sec = 1;  
    itv.it_interval.tv_usec = 0;  
    itv.it_value.tv_sec = 1;  
    itv.it_value.tv_usec = 0;  
    setitimer(ITIMER_REAL, &itv, &oldtv);  
}  
  
void signal_handler(int m)  
{  
    count ++;  
    printf("%d\n", count);  
}  
  
int main()  
{  
    signal(SIGALRM, signal_handler);  
    set_timer();  
    while(count < 10000);  
    exit(0);  
    return 1;  
}  

