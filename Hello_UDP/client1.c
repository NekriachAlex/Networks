#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>

#define MAXLINELEN 1000000
#define MAXPACKETSIZE 1024
#define PORT 8888
#define IP "127.0.0.1"

int main()
 {
    int sockfd;
    struct sockaddr_in client_addr2;
    char line[MAXLINELEN];
    char packet[MAXPACKETSIZE + sizeof(size_t)];

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

    size_t length = strlen(line);
    size_t packetcount = 0;

    for(size_t i = 0; i < length; i += MAXPACKETSIZE) 
    {
        packetcount++;
        memset(packet, 0, MAXPACKETSIZE);
        strncpy(packet, packetcount, sizeof(size_t));
        strncpy(packet[sizeof(size_t)], line + i, MAXPACKETSIZE);
        sendto(sockfd, packet, MAXLINELEN + sizeof(size_t), 0, &client_addr2, sizeof(client_addr2));
    }
    
    
    printf("Total packets sent: %ld\n", packetcount);

    close(sockfd);

    return 0;
 }