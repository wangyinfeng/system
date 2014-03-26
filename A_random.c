/*====================================================================
 * Description: use /dev/random to gernate random number
 * DATE: 2014/03/26
 * Modify:
 * Conclusion:
===================================================================*/
/* includes */
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/shm.h>

/* Macros */
#define false           0
#define true            1

#define MAX 1024
#define MIN 100

int
main(int argc, char **argv)
{
    char *random_byte;
    unsigned int random;
    static int random_fd = -1;
    size_t size = sizeof(random);

    /* just use open and read it as it is a file */
    /* use /dev/random to generate random based on the input interval(key board or mouse)
     * so it may blocked if there no input. */
    random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        /* /dev/urandom also generate random number, but not so 'random' as this one */
        fprintf(stderr, "Open /dev/random failed\n");
        return 0;
    }
    else {
        random_byte = (char*)&random;
        do {
            int read_byte;
            read_byte = read(random_fd, random_byte, size);
            size -= read_byte;
            random_byte += read_byte;
        }
        while(size >0);
    }
    random = MIN + (random % (MAX-MIN+1));
    printf("Get random between %d-%d is %d\n", MAX, MIN, random);
    
    return 0;
}
