#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>
#include <poll.h>

#define PORT 3000
#define MAX_CLIENTS 5
#define BUFFER_SIZE 512

void handle_client()
{
    return;    
}

int main()
{
    char buffer[BUFFER_SIZE];

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        perror("Server socket cant be created");
        exit(1);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
    {
        perror("Cant bind server socket");
        exit(1);
    }
    
    if(listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Cant listen");
        exit(1);
    }

    struct pollfd server_fds[MAX_CLIENTS + 1];
    memset(server_fds, 0, sizeof(server_fds));
    server_fds[0].fd = server_socket;
    server_fds[0].events = POLLIN;
    for(nfds_t clientsSocketNumber = 1; clientsSocketNumber <= MAX_CLIENTS; clientsSocketNumber++)
    {
        server_fds[clientsSocketNumber].fd = -1;
        server_fds[clientsSocketNumber].events = POLLIN;
    }
    nfds_t clientCounter = 0;
    nfds_t currentSockets = 1;
    int serverIsFull = 0;
    while(1)
    {
        if(poll((struct pollfd *)&server_fds[serverIsFull], currentSockets, -1) == -1)
        {
            perror("Cant using poll");
            exit(1);
        }

        if((server_fds[0].revents & POLLIN) && (serverIsFull == 0)) 
        {
            int clientFd;
            if((clientFd = accept(server_socket, NULL, NULL)) == -1)
            {
                perror("Cant accept");
                exit(1);
            }
            clientCounter++;
            server_fds[clientCounter].fd = clientFd;
            currentSockets++;
            if(clientCounter == MAX_CLIENTS)
            {
                serverIsFull = 1;
            }
        }

        for(nfds_t clientSocketNumber = 1; clientSocketNumber <= clientCounter; clientSocketNumber++)
        {
            if(server_fds[clientSocketNumber].revents & POLLIN)
            {
                   
                    handle_client();
                    close(server_fds[clientSocketNumber].fd);
                    server_fds[clientSocketNumber].fd = server_fds[clientCounter].fd;
                    server_fds[clientCounter].fd = -1;
                    clientCounter--;
                    currentSockets--;
                    serverIsFull = 0;
                
            }

        }
         
    }

}