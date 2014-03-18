/*====================================================================
 * Description: test thread specific data area.
 * Just as the example show, create a log variable, each thread use
 * their own.
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

/* Macro definition */
#define false           0
#define true            1

static pthread_key_t    thread_log_key;

void *write_log(const char* msg) 
{
    /* get the thread specific data by the key */
    FILE *thread_log = (FILE*)pthread_getspecific(thread_log_key);
//    assert(thread_log != NULL);
    fprintf(thread_log, "%s", msg); 
    return NULL;
}

void close_log(void *file)
{
    fclose((FILE*)file);
}

void *thread_func(void *argv)
{
    char thread_log_name[32];
    FILE *thread_log;

    sprintf(thread_log_name, "thread_log_%d", (int)pthread_self());
    thread_log = fopen(thread_log_name, "w"); /* NOT "W" */

    /* set the thread specific data(thread_log) under the key(thread_log_key) */
    pthread_setspecific(thread_log_key, thread_log);
    write_log("Just try\n");
    
    return NULL;
}

void release(void *buff)
{
    if (buff != NULL)
        free(buff);
}

void *cleanup_handle(void *argv)
{
    char *buff = malloc(100);
    if (buff != NULL)
        pthread_cleanup_push(release,buff);
    /* do something here */
    /* if the thread be terminated or exit with pthread_exit without
     * relase the buffer, the cleanup_handle(release) will release the
     * buffer. */

    /* pass non-zero parameter to the unregister function, will actually
     * run the cleanup function. the same as use free() explicitly. */
    pthread_cleanup_pop(1);
}

int
main(int argc, char *argv[])
{
    int i;
    pthread_t thread[2];
    pthread_t cleanup_thread;

    /* create the key, and also specify the destructor */
    pthread_key_create(&thread_log_key, close_log);

    for (i=0; i<2; i++) {
        pthread_create(&(thread[i]), NULL, thread_func, NULL);
    }
    for (i=0; i<2; i++) {
        if (!pthread_equal(pthread_self(), thread[i]))
            pthread_join(thread[i], NULL);
    }

    pthread_create(&cleanup_thread, NULL, cleanup_handle, NULL);

    fprintf(stdout, "End of main\n");
    return 0;
}

