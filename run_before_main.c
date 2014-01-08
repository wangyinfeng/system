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
/* constructor/destructor has prority, for the constructor run the the lower number first.
 * for the destructor run the higher number first */
/* constructor priorities from 0 to 100 are reserved for the implementation */
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

/* another way is specify the entry with gcc '-e' option,
 * to replace the default entry _start */
/* Got segmentation fault after new_start */
void new_start(void)
{
    printf("%s\n", __func__);
}

int
main(int argc, char **argv)
{

    printf("%s\n", __func__);
    return 0;
}

