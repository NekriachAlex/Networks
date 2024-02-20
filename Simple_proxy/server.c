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

void handle_client(int client_socket, int target_socket)
 {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, sizeof(buffer))) > 0) {
        write(target_socket, buffer, bytes_read);
    }

    close(client_socket);
    close(target_socket);
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
        if(poll((struct pollfd *)&serverFds[serverIsFull], currentSockets, -1) == -1)
        {
            perror("Cant using poll");
            unlink(PATH_TO_SOCKET);
            exit(1);
        }

        if((serverFds[0].revents & POLLIN) && (serverIsFull == 0)) 
        {
            int clientFd;
            if((clientFd = accept(serverSocket, NULL, NULL)) == -1)
            {
                perror("Cant accept");
                unlink(PATH_TO_SOCKET);
                exit(1);
            }
            clientCounter++;
            serverFds[clientCounter].fd = clientFd;
            currentSockets++;
            if(clientCounter == NUMBER_OF_CLIENTS)
            {
                serverIsFull = 1;
            }
        }

        for(nfds_t clientSocketNumber = 1; clientSocketNumber <= clientCounter; clientSocketNumber++)
        {
            if(serverFds[clientSocketNumber].revents & POLLIN)
            {
                size_t readSize = read(serverFds[clientSocketNumber].fd, buffer, BUFFER_SIZE);
                   
                if(readSize > 0)
                {
                    printUpper(buffer, readSize);
                }
                else
                {
                    close(serverFds[clientSocketNumber].fd);
                    serverFds[clientSocketNumber].fd = serverFds[clientCounter].fd;
                    serverFds[clientCounter].fd = -1;
                    clientCounter--;
                    currentSockets--;
                    serverIsFull = 0;
                }
            }

        }
         
    }

}