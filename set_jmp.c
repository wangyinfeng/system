/*====================================================================
 * Description: The usage of the setjmp/longjmp
 * DATE: 2014/01/09
 * Modify:
 * Conclusion: Not understand the following code yet
===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <setjmp.h>

/* Macro definition */
#define false           0
#define true            1

jmp_buf b;
void f()
{
    longjmp(b,1);
}

int
main(int argc, char **argv)
{
    if (setjmp(b))
        printf("World\n");
    else
    {
        printf("Hello ");
        f();
    }
    return 0;
}

