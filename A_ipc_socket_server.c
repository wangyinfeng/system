/*====================================================================
 * Description: First time to know what's the socket works, copy from
 * others
 * DATE: 2014/06/06
 * Modify:
 * Conclusion:
===================================================================*/
/* includes */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>

/* Macros */
#define false           0
#define true            1

/**
 * Written by Austin Walters
 * For an example on austingwalters.com,
 * on sockets
 */

int 
main(int argc, char** argv)
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct addrinfo directives, *result;
    memset(&directives, 0, sizeof(struct addrinfo));
    directives.ai_family = AF_INET;
    directives.ai_socktype = SOCK_STREAM;
    directives.ai_flags = AI_PASSIVE;

    /* Translates IP, port, protocal into struct */
//    if(0 !=  getaddrinfo("192.168.56.101", "8080", &directives, &result))
    if(0 !=  getaddrinfo(NULL, "8080", &directives, &result))
        exit(1);

    /* Binds socket to port, so we know where new connections form */
    if(bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0)
        exit(1);
    /* Places socket to "listen" or "wait for stuff" state */
    if(listen(sock_fd, 10) != 0)
        exit(1);

    printf("Waiting for connection on http://localhost:8080 ...\n");

    while(1){

        /* Accepts Connection */
        int client_fd = accept(sock_fd, NULL, NULL); 
        char buffer[1000];
        int len = read(client_fd, buffer, 999);
        buffer[len] = '\0';

        char * header = "<b>You Connected to the Server!</b></br></br>";
        char * body = "Brought to you by: <a href=\"austingwalters.com\">Austin Walters</a>!";

        write(client_fd, header, strlen(header)); 
        write(client_fd, body, strlen(body));

        printf("\nRead %d chars\n", len);
        printf("=== Client Sent ===\n");
        printf("%s\n", buffer);

        close(client_fd);
        sleep(1);
    }

    return 0;
}

