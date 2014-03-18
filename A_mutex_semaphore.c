/*====================================================================
 * Description: TBD. Implement the code at page 108.
 * - linked list, manage the job queue
 * - mutex, protect the job queue operation
 * - semaphore, count the job number and continue to process job until exit
 * DATE: 2014/03/18
 * Modify:
 * Conclusion:
===================================================================*/
/* include */
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

/* Macro definition */
#define false           0
#define true            1

pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t	job_queue_count;

void initialize_job_queue()
{
    ....
    sem_init(&job_queue_count, 0, 0);
}

/* get job from the queue */
void* job_handle(void* arg)
{
}

void equeue_job()
{
}

int
main(int argc, char **argv)
{

    return 0;
}

