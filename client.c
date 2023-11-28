#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define PORT "1350"
#define BACKLOG 10

extern int errno;

int main()
{
    int g_status, sockfd, c_status, r_status, s_status;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    g_status = getaddrinfo(NULL, PORT, &hints, &res);

    if (g_status != 0)
    {
        fprintf(stderr, "Error getaddrinfo(): %s\n", gai_strerror(g_status));
        return 1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
    {
        fprintf(stderr, "Error socket(): %s\n", strerror(errno));
        return 1;
    }

    printf("Attempting to connect...\n");

    c_status = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (c_status == -1)
    {
        fprintf(stderr, "Error connect(): %s\n", strerror(errno));
        return 1;
    }

    printf("Connected.\n\n");

    addr_size = sizeof incoming_addr;
    char r_buffer[255];
    char s_ack[] = "ACK";

    for (;;)
    {   
        printf("user@socket: ");
        fgets(s_buffer, 255, stdin);
        s_status = send(sockfd, s_buffer, strlen(s_buffer), 0);
        if (s_status == -1)
        {
            fprintf(stderr, "Error send(): %s\n", strerror(errno));
            return 1;
        }

        r_status - recv(sockfd, r_buffer, 255, 0);
        if (r_status == -1)
        {
            fprintf(stderr, "Error recv(): %s\n", strerror(errno));
            return 1;
        }
        if (r_status == 0)
        {
            printf("Server closed connection.\n\n");
            return 1;
        }
    }

    freeaddrinfo(res);
    return 0;
}