/*====================================================================
 * Description: Shared memory
 * DATE: 2014/03/26
 * Modify:
 * Conclusion:
 * - By this example, the shared memory is created before fork, but 
 *   what if the shared memory is created after fork? 
 * - Processes must coordinate access to the shared memory, need 
 *   semaphore's help
 *   - there are two kinds of semaphores, counting semaphore and 
 *   binary semaphore. For the semaphore get by semget(), is binary
 *   semaphore, cannot be used by sem_wait() and sem_post(), and 
 *   should be initialized by its own way.
 *   TODO:Implement the binary semaphore at the page 126.
===================================================================*/
/* includes */
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

/* Macro definition */
#define false           0
#define true            1

int
main(int argc, char **argv)
{
    pid_t child;
    int segment_id;
    char* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    /* be sure allocate segment size round up to the page size(4k?) */
    const int shared_segment_size = getpagesize()*2;

    /* create a new shared memory */
    segment_id = shmget(IPC_PRIVATE, shared_segment_size,
            IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR); /* new, read and write able*/

    child = fork();
    if (child < 0) {
        fprintf(stderr, "Fail to fork new process\n");
        return 0;
    }
    else if (child == 0) {
        /* attach the shared memory, read something from the shared memory, 
         * then detach it, exit */ 
        printf("Child process %d running...\n", getpid());

        shared_memory =(char*)shmat(segment_id, 0, 0);
        shmctl(segment_id, IPC_STAT, &shmbuffer);
        /* what's the detail of the shmid_ds? */ 
        segment_size = shmbuffer.shm_segsz;
        printf("shared memory attached at address %p, size %d\n", shared_memory, segment_size);

        /* read something from the segment */
        printf("%s", shared_memory);

        exit(0);
    }
    else {
        /* create a shared memory, write something to it, after child process 
         * terminated, deallocate it */
        printf("Father process %d running, its child is %d ...\n", getpid(), (int)child);
        /* attach the segment, the 2nd parment be 0 means let system mapping the segment to its virtual memory automatally */
        shared_memory =(char*)shmat(segment_id, 0, 0);

        /* get the segment status */
        shmctl(segment_id, IPC_STAT, &shmbuffer);
        segment_size = shmbuffer.shm_segsz;
        printf("shared memory attached at address %p, size %d\n", shared_memory, segment_size);

        /* write something */
        sprintf(shared_memory, "Hellow world from process %d\n", getpid());

        /* detach the segment */
        shmdt(shared_memory);

        wait(NULL);
        printf("Child has done. END\n");

        /* after all process detach the segment, deallocate it */
        shmctl(segment_id, IPC_RMID, 0);
    }

    return 0;
}

