/*====================================================================
 * Description: Know the usage of getopt() and getopt_long()
 *     the parameter wait to parse:
 *{    -m, --mode=MODE
             set file mode
       -v, --verbose
              print a message for each created directory
       --help display this help and exit
 *}
 * DATE: 2013/12/26
 * Modify:
 * Conclusion:
===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <getopt.h>

/* Macro definition */
#define false           0
#define true            1

/*
extern char *optarg;
extern int optind,  //the index of next element to be processed in argv 
            opterr, //a switch, 1 for print error message when get a not recognized option, 0 for not print error message
            optopt; //if get a not recognized option, store the option in optopt
int getopt(int argc, char * const argv[],
           const char *optstring); //a string containing the legitimate option characters.
*/

/*
int getopt_long(int argc, char * const argv[], //accept long option, start with 2 dashes.
           const char *optstring, // pointer to the 
                               struct option {
                                    const char *name;
                                    int         has_arg;
                                    int        *flag;
                                    int         val;
                                };
           const struct option *longopts, int *longindex);
*/

extern char *optarg;
extern int optind, opterr, optopt;

static struct option long_option[] = { 
        {"mode",    required_argument,  0, 0},
        {"verbose", no_argument,        0, 0},
        {"help",    no_argument,        0, 0}
    };

int
main(int argc, char **argv)
{
    int opt;
    char module[32];
    memset(module, 0x0, sizeof(module));
    while((opt = getopt(argc, argv, "m:vh")) != -1) { /* until to the end */
//        printf("getopt %d\n", opt);
        switch(opt) {
            case 'm':
                memcpy(module, optarg, strlen(optarg));
                printf("module: %s\n", module);
                break;
            case 'v':
                printf("version\n");
                break;
            case 'h':
                printf("help\n");
                break;
            default:
                printf("unknow argument %c\n", (char)opt);
                exit(EXIT_FAILURE);    
        }
    }
    return 0;
}

