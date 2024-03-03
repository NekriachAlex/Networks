#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h> 
#include "Packages.h"

#define MAXLINELEN 1000000
#define MAXPACKETSIZE 1024
#define PORT 8888
#define IP "127.0.0.1"

int main()
 {
    int sockfd;
    struct sockaddr_in client_addr2;
    char line[MAXLINELEN];

    if(sockfd = socket(AF_INET, SOCK_DGRAM, 0)) 
    {
        perror("Socket creation failed");
        exit(1);
    }

    printf("Enter a string to send: ");
    fgets(line, MAXLINELEN, stdin);

    memset(&client_addr2, 0, sizeof(client_addr2));
    client_addr2.sin_family = AF_INET;
    client_addr2.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &client_addr2.sin_addr.s_addr);

    struct Package* packages = create_packages(line);
    size_t package_amount = package_count(line);
    for(size_t package_counter = 0; package_counter < package_amount; package_counter++)
    {
        sendto(sockfd, packages + package_counter * sizeof(struct Package), sizeof(struct Package), 0, &client_addr2, sizeof(client_addr2));
    }
    
    printf("Packets are sent");
    
    close(sockfd);
    free(packages);
    return 0;
 }