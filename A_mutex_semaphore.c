/*====================================================================
 * Description: TBD. Implement the code at page 108.
 * - linked list, manage the job queue
 *   WTF, need to understand this basic data structure completely!!!
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
sem_t   job_queue_count;

typedef struct node{
    struct node *next;
    char    job;
}N;

typedef struct queue{
    N*  header;
    N*  tail;
    N*  current;
    N*  num;
}Q;


void initialize_job_queue(Q *job_queue)
{
    job_queue->header = NULL;
    job_queue->tail = NULL;
    job_queue->current = NULL;
    job_queue->num = 0;
    sem_init(&job_queue_count, 0, 0);
}

/* get job from the queue */
void* job_handle(void* arg)
{
    Q *job_list = (Q*)arg;
    while(1) {
        N *job;
        sem_wait(&job_queue_count);
        pthread_mutex_lock(&job_queue_mutex);
        job = job_list->header;
        job_list->header = job_list->header->next;
        pthread_mutex_unlock(&job_queue_mutex);

        if(job == NULL)
            printf("DONE\n");
        else {
            printf("Process job %c\n", job->job);
            free(job);
        }
    }
    return NULL;
}

void equeue_job(Q *job_queue, char job)
{
    N *new_node = (N*)malloc(sizeof(N));
    if (new_node == NULL){
        fprintf(stderr, "malloc new job fail\n");
        return;
    }
    new_node->job = job;
    pthread_mutex_lock(&job_queue_mutex);
    if(job_queue->header == NULL) { /* 1st */
        new_node->next = NULL;
        job_queue->tail = new_node;
        job_queue->num++;
    }
    else {
        new_node->next = job_queue->header;
        job_queue->num++;
    }
    job_queue->header = new_node;
    sem_post(&job_queue_count);
    pthread_mutex_unlock(&job_queue_mutex);
}

void print_job_queue(Q *q){
    printf("Job Queue:\n");
    if (q->header == NULL) {
        printf("No job exit\n");
    }
    else {
        q->current = q->header;
        while (q->current) {
            printf("->%c", q->current->job);
            q->current = q->current->next;
        }
    }
    printf("\n");
}

int
main(int argc, char **argv)
{
    int     i;
    Q   job_queue;
    pthread_t job_thread[2];
    char jobs[5] = {'A', 'B', 'C', 'D', 'E'};
    initialize_job_queue(&job_queue);
    for (i=0; i<5; i++) {
        equeue_job(&job_queue, jobs[i]);
    }
    printf("equeue job number %d\n", job_queue_count);
    print_job_queue(&job_queue);
    
    for(i=0; i<1; i++) {
        pthread_create(&job_thread[i], NULL, job_handle, &job_queue);
        pthread_join(job_thread[i], NULL);
    }

    print_job_queue(&job_queue);
    return 0;
}

