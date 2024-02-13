#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, port_number;
    char buffer[14];

  
    int readed_size;

    if (argc < 2) {
        fprintf(stderr, "no port number");
        return 1;
    }

    port_number = atoi(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket");
        return 1;
    }

    struct sockaddr_in server_address, client_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "ERROR on binding");
        return 1;
    }

    listen(sockfd, 1);
 
    socklen_t client_length;
    while (1) {
        client_length = sizeof(client_address);
        newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &client_length);
        if (newsockfd < 0) {
            fprintf(stderr, "ERROR on accept");
            return 1;
        }

        memset(buffer, 0, sizeof(buffer));
        readed_size = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (readed_size < 0) {
            fprintf(stderr, "ERROR reading from socket");
            return 1;
        }

        printf("Received message from client: %s\n", buffer);
        close(newsockfd);
    }
    close(sockfd);

    return 0;
}