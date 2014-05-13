/*====================================================================
 * Description: Shared memory
 * DATE: 2014/05/13
 * Modify: Add binary semaphore
 * Conclusion:
 * - By this example, the shared memory is created before fork, but 
 *   what if the shared memory is created after fork? 
 * - Processes must coordinate access to the shared memory, need 
 *   semaphore's help
 *   - there are two kinds of semaphores, counting semaphore and 
 *   binary semaphore. For the semaphore get by semget(), is binary
 *   semaphore, cannot be used by sem_wait() and sem_post(), and 
 *   should be initialized by its own way.
 *
 *   Usage of binary semaphore: simply use semget to associate or
 *   create a semaphore, then use semctl to initialize or delete
 *   the semaphore, and use semop to lock or release the resource.
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

/* why need to define the semun? */
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
};

#if defined EXAMPLE
void create_semid(int *semid)
{
    int nsems;
    int semflg;

    nsems = 1;
    semflg = IPC_CREAT | 0666;
    *semid = semget(1, nsems, semflg);
}

void rm_semid(int semid)
{
    int i;
    int semflg;

    semflg = IPC_RMID;
    i=semctl(semid,0,semflg,0);
    printf("#%d\n",i);
}

void up(int semid)
{
    int i;
    struct sembuf sembuf;

    sembuf.sem_num = 0;
    sembuf.sem_op = 1;
    sembuf.sem_flg = 0;
    i = semop(semid,&sembuf, 1);
    if (i == -1) perror("Up semop");
}

void down(int semid)
{
    int i;
    struct sembuf sembuf;

    sembuf.sem_num = 0;
    sembuf.sem_op = -1;
    sembuf.sem_flg = 0;
    i = semop(semid,&sembuf, 1);
    if (i == -1) perror("Down semop");
}
#endif

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

    union semun init_argument;
    unsigned short values[1];
    key_t sem_key;
    int semid;
    struct sembuf oper;
    /* Use the first (and only) semaphore. */
    oper.sem_num = 0;
    /* increment by 1. */
    oper.sem_op = 1;
    /* Permit undo’ing. */
    oper.sem_flg = SEM_UNDO;

    /* Get unique key for semaphore. */
    /* The ftok() function uses the identity of the file named by the given 
     * pathname (which must refer to an existing, accessible file) and the 
     * least significant 8 bits of proj_id (which must be nonzero) to 
     * generate a key_t type System V IPC key */
    if ((sem_key = ftok("/tmp", 'a')) == (key_t) -1) {
        perror("IPC error: ftok"); exit(1);
    }

    if ((semid = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR |
                    S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1) {
        perror("IPC error: allocate semaphore\n"); exit(1);
    }
    else { /* initialize semaphore */
        values[0] = 1;
        init_argument.array = values;
        semctl (semid, 0, SETALL, init_argument);
    }

    /* create a new shared memory */
    segment_id = shmget(IPC_PRIVATE, shared_segment_size,
            IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR); /* new, read and write able*/

    /*
     * semaphore already been initialized by semctl(), set the init value
     * to 1, so should not increase the semephore anymore!
    printf("shared memory resource created...\n");
    oper.sem_op = 1;
    if (semop(semid, &oper, 1) == -1) {
        fprintf(stderr, "IPC error: semop\n"); 
        exit(1);
    }
    */

    child = fork();
    if (child < 0) {
        fprintf(stderr, "Fail to fork new process\n");
        return 0;
    }
    else if (child == 0) {
        /* attach the shared memory, read something from the shared memory, 
         * then detach it, exit */ 
        printf("Child process %d running...\n", getpid());
        oper.sem_op = -1;
        /*
         * This shit error wast me one hour to debug.
         * if you cannot see the shit at the first glance,
         * you may still make such shit in the future.
        if (semop(semid, &oper, 1) == -1) 
            fprintf(stderr, "IPC error: semop"); exit(1);
       */
        if (semop(semid, &oper, 1) == -1) {
            fprintf(stderr, "IPC error: semop"); 
            exit(1);
        }
        printf("Child lock resource...\n");

        shared_memory =(char*)shmat(segment_id, 0, 0);
        shmctl(segment_id, IPC_STAT, &shmbuffer);
        /* what's the detail of the shmid_ds? */ 
        segment_size = shmbuffer.shm_segsz;
        printf("shared memory attached at address %p, size %d\n", shared_memory, segment_size);
        /* read something from the segment */
        printf("%s", shared_memory);

        printf("Child unlock resource...\n");
        oper.sem_op = 1;
        if (semop(semid, &oper, 1) == -1){
            perror("IPC error: semop"); 
            exit(1);
        }

        exit(0);
    }
    else {
        /* create a shared memory, write something to it, after child process 
         * terminated, deallocate it */
        printf("Parent process %d running, its child is %d ...\n", getpid(), (int)child);

        oper.sem_op = -1;
        if (semop(semid, &oper, 1) == -1) {
            perror("IPC error: semop"); 
            exit(1);
        }
        printf("Parent lock resource...sleep one second\n");

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
        sleep(1);

        printf("Parent unlock resource...\n");
        oper.sem_op = 1;
        if (semop(semid, &oper, 1) == -1) {
            perror("IPC error: semop"); 
            exit(1);
        }

        wait(NULL);
        printf("Get return value from child. END\n");

        /* after all process detach the segment, deallocate it */
        shmctl(segment_id, IPC_RMID, 0);
        /* deallocate semaphore set */
        semctl (semid, 0, IPC_RMID, 0);
    }

    return 0;
}

