#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LINE_LEN 1000000
#define MAX_PACKET_SIZE 1024
#define PORT 8888
#define IP "127.0.0.1"

int check_consistency(int* packets, size_t packet_number)
{
    for(size_t i = 0; i < packet_number; i++)
    {
        if(packets[i] == 0)
        {
            return 0;
        }
    }
    return 1;
}
int main() {
    int sockfd;
    char packet[MAX_PACKET_SIZE];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char line[MAX_LINE_LEN];
    char* packets[MAX_LINE_LEN / MAX_PACKET_SIZE];
    int received_packets[MAX_LINE_LEN / MAX_PACKET_SIZE];

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
    while(1) 
    {    
        memset(packet, 0, MAX_PACKET_SIZE);
        
        size_t len = recvfrom(sockfd, packet, MAX_PACKET_SIZE, 0, &client_addr, sizeof(client_addr));

        if(len < 0) 
        {
            perror("Receiving failed");
            exit(1);
        }

        size_t packet_num = 0;
        strncpy(&packet_num, packet, sizeof(size_t));
        
        if(packet_num == 0 && check_consistency(&received_packets, packet_count))
        {
            break;
        }


        if(packet_num > 0)
        {
            received_packets[packet_num] = 1;
            packets[packet_num] = malloc(MAX_PACKET_SIZE);
            strncpy(packets[packet_num], packet + sizeof(size_t), MAX_PACKET_SIZE);
            packet_count++;
        }
        
    }
    
    for(int i = 0; i < MAX_LINE_LEN; i += MAX_PACKET_SIZE)
    {
        printf("%s", packets[i/MAX_PACKET_SIZE]);
    }
    
    close(sockfd);
    
    return 0;
}