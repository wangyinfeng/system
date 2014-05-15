/*====================================================================
 * Description: Have saw many times the useage of lseek, code some
 * example to better understand it.
 * 
 * DATE: 2014/05/15
 * Modify: Change the source file to get file length, more common
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
#include <fcntl.h>

/* Macros */
#define false           0
#define true            1

#define FILENAME "A_get_filesize.c"

/* There are several ways to get a file size */
int
main(int argc, char **argv)
{
    int length = 0;
    int fd = open(FILENAME, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: open\n");
        exit(1);
    }

/* off_t lseek(int fildes, off_t offset, int whence);
 * set the file offset for the open file description associated with the file 
 * descriptor fildes, as follows:
 * If whence is SEEK_SET, the file offset shall be set to offset bytes.
 * If whence is SEEK_CUR, the file offset shall be set to its current location plus offset.
 * If whence is SEEK_END, the file offset shall be set to the size of the file plus offset.*/
    length = (int)lseek(fd, 0, SEEK_END);
    printf("Get file %s length %d by lseek\n", FILENAME, length);

/* int fseek(FILE *stream, long offset, int whence);
 * similar with the lseek. Not return the length directly, use ftell to
 * get the current value of the file position indicator for the stream 
 * pointed to by stream. */
    /* int fileno(FILE *stream); examines the argument stream and returns its integer descriptor.
     * FILE *fdopen(int fd, const char *mode); associates a stream with the existing file descriptor, fd. */    
    FILE *f = fdopen(fd, "r");
    if (f == NULL) {
        fprintf(stderr, "Error: fdopen\n");
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET); /* restore the position indicater */
    printf("Get file %s length %d by fseek\n", FILENAME, length);

/* int fstat(int fildes, struct stat *buf); */    
    struct stat fst;
    fstat(fd, &fst);
    length = fst.st_size;
    printf("Get file %s length %d by fstat\n", FILENAME, length);
/* Or use int stat(const char *restrict path, struct stat *restrict buf); */
    struct stat st;
    stat(FILENAME, &st);
    length = st.st_size;
    printf("Get file %s length %d by stat\n", FILENAME, length);

    close(fd);
    return 0;
}

