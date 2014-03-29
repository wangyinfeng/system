/*====================================================================
 * Description: Read the files under /proc are actually get dynamic info
 * from kernel. I'm interesting about the text parse, search line with 
 * strstr() and get word with the sscanf()...
 * DATE: 2014/03/27
 * Modify: 2014/03/28 - practice more string parse
 * Conclusion: Besides know how to get process related infomation
 * from the /proc, also know how to open/read files, prase the string output
 * by the strstr/strrchr...functions.
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
#include <sys/types.h>
#include <fcntl.h>

/* Macros */
#define false           0
#define true            1

/* The scanf() function reads input from the standard input stream stdin, 
 * fscanf() reads input from the stream pointer stream, 
 * and sscanf() reads its input from the character string pointed to by str.*/

/* char *strstr(const char *haystack, const char *needle);
 * The strstr() function finds the first occurrence of the substring needle 
 * in the string haystack. The terminating null bytes are not compared. 
 *
 * The strrchr() function returns a pointer to the last occurrence of the 
 * character c in the string s.*/

/* ssize_t readlink(const char *restrict path, char *restrict buf, size_t bufsize); 
 * read the contents of a symbolic link*/


#define ADDR_STR    "cache_alignment" 

void
print_argument(pid_t pid)
{
    int fd;
    char process_name[32];
    char arguments[1024];
    char* next_arg;
    int argu_length;

    snprintf(process_name, sizeof(process_name), "/proc/%d/cmdline", (int)pid);
    /* the fopen()/fclose()/fread() use FILE* fp, the open()/read()/close() use int fd */
    fd = open(process_name, O_RDONLY);
    argu_length = read(fd, arguments, sizeof(arguments));
    close(fd);
    
    arguments[argu_length] = '\0';
    next_arg = arguments;
    while(next_arg < arguments+argu_length) {
        printf("%s\n", next_arg);
        next_arg += strlen(next_arg)+1;
    }
}

void
print_environment(pid_t pid)
{
    /* the same as argument parse */
}

int
print_exec_path(pid_t pid)
{
    char process_name[32];
    char process_path[64];
    char* path_end;

    snprintf(process_name, sizeof(process_name), "/proc/%d/exe", (int)pid);
    if (readlink(process_name, process_path, sizeof(process_path)) <= 0) {
        fprintf(stderr, "read the symbolic link failed\n");
        return 1;
    }
    path_end = strrchr(process_path, '/');
    if (path_end == NULL) {
        fprintf(stderr, "Not valid path\n");
        return 1;
    }
    ++path_end;
    *path_end = '\0';
    printf("The process %d running from directory %s\n", (int)pid, process_path);

    return 0;
}

int
print_cpu_info(void)
{
    FILE* fp;
    char buffer[1024];
    int buffer_size;
    char* addr_size_str;
    char addr_size;

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

}

void
print_up_time(void)
{
    FILE* fp;
    double up_time, idle_time;
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
}

int
main(int argc, char **argv)
{
    pid_t pid;
    if (argc < 2) {
        fprintf(stderr, "Please specify the target pid\n");
        return 1;
    }
    pid = (pid_t)atoi(argv[1]);

    print_cpu_info();
    print_up_time();
    print_argument(pid);
    print_environment(pid);
    /* get self path /proc/self/exe is OK, get other process's info require
     * root power */
    print_exec_path(pid);

    return 0;
}

