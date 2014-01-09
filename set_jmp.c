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

#if 0
00000000004005a4 <f>:
void f()
{
  4005a4:       55                      push   %rbp
  4005a5:       48 89 e5                mov    %rsp,%rbp
    longjmp(b,1);
  4005a8:       be 01 00 00 00          mov    $0x1,%esi
  4005ad:       bf 00 0a 60 00          mov    $0x600a00,%edi
  4005b2:       e8 f1 fe ff ff          callq  4004a8 <longjmp@plt>

00000000004005b7 <main>:
}

int main()
{
  4005b7:       55                      push   %rbp
  4005b8:       48 89 e5                mov    %rsp,%rbp
    if (setjmp(b))
  4005bb:       bf 00 0a 60 00          mov    $0x600a00,%edi
  4005c0:       e8 c3 fe ff ff          callq  400488 <_setjmp@plt>
  4005c5:       85 c0                   test   %eax,%eax
  4005c7:       74 0c                   je     4005d5 <main+0x1e>	;why jump to 4005d5? who decide the offset?
        printf("World\n");
  4005c9:       bf f8 06 40 00          mov    $0x4006f8,%edi
  4005ce:       e8 a5 fe ff ff          callq  400478 <puts@plt>
  4005d3:       eb 1c                   jmp    4005f1 <main+0x3a>
    else
    {
        printf("Hello ");
  4005d5:       b8 fe 06 40 00          mov    $0x4006fe,%eax
  4005da:       48 89 c7                mov    %rax,%rdi
  4005dd:       b8 00 00 00 00          mov    $0x0,%eax
  4005e2:       e8 81 fe ff ff          callq  400468 <printf@plt>
        f();
  4005e7:       b8 00 00 00 00          mov    $0x0,%eax
  4005ec:       e8 b3 ff ff ff          callq  4005a4 <f>
    }
}
#endif
