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

int main()
{
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
    //recieve packets
    struct pollfd recieving;
    memset(&recieving, 0, sizeof(struct pollfd));
    recieving.fd = sockfd;
    recieving.events = POLLIN;
    struct Package package;
    memset(&package, 0, sizeof(struct Package));

    struct Package* packages_ptr = malloc(sizeof(struct Package)*5);
    memset(packages_ptr, 0, sizeof(struct Package)*5);
    struct Packages packages;
    packages.size = 5;
    packages.packages = packages_ptr;
    packages.counter = 0;

    while(1)
    {
        int timeout = poll(&recieving, 1, 5000);
        if(timeout == 0)
        {
            printf("timeout \n");
            if(packages.counter != packages.packages[0].count)
            {
                printf("p.counter: %ld; count from first: %ld\n",packages.counter, packages.packages[0].number);
                size_t* not_rec = not_received(&packages, packages.packages[0].count);
                //sent this to client;
                printf("sent this to client;");
                exit(1);
            }
            if(packages.counter == packages.packages[0].count)
            {
                break;
            }
        }
        printf("before recvfrom \n");
        size_t size = recvfrom(sockfd, &package, sizeof(struct Package), 0, &client_addr, sizeof(client_addr));
        if(size < 0 )
        {
            perror("recvfrom error");
        }
        
        add_to_packages(&packages, package);
    }
    for(size_t package_number = 0; package_number < packages.packages[0].count; package_number++)
        {
            printf("im making line \n");
            strncpy(line + package_number * DATA_SIZE, packages.packages[package_number].packet, DATA_SIZE);
        }
    printf("line: %s \n", line);
    close(sockfd);
    return 0;
}