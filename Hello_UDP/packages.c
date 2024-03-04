#include "Packages.h"

struct Package* create_packages(char* data)
{
    size_t number_of_packages = package_count(data);
    
    struct Package* packages = (struct Package*)malloc(number_of_packages * sizeof(struct Package));
    memset(packages, 0, number_of_packages * sizeof(struct Package));
    size_t packetcount = 1;

    struct Package current_package;
    current_package.count = number_of_packages;

    for(size_t line_iterator = 0; line_iterator < strlen(data); line_iterator += DATA_SIZE) 
    {   
        memcpy(current_package.packet, &data[line_iterator], DATA_SIZE);
        current_package.number = packetcount;
        memcpy(&packages[packetcount - 1] , &(current_package), sizeof(struct Package));
        packetcount++;
    }
    return  packages;
}

int check_packets(size_t *recived, size_t count)
{
    
    for(size_t package_counter; package_counter < count; package_counter++)
    {
        if(recived[package_counter] == 0)
        {
            return 0;
        }
    }
    return 1;
}

size_t package_count(char *data)
{
    size_t size_of_data = strlen(data);
    size_t number_of_packages = size_of_data / DATA_SIZE;
    if(size_of_data % DATA_SIZE != 0)
    {
        number_of_packages++;  
    }

    return number_of_packages;
}

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
