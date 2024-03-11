#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h> 
#include "Packages.h"

#define MAXLINELEN 1000000
#define PORT_CL2 8888
#define PORT_MY 9999
#define IP "127.0.0.1"

int main()
 {
    int sockfd;
    struct sockaddr_in client_addr2;
    char line[MAXLINELEN];

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in my_server;
    int my_server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&my_server, 0, sizeof(my_server));
    my_server.sin_family = AF_INET;
    my_server.sin_port = htons(PORT_MY);
    my_server.sin_addr.s_addr = inet_addr(IP);

    if(bind(my_server_socket, (struct sockaddr_in*) &my_server, sizeof(my_server)) < 0) 
    {
        perror("Bind failed");
        exit(1);
    }

    printf("Enter a string to send: ");
    fgets(line, MAXLINELEN, stdin);

    memset(&client_addr2, 0, sizeof(client_addr2));
    client_addr2.sin_family = AF_INET;
    client_addr2.sin_port = htons(PORT_CL2);
    inet_pton(AF_INET, IP, &client_addr2.sin_addr.s_addr);

    struct Package* packages = create_packages(line);
    size_t package_amount = package_count(line);
    for(size_t package_counter = 0; package_counter < package_amount - 2; package_counter++)
    {
        sendto(sockfd, packages + package_counter, sizeof(struct Package), 0, &client_addr2, sizeof(client_addr2));
    }

    printf("Client1: Packets are sent \n");
    size_t* not_rec = malloc(sizeof(size_t) * package_amount);
    memset(not_rec, 0, package_amount);
    size_t size = recvfrom(my_server_socket, not_rec, sizeof(size_t) * package_amount, 0, NULL, sizeof(client_addr2));
    printf("packet is %ld\n", not_rec[1]);

    for(size_t package_counter = 0; package_counter < not_rec[0]; package_counter++)
    {
        printf("starting send unreceived packaets\n");
        printf("number of unreceive packet is %ld\n", not_rec[package_counter + 1]);
        printf("number of unreceive packet is %ld\n",packages[not_rec[package_counter + 1] - 1].number);
        sendto(sockfd, &packages[not_rec[package_counter + 1] - 1], sizeof(struct Package), 0, &client_addr2, sizeof(client_addr2));
    }
    printf("Client1: Packets are sent \n");
    
    close(sockfd);
    free(packages);
    return 0;
 }