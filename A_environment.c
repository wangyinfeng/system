/*====================================================================
 * Description: Environment variables are commonly used to communicate 
 * configuration information to programs.
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

/* use environ to enumerate environment variables */
extern char** environ;

int
main(int argc, char *argv[])
{
    char** var;
    for (var = environ; *var != NULL; ++var)
        printf("%s\n", *var);

    /* well, program can get environment variable by these functions,
     * if want to read configuration files, need to decode by itself? */
    char *user = getenv("USER");
    if (user == NULL)
        printf("Ghost is login\n");
    else
        printf("%s is login\n", user);
    
    exit(EXIT_SUCCESS);
}

