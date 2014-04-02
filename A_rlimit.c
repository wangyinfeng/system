/*====================================================================
 * Description: Practice the getrlimit() and setrlimit() system calls.
 * RLIMIT_CPU - The maximum CPU time, in seconds
 * RLIMIT_DATA - The maximum bytes of memory that a program can allocate.
 *               If this limit is exceeded, the brk(), malloc() and sbrk() 
 *               functions will fail with errno set to [ENOMEM].
 * RLIMIT_NPROC - The maximum number of child processes
 * RLIMIT_NOFILE - The maximum number of file descriptors
 *
 * DATE: 2014/04/02
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
#include <sys/resource.h>
#include <sys/time.h>
#include <fcntl.h>


/* Macros */
#define false           0
#define true            1

int
main(int argc, char **argv)
{
    /* rlimit structure has two fields: rlim_cur is the soft limit, 
     * and rlim_max is the hard limit. */
    struct rlimit rl;
    char* buffer;
    /* Try RLIMIT_CPU and RLIMIT_DATA. */
    getrlimit(RLIMIT_DATA, &rl);
    printf("Current memory limit %d\t Max memory limit %d\n", 
            rl.rlim_cur, rl.rlim_max);
    rl.rlim_cur = 1024000;
    setrlimit(RLIMIT_DATA, &rl);
    printf("Update memory limit %d\t Max memory limit %d\n", 
            rl.rlim_cur, rl.rlim_max);
    buffer = malloc(2048000);
    if (buffer == NULL) {
        printf("malloc failed\n");
    }
    else {
        if (errno == ENOMEM)
            printf("success? buffer %p errno %d\n", buffer, errno);
    }
    /* Strange it's not work for RLIMIT_DATA.
     * Need to check the errno besides the return value of malloc,
     * because even malloc return not NULL, the allocation may also
     * not successed. 
     * And when set the RLIMIT_DATA, have request for the minize 
     * data block size? for example 4k based? why it's not work here? */

    getrlimit(RLIMIT_CPU, &rl);
    printf("Current cpu time limit %d\t Max cpu time limit %d\n", 
            rl.rlim_cur, rl.rlim_max);
    rl.rlim_cur = 1; /* 1 second */
    setrlimit(RLIMIT_CPU, &rl);
    printf("Update cpu time limit %d\t Max cpu time limit %d\n", 
            rl.rlim_cur, rl.rlim_max);
    while(1);

    return 0;
}

