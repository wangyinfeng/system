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
 * Modify: 2014/03/12 implement with getopt_long();
 * Conclusion: life is short, save the time, use existed tools
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

extern char *optarg;
extern int optind, opterr, optopt;

const char* short_option = "m:vh";
static struct option long_option[] = { 
        {"module",  required_argument,  NULL, 'm'},
        {"version", no_argument,        NULL, 'v'},
        {"help",    no_argument,        NULL, 'h'},
        {NULL,      0,                  NULL, 0} /* mandatory at the end */
    };
/* {full-name, have-argument? 1:0, ?, short-name} */

/* accept long option, start with 2 dashes.
- parse the options one by one, return the short option
- return -1 if no more options
- return ? if option not in scope

int getopt_long(int argc, char * const argv[], 
            const char *optstring, // pointer to the short options  
            const struct option *longopts, 
                               // pointer to the long options
                               struct option {
                                    const char *name;
                                    int         has_arg;
                                    int        *flag;
                                    int         val;
                                };
            int *longindex);
*/

int
main(int argc, char **argv)
{
    int opt;
    char module[32];
    memset(module, 0x0, sizeof(module));
    if (argc < 2)
        printf("usage: \n" /* use "" to combine multiline is better than use '\' */
        "    -m/--module MODULE: show module\n"
        "    -v/--version: show verion\n"
        "    -h/--help: show help info\n");
#if USE_GETOPT
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
            case '?': /* parameter not in scope will return '?' */
                printf("unknow parameter\n");
                break;
            default:
                printf("unknow argument %c\n", (char)opt);
                exit(EXIT_FAILURE);    
        }
    }
#endif
    while((opt = getopt_long(argc, argv, "m:vh", long_option, NULL)) != -1) { /* until to the end */
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
            case '?': /* parameter not in scope will return '?' */
                printf("unknow parameter: %c\n", (char)opt);
                break;
            default:
                printf("unknow argument %c\n", (char)opt);
                exit(EXIT_FAILURE);    
        }
    }
    return 0;
}


