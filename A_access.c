/*====================================================================
 * Description: the access() to check file permissions. Specify the file
 * name from cli, check if the file exist, writable, readable or executable.
 * DATE: 2014/04/01
 * Modify:
 * Conclusion:
 *  If debug with gdb, be careful about the arguments. The usage of gdb:
 *      gdb [options] [executable-file [core-file or process-id]]
 *      gdb [options] --args executable-file [inferior-arguments ...]
 *  should type
 *      gdb --args ./a.out THE_FILE    
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
#include <fcntl.h>

/* Macros */
#define false           0
#define true            1

#define MSG     "Hello"

int
main(int argc, char *argv[])
{
    int rv;
    printf("argc %d, argv-1 %s, argv-2 %s\n", argc, argv[0], argv[1]);
    char* filename = argv[1];
    FILE* fd;
    char buffer[64];
    int buffer_size;
    if (filename == NULL) {
        printf("No file specified\n");
        return 1;
    }

    rv = access(filename, F_OK);
    if(rv == 0) {
        printf("%s exist\n", filename);
    }
    else {
        if (rv == ENOENT) {
            printf("%s not exist\n", filename);
        }
        else if (rv == EACCES) {
            printf("%s not accessable\n", filename);
        }
        else {
            printf("not specified error number %d\n", rv);
        }
        return 1;
    }

    rv = access(filename, W_OK);
    if (rv == 0) {
        printf("%s is writable. Writing \"%s\" to the file\n",filename, MSG);
        fd = fopen(filename, "w");
        fwrite(MSG, 1, strlen(MSG), fd);
        fclose(fd);
        /* open file with append mode */
        fd = fopen(filename, "a");
        fwrite("\n", 1, strlen("\n"), fd);
        fclose(fd);
    }
    else {
        if (rv == EACCES) {
            printf("%s not accessable\n", filename);
        }
        else if (rv == EROFS) {
            printf("%s is read-only\n", filename);
        }
        else {
            printf("not specified error number %d\n", rv);
        }
        return 1;
    }

    rv = access(filename, R_OK);
    if (rv == 0) {
        printf("%s is readable, reading context out...\n", filename);
        fd = fopen(filename, "r");
        buffer_size = fread(buffer, 1, sizeof(buffer), fd);
        fclose(fd);
        if (buffer_size == 0 || buffer_size == sizeof(buffer)) {
            printf("Read context from file %s failed\n", filename);
            return 1;
        }
        /* \r return to the header of current line 
         * \b rollback one character */
        /* Not possible to return to the last line? */
        printf("Context from file %s is \"%s\"\n", filename, buffer);
    }
    else {
        if (rv == EACCES) {
            printf("%s not accessable\n", filename);
        }
        else {
            printf("not specified error number %d\n", rv);
        }
        return 1;
    }

    return 0;
}

