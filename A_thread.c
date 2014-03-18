/*====================================================================
 * Description: Review the pthread. main() wait the thread it's created,
 * pass parmenets to the created thread, get return value.
 * DATE: 2014/03/17
 * Modify:
 * TODO: How to return complex value from the thread, such as a structure
 * - pthread_join(pthread_t thread, void **retval) copies the exit status 
 * of the target thread (i.e., the value that the target thread supplied to 
 * pthread_exit(3)) into the location pointed to by *retval.
 * So the return value for the pthread_join is just the exit code, not
 * intend to return something complex?
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

pthread_attr_t attr;

typedef struct parment{
    int count;
    char c;
}Parment;

void *put_x(void *argv)
{
    int i;
    char r = 'B';
    Parment *p = (Parment*) argv;
    for (i=0; i<p->count; i++) {
        sleep(1);
//        fputc(p->c, stdout);
        fputc(p->c, stderr);
    }
    return (void*)r;
}

void *count_x(void *argv)
{
    int i;
    Parment *p = (Parment*) argv;
    i = (p->c)*(p->c);
    return (void*)i;
}

void *detach_thread(void *argv)
{
    int i;
    Parment *p = (Parment*)argv;
    printf("I'm detached thread\n");
    for (i=0; i<p->count; i++){
        sleep(1);
        fputc(p->c, stderr);
    }
    printf("I'm living longer than the main, am I still alive?\n");
}

int
main(int argc, char *argv[])
{
    pthread_t thread_put;
    pthread_t thread_count;
    pthread_t thread_detach;
    Parment p_to_put;
    Parment p_to_count;
    Parment p_to_detach;
    char put_return[100];
    int count_return;

    p_to_put.count = 3;
    p_to_put.c = 'A';

    p_to_detach.count = 5;
    p_to_detach.c = 'X';

    p_to_count.c = 5;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread_detach, &attr, &detach_thread, &p_to_detach);
    /* no need to keep the attribute after create the thread */
    pthread_attr_destroy(&attr);

    /* by default is joinable thread */
    pthread_create(&thread_put, NULL, &put_x, &p_to_put);
    pthread_create(&thread_count, NULL, &count_x, &p_to_count);

//    pthread_join(thread_put, NULL);
    if (!pthread_equal(pthread_self(), thread_put))
        pthread_join(thread_put,(void*)&put_return[0]);
    if (!pthread_equal(pthread_self(), thread_count))
        pthread_join(thread_count, (void*)&count_return);


    fprintf(stdout, "\nReturn value of put thread is %c\n", &put_return[0]);
    fprintf(stdout, "\nReturn value of count thread is %d\n", count_return);
    fprintf(stdout, "My pid %d\nEnd of main\n", getpid());
    return 0;
}

