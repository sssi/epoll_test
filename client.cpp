#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXLINE 500

int
main(int argc, char **argv)
{
    int                    sockfd;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in    servaddr;

    if (argc != 2)
        printf("usage: a.out <IPaddress>");

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(5000);   
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf("inet_pton error for %s", argv[1]);

    if (connect(sockfd, ( struct sockaddr* ) &servaddr, sizeof(servaddr)) < 0)
        printf("connect error");

    char input[100];
    while (fgets(input, 100, stdin))
    {
        write(sockfd, input, strlen(input));

        int n = 0;
        int count = 0;
        while (1)
        {
            n = read(sockfd, recvline + count, MAXLINE);
            if (n == MAXLINE)
            {
                count += n;
                continue;
            }
            else 
                break;
        }
        printf("%s\n", recvline);
    }
    exit(0);
}
