/*====================================================================
 * Description: Read the files under /proc are actually get dynamic info
 * from kernel. I'm interesting about the text parse, search line with 
 * strstr() and get word with the sscanf()...
 * DATE: 2014/03/27
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

/* The scanf() function reads input from the standard input stream stdin, 
 * fscanf() reads input from the stream pointer stream, 
 * and sscanf() reads its input from the character string pointed to by str.*/

/* char *strstr(const char *haystack, const char *needle);
 * The strstr() function finds the first occurrence of the substring needle 
 * in the string haystack. The terminating null bytes are not compared. */

#define ADDR_STR    "cache_alignment" 

int
main(int argc, char **argv)
{
    FILE* fp;
    char buffer[1024];
    int buffer_size;
    char* addr_size_str;
    char addr_size;
    double up_time, idle_time;

    fp = fopen("/proc/cpuinfo", "r");
    buffer_size = fread(buffer, 1, sizeof(buffer), fp);
    if (buffer_size == 0 || buffer_size == sizeof(buffer)) {
        fprintf(stderr, "Read info from cpuinfo failed\n");
        return 1;
    }
    fclose(fp);

    addr_size_str = strstr(buffer, ADDR_STR);
    if (addr_size_str == NULL) {
        fprintf(stderr, "There are no string %s\n", ADDR_STR);
        return 1;
    }
    sscanf(addr_size_str, "cache_alignment : %d", &addr_size);
    printf("Parse the cpuinfo and get the cpu address is : %d\n", addr_size);

    /* convert the uptime to readable */
    fp = fopen("/proc/uptime", "r");
    /* The way to get the output of the command */
    /* no matter how many spaces between the %lf, will get the correct value.
     * What if want to get the number in the output like: "A 123, B 456"? */
    fscanf(fp, "%lf %lf", &up_time, &idle_time); 
    fclose(fp);
    printf("System up time: %d Day:%d Hour:%d Min:%d Sec, idle time %f\n",
            (long)up_time/24/60/60, 
            (long)up_time/60/60%24, 
            (long)up_time/60%60, 
            (long)up_time%60, idle_time);

    return 0;
}

