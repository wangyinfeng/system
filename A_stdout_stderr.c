/*====================================================================
 * Description: stderr has different output attribute with stdout
 * DATE: 2014/03/12
 * Modify:
 * Conclusion:
===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Macro definition */
#define false           0
#define true            1


int
main(int argc, char *argv[])
{
    /* by default, error message will print to stderr, instead of stdout
     * that's why need to use 2>&1 to merge stderr message to stdout when
     * collection program output with such command: ./a.out > log 2>&1.
     * ./a.out > log will only write stdout to the log file, while print
     * stderr to the console */
    fprintf(stderr, ("ERROR message\n"));
    fprintf(stdout, ("Normal message\n"));

    /* stdout and stdin is line-buffered, stderr is not buffered */
    while(1) {
        printf(".");
        fflush(stdout);
//        fprintf(stderr, ("."));
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}

