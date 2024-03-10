#include <string.h>
#include <stdlib.h>

#define DATA_SIZE 1

struct Package
{
    size_t number;
    size_t count;
    char packet[DATA_SIZE];
    unsigned char checksum;
};

struct Packages
{
    size_t counter;
    size_t size;
    struct Package* packages;
};

void add_to_packages(struct Packages* packages, struct Package package);
struct Package* create_packages(char* data);
int check_packets(size_t* recived, size_t count);
size_t package_count(char* data);
int check_consistency(int* packets, size_t packet_number);
unsigned char calculate_checksum(const void* data, size_t size);
void packages_sort(struct Packages* packages);

size_t* not_received(struct Packages* packages, size_t number);