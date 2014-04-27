/*====================================================================
 * Description: Know the high performance file send api
 * DATE: 2014/04/27
 * Modify:
 * Conclusion: using sendfile(), no need to require addtional memory
 * allocting.
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
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Macros */
#define false           0
#define true            1

/* copy the running file to local with the name xxx.copy */
int
main(int argc, char **argv)
{
    int read_fd;
    int write_fd;
    struct stat stat_buf;
    off_t offset = 0;
    char copy_file[32];
    sprintf(copy_file, "%s"".copy", argv[0]);
//    printf("Copying file %s to %s \n", argv[0], copy_file);
    /* Open the input file. */
    read_fd = open (argv[0], O_RDONLY);
    /* Stat the input file to obtain its size. */
    fstat (read_fd, &stat_buf);
    /* Open the output file for writing, with the same permissions as the
     * source file. */
    write_fd = open (copy_file, O_WRONLY | O_CREAT, stat_buf.st_mode);
    /* Blast the bytes from one file to the other. */
    sendfile (write_fd, read_fd, &offset, stat_buf.st_size);
    /* Close up. */
    close (read_fd);
    close (write_fd);
    return 0;
}

