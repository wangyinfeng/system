/*====================================================================
 * Description: know pthread
 * DATE: 2013/12/17
 * Modify:
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
void *func_1(void* args)  
{  
    while(1){  
        sleep(1);  
        printf("this is func_1!\n");  
    }  
}
  
void *func_2(void* args)  
{
    sleep(2);
    printf("this is func_2!\n");  
    pthread_exit(NULL);
}

int
main(int argc, char **argv)
{
    /* A thread may either be joinable or detached */
    pthread_t pid1, pid2;  

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

//    while(1){
//        sleep(3);
//    }
    pthread_join(pid2, NULL); /* wait for the thread 2 exit */
    printf("end of main\n");

    return 0;
}

