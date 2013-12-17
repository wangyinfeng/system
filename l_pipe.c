/*====================================================================
 * Description: Know the usage of pipe
 * DATE: 2013/12/17
 * Modify:
 * Conclusion:
===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Macro definition */
#define false           0
#define true            1

int pipe_fd[2];

int
main(int argc, char **argv)
{
    pid_t pid;
    char buffer[32];
  
    memset(buffer, 0, 32);
    /* create a pipe, the pipe_fd[0] refers the read end of the pipe
     * the pipe_fd[1] refers the write end of the pipe.
     * Data written to the write end of the pipe is buffered by the kernel
     * until read from the read end of the pipe. 
     * ---- what's the size of the buffer supported by kernel?
     *      from ulimit -a, the pipe size is "(512 bytes, -p) 8", 
     *      that's 4k bytes, the reason why PIPE_BUF is 4KB is that 
     *      PIPE_BUF is the largest size for which writes are guaranteed 
     *      to be atomic(thread-safe).
     *      When test the real size of the pipe buffer, the size is 64K bytes,
     *      and it's programble since 2.6.35?
     *       */
    if(pipe(pipe_fd) < 0)
    {
        printf("Failed to create pipe!\n");
        return 0;
    }
    printf("Max pipe buffer size PIPE_BUF: %d\n", PIPE_BUF);

    if(0 == (pid = fork()))
    {
        /* close the unused write end */
        close(pipe_fd[1]);
        sleep(5);  
        if(read(pipe_fd[0], buffer, 32) > 0)  
        {
            printf("Receive data from server, %s!\n", buffer);
        }
        close(pipe_fd[0]);
    }
    else
    {
        /* close the unused read end */
        close(pipe_fd[0]);
        if(-1 != write(pipe_fd[1], "hello", strlen("hello")))
        {
            printf("Send data to client, hello!\n");
        }
        close(pipe_fd[1]);  
        waitpid(pid, NULL, 0);  
    }  
  
    return 0;
}

