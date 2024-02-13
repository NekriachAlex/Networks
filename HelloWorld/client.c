#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    int sockfd, port_number;
    char buffer[14];
    char *message = "Hello, World!";

    if (argc < 3) {
        fprintf(stderr,"no port number\n");
        return 1;
    }

    port_number = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "ERROR opening socket");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_number);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "ERROR invalid address");
    }
        
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    { 
        fprintf(stderr, "ERROR connecting");
    }

    int written_size;
    written_size = write(sockfd, message, strlen(message));
    if (written_size < 0)
    {    
        fprintf(stderr, "ERROR writing to socket");
    }

    close(sockfd);
    return 0;
}