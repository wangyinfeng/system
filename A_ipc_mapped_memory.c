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
 *   - faster file access, reduce memory copies
 *   - work with /de/zero, an effeicent allocate a large 
 *     size(pages) initialized memory
 *   - data struct?
 *   - when use malloc for large size of memory, usually internally
 *     use the mmap to implement.
 *   - mmap also allows the operating system to optimize paging operations. 
 *     For example, consider two programs; program A which reads in a 1MB file
 *     into a buffer creating with malloc, and program B which mmaps the 1MB 
 *     file into memory. If the operating system has to swap part of A's 
 *     memory out, it must write the contents of the buffer to swap before it 
 *     can reuse the memory. In B's case any unmodified mmap'd pages can be 
 *     reused immediately because the OS knows how to restore them from the 
 *     existing file they were mmap'd from. (The OS can detect which pages 
 *     are unmodified by initially marking writable mmap'd pages as read only 
 *     and catching seg faults, similar to Copy on Write strategy).  
 * - disadvantage
 *   - require continguous address space
 *   - require page align    
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
#define FILE_LOCK       1
#define FILE_UNLOCK     0

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
file_lock_oper(int fd, int oper)
{
    int result;
    struct flock fl;
    if (oper == FILE_LOCK)
        fl.l_type = F_WRLCK; /* F_WRLCK, F_RDLCK, F_UNLCK */
    else
        fl.l_type = F_UNLCK;

    fl.l_whence = SEEK_SET;
    fl.l_start = 0; /* from whence */
    fl.l_len = 0; /* to the EOF */
    fl.l_pid = getpid();

/* F_SETLKW - attempt to obtain the lock requested in the struct flock 
 * structure. If the lock cannot be obtained (since someone else has it 
 * locked already), fcntl() will wait (block) until the lock has cleared, 
 * then will set it itself.  
 * F_SETLK - will not wait if it cannot obtain a lock. 
 * It will return immediately with -1. 
 * F_GETLK - check to see if there is a lock*/
    if (oper == FILE_LOCK)
        result = fcntl(fd, F_SETLKW, &fl);
    else 
        result = fcntl(fd, F_SETLK, &fl);

    if (result == -1) {
        perror("Error: fcntl\n");
        exit(1);
    }
    else
        return result;
}

int
main(int argc, char **argv)
{
    int fd;
    void *file_memory;
    int mem_len = getpagesize();
    pid_t child;
    int file_len;

#if defined MEM_TEST
/* An efficient way to allocate a large size of memory and initialize it */
//    printf("the sizeof pointer on x64: %dBytes\n", sizeof(file_memory));
    fd = open("/dev/zero", O_RDWR, S_IRUSR);
    /* the memory space should be multiple of the page size of current system
     * if the length parament is not multiple of page size, will round up */
    file_memory = mmap(NULL, mem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (file_memory == MAP_FAILED) {
        perror("Error: mmap");
        exit(1);
    }
    dump_mem(file_memory, mem_len);
    munmap(file_memory, mem_len);
/* No need to free 
 * mmap-munmap are just like allocate-free */
//    free(file_memory);
#endif
    /* the parent process open a file(mapped_memory) in current directory, 
     * if not exist, create it. then mmap to memory, write the source file 
     * size to it. The child process read the number out, and double it,
     * then write back. */
    if (argc < 2) {
        perror("Please specify the file name\n");
        exit(1);
    }
    fd = open(argv[1], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (fd == -1) {
        perror("open\n");
        exit(1);
    }
    /* without lseek seems not able to access the file, why? */
    /* these operations actually allocate the file with specified size,
     * otherwise the file size is 0 */
    lseek(fd, mem_len+1, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);

    file_len = lseek(fd, 0, SEEK_END);
    file_memory = mmap(NULL, mem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    child = fork();
    if (child < 0) {
        perror("fork\n");
        exit(1);
    }
    else if (child == 0) {
        printf("Child process %d running...parent is %d. locking the file\n", 
                getpid(), getppid());
        file_lock_oper(fd, FILE_LOCK);
        sscanf(file_memory, "%d", &file_len);
        printf("Child reading number from file %d, double it\n", file_len);
        sprintf((char*)file_memory, "%d\n", file_len*2);
        file_lock_oper(fd, FILE_UNLOCK);
        exit(0);
    }
    else {
        printf("Parent process %d running...locking the file\n", getpid());
        file_lock_oper(fd, FILE_LOCK);
        sprintf((char*)file_memory, "%d\n", file_len);
        sleep(5);
        printf("..... Parent writting done.");
        file_lock_oper(fd, FILE_UNLOCK);

        wait(NULL);
        printf("Child done. unmap the file\n");
        close(fd);
        munmap(file_memory, mem_len);
    }

    return 0;
}

