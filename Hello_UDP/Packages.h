#include <string.h>
#include <stdlib.h>
#define DATA_SIZE 1024

struct Package
{
    size_t number;
    size_t count;
    char packet[DATA_SIZE];
};

struct Package* create_packages(char* data);

size_t package_number(char* data);