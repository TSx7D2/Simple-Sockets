#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define PORT "1350"
#define BACKLOG 10

extern int errno;

int main()
{
    int g_status, sockfd, b_status, l_status, n_sockfd, r_status, s_status;
    struct addrinfo hints, *res;
    struct sockaddr_storage incoming_addr;
    socklen_t addr_size;

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

    b_status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (b_status == -1)
    {
        fprintf(stderr, "Error bind(): %s\n", strerror(errno));
        return 1;
    }

    l_status = listen(sockfd, BACKLOG);
    if (l_status == -1)
    {
        fprintf(stderr, "Error listen(): %s\n", strerror(errno));
        return 1;
    }

    addr_size = sizeof incoming_addr;
    char r_buffer[255];
    char s_ack[] = "ACK";

    for (;;)
    {   
        printf("Waiting for connection...\n");

        n_sockfd = accept(sockfd, (struct sockaddr *)&incoming_addr, &addr_size);
        if (n_sockfd == -1)
        {
            fprintf(stderr, "Error accept(): %s\n", strerror(errno));
            return 1;
        }

        printf("Connection established.\n\n");

        for (;;)
        {
            r_status = recv(n_sockfd, r_buffer, 255, 0);
            if (r_status == -1)
            {
                fprintf(stderr, "Error recv(): %s\n", strerror(errno));
                return 1;
            }

            if (r_status == 0)
            {
                printf("Client closed connection.\n\n");
                break;
            }

            r_buffer[r_status] = '\0';
            printf("Received: %s", r_buffer);

            s_status = send(n_sockfd, s_ack, strlen(s_ack), 0);
            if (s_status)
            {
                fprintf(stderr, "Error send(): %s\n", strerror(errno));
            }

        }
    }

    freeaddrinfo(res);
    return 0;
}