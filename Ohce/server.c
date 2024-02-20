#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3100
#define SIZE 512

char* echo(char* input_text) 
{
    size_t length = strlen(input_text);
    char* echo_text = (char*)malloc(length + 1);
    for (int i = 0; i < length; i++) 
    {
        echo_text[i] = input_text[length - i - 1];
    }
    echo_text[length + 1] = '\n';
    return echo_text;
}

int main() 
{
    int server_fd;
    int client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("Server socket creation failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 1) < 0) 
    {
        perror("Listen failed");
        exit(1);
    }


    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
    {
        perror("Accept failed");
        exit(1);
    }

    printf("Connection accepted\n");
    FILE* client = fdopen(client_fd, "r+");
    fgets(buffer, SIZE, client);
    char* output_text = echo(buffer);
    send(client_fd, output_text, strlen(output_text), 0);
    free(output_text);
   

    close(client_fd);
    close(server_fd);
    return 0;
}