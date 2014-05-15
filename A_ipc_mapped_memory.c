/*====================================================================
 * Description: Practice about mapped memory IPC
 * DATE: 2014/05/15
 * Modify:
 * Conclusion:
 * - No race condition need to consider about?
 *   - Same as shared memory, request race condition manually.
 *     or use fcntl to place a write/read lock.
 * - Program auto terminated after write something to the file?
 *   no need to call munmap()?
 * - advanced usage of mmap  
 *   - work with /de/zero, an effeicent allocate a large 
 *     size(pages) initialized memory
 *   - data struct?   
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
#include <sys/mman.h>
#include <fcntl.h>

/* Macros */
#define false           0
#define true            1

void
dump_mem(void *begin, int len)
{
    int i = 0;
    if (begin == NULL)
        return;
    printf("Dump contents from address %p, length 0x%x\n", begin, len);
    while(i < len) {
        if (i%16 == 0) {
            printf("\n%p: ", begin+i);
        }
        printf("%02x ", 0xFF&((char*)begin)[i]);
        i++;
    }
    printf("\n");
}

int
main(int argc, char **argv)
{
    int fd;
    void *file_memory;
    int mem_len = getpagesize();

/* An efficient way to allocate a large size of memory and initialize it */
//    printf("the sizeof pointer on x64: %dBytes\n", sizeof(file_memory));
    fd = open("/dev/zero", O_RDWR, S_IRUSR);
    file_memory = mmap(NULL, mem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (file_memory == MAP_FAILED) {
        perror("Error: mmap");
        exit(1);
    }
    dump_mem(file_memory, mem_len);
    munmap(file_memory, mem_len);
//    No need to free
//    free(file_memory);

    return 0;
}

