/*====================================================================
 * Description: How to run code before main()
 * DATE: 2014/01/08
 * Modify:
 * Conclusion: 
 * ===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Macro definition */
#define false           0
#define true            1
/* GCC extension */
/* #1 constructor/destructor has prority, for the constructor run the the lower number first.
 * for the destructor run the higher number first */
/* constructor priorities from 0 to 100 are reserved for the implementation */
/* the constructor function locate in .ctors and the destructor function locate in .dtors */
void __attribute__((constructor(101))) init_func_1(void)
{
    printf("%s\n", __func__);
}
void __attribute__((constructor(102))) init_func_2(void)
{
    printf("%s\n", __func__);
}
void __attribute__((destructor(101))) finit_func_1(void)
{
    printf("%s\n", __func__);
}
void __attribute__((destructor(102))) finit_func_2(void)
{
    printf("%s\n", __func__);
}

/* #1.1 one more way to add in constructor section */
typedef void(*ctor_t)(void);
void ctor_init(void)
{
    printf("%s\n", __func__);
}
ctor_t __attribute__((section(".ctors"))) ctor_init_p = &ctor_init;

/* #2 Put function in .init and .finit section */
int hello_init()
{
    __asm__ (".section .init \n call hello_init \n .section .text\n");
    printf("init hello\n");
    return 0;
}

int hello_finit()
{
    __asm__ (".section .finit \n call hello_finit \n .section .text\n");
    printf("finit hello\n"); /* not see finit hello... */
    return 0;
}


/* #3 another way is specify the entry with gcc '-e' option,
 * to replace the default entry _start */
/* Got segmentation fault after new_start */
void new_start(void)
{
    printf("%s\n", __func__);
}

/* #4 use atexit() */ 
void func_exit()
{
    printf("%s\n", __func__);
}

int
main(int argc, char **argv)
{

    atexit(func_exit);
    printf("%s\n", __func__);
    return 0;
}

