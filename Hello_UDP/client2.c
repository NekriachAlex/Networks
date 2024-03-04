#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include "Packages.h"

#define MAX_LINE_LEN 1000000
#define PORT 8888
#define IP "127.0.0.1"
#define WAITING_TIME 5000



int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char line[MAX_LINE_LEN];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) 
    {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if(bind(sockfd, (const struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Bind failed");
        exit(1);
    }

    size_t packet_count = 0;
    struct Package package;
    size_t size = recvfrom(sockfd, &package, sizeof(struct Package), 0, &client_addr, sizeof(client_addr));
    if(size < 0 ) 
    {   
        perror("Receiving failed");
        exit(1);
    }
    printf("recived packet\n");
    if(package.count != 1)
    {
        printf("first analysis\n");
        printf("count = %d\n", package.count);
        printf("packet number = %d\n", package.number);
        printf("data = %c\n", package.packet);
        struct Package* packages = malloc(sizeof(struct Package) * package.count);
        memset(packages, 0, sizeof(struct Package) * package.count);
        printf("after memset\n");
       
        memcpy(packages + sizeof(struct Package) * package.number, &package, sizeof(struct Package));
        printf("after memcpy", package.count);
        size_t package_counter = 1;
        struct pollfd socket;
        memset(&socket, 0, sizeof(socket));
        socket.events = POLLIN;
        socket.fd = sockfd;
        
        int* recived = malloc(sizeof(size_t) *  package.count);
        memset(&recived, 0, sizeof(size_t) *  package.count);
        recived[package.count] = 1;
        int timeout = 0;
        while(1) 
        {    
            printf("im in cycle\n");
            if((timeout = poll(&socket, 1, WAITING_TIME)) == 0)
            {
                if(!check_packets(recived, package.count))
                {
                    exit(1);
                }
            } 
            size_t size = recvfrom(sockfd, &package, sizeof(struct Package), 0, &client_addr, sizeof(client_addr));
            if(size < 0 ) 
            {
                perror("Receiving failed");
                exit(1);
            }

            memcpy(packages + sizeof(struct Package) * (package.number - 1), &package, sizeof(struct Package));
            printf("i copy packet\n");
            package_counter++;
            recived[package.count] = 1;
            if(package_counter == package.count)
            {
                break;
            }

        }
        
        for(size_t package_number = 0; package_number < package.count; package_number++)
        {
            printf("try to make a line\n");
            strncpy(line + package_number * DATA_SIZE, packages[package_number].packet, DATA_SIZE);
        }
    }
    else
    {
         printf("try to make a line fo 1 packet\n");
         strncpy(line, package.packet, DATA_SIZE);
    }
    printf("%s \n", line);
    
    close(sockfd);
    return 0;
}