/*====================================================================
 * Description: know pthread
 * DATE: 2013/12/17
 * Modify: 2013/12/18 - getpid(), getppid(), the process not share the data
 *                    - mutex
 * Conclusion:
===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Macro definition */
#define false           0
#define true            1

/* link with -lpthread */
  
/* Thread terminate:
* Calls pthread_exit(), specifying an exit status value that is available to another thread in the same process that calls pthread_join().
* It returns from start_routine(). This is equivalent to calling pthread_exit() with the value supplied in the return statement.
* It is canceled (see pthread_cancel()).
* Any of the threads in the process calls exit(), or the main thread performs a return from main(). This causes the termination of all threads in the process.
*/

static int count;
//pthread_mutex_t mutex;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *func_1(void* args)  
{
    while(1){ 
        pthread_mutex_lock(&mutex);
        sleep(1);
        count++;
        printf("this is func_1, count %d!\n",count);
        pthread_mutex_unlock(&mutex);
    }
}
  
void *func_2(void* args)  
{
    while(1){
        pthread_mutex_lock(&mutex);
        sleep(1);
        count++;
        printf("this is func_2!, count %d\n", count);  
        pthread_mutex_unlock(&mutex);
//        pthread_exit(NULL);
    }
}
  
int
main(int argc, char **argv)
{
    /* A thread may either be joinable or detached */
    pthread_t pid1, pid2;  
    pid_t pid;
    //pthread_mutex_init(&mutex, NULL);

    /* int pthread_create(pthread_t *thread,    -pthread_id
     *                    const pthread_attr_t *attr, -attribute
     *                    void *(*start_routine) (void *), -routeine 
     *                    void *arg);
     *                    */
    if(pthread_create(&pid1, NULL, func_1, NULL))  
    {  
        return -1;  
    }  
  
    if(pthread_create(&pid2, NULL, func_2, NULL))  
    {
        return -1;
    }

    while(1){
        sleep(3);
    }

    pthread_join(pid2, NULL);

    if ((pid=fork()) == 0) {
        sleep(1);
        count++;
        printf("count in child process %d(PPID %d) is %d\n", getpid(), getppid(), count);
    }
    else {
        count+=100;
        printf("count+100 in parent process %d(PPID %d) is %d\n", getpid(), getppid(), count);
    /* if the parent process exit earlier than the child process, the child process
    *  will be orphan and it's paraent's process id will be the init(1). 
    *  that's why need wait() or waitpid() here. */
        wait(NULL);
    }

    printf("Process %d end of main\n", getpid());
    return 0;
}

