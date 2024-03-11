#include "Packages.h"
//check sum
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
        current_package.checksum = calculate_checksum(&current_package.packet, DATA_SIZE);
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

unsigned char calculate_checksum(const void* data, size_t size)
{
    const unsigned char* ptr = (unsigned char*)data;
    unsigned char checksum = 0;
    for (size_t i = 0; i < size; ++i) 
    {
        checksum ^= ptr[i];
    }
    return checksum;
}

void add_to_packages(struct Packages* packages, struct Package package)
{
    printf("add_to_packages: starts\n");
    printf("number is: %ld\n", package.number);
   // printf("sum is %c; calculated sum is %c\n", package.checksum, calculate_checksum(&package, sizeof(struct Package) - sizeof(unsigned char)));
    
    if(!(package.checksum == calculate_checksum(&package.packet, DATA_SIZE)))
    {   
        printf("add_to_packages: wrong packet\n");
        return;
    }
    
    if(packages->counter == packages->size)
    {
        printf("add_to_packages: realloc\n");
        packages->packages = realloc(packages->packages, packages->size*2*sizeof(struct Package));
        packages->size = (packages->size)*2;
        memcpy(&(packages->packages[packages->counter]), &package, sizeof(struct Package));
    }
    else
    {
        printf("add_to_packages: simple adding\n");
        memcpy(&(packages->packages[packages->counter]), &package, sizeof(struct Package));    
    }
    (packages->counter)++;
}

int compare (const void * a, const void * b)
{
  // do compare a and b
  // return -1 (or any negative number) if a < b
  // return 0 if a == b
  // return 1 (or any positive number) if a > b
   printf("compare\n");
  if(((struct Package*)a)->number > ((struct Package*)b)->number)
  {
    return 1;
  }
  else if(((struct Package*)a)->number < ((struct Package*)b)->number)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

void packages_sort(struct Packages* packages)
{
    printf("qsort\n");
    printf("number of 1 pack: %ld\n", packages->packages[0].number); 
    qsort(packages->packages, packages->counter, sizeof(struct Package), &compare);
    printf("number of 1 pack: %ld\n", packages->packages[0].number); 
    printf("stop sorting\n");
}

size_t* not_received(struct Packages* packages, size_t number)
{
    printf("not_received\n");
    size_t* not_received_packages = malloc(sizeof(size_t)*(number - packages->counter));
    printf("number %ld counter %ld \n", number, packages->counter);
    memset(not_received_packages, 0 , number - packages->counter);
    printf("sorting\n");
    packages_sort(packages);
    size_t tmp_number = 1;
    size_t counter = 0;
    printf("before for\n");
    printf("counter after sort %ld\n",packages->counter);
    printf("number of 1 pack: %ld\n", packages->packages[0].number); 
    printf("number in if %ld, tmp is: %ld\n", packages->packages[0].count, tmp_number);
    not_received_packages[0] = number - packages->counter;
    for(size_t iterator = 0; iterator < packages->counter; iterator++)
    {
        printf("in for\n");
        while(packages->packages[iterator].number != tmp_number)
        {
          not_received_packages[counter + 1] = tmp_number;
          tmp_number++;
          counter++;
        }
        tmp_number++;
    }
    tmp_number--;
    printf("number in if %ld, tmp is: %ld\n", packages->packages[0].count, tmp_number);
    //if we lost greater number than last
    if(tmp_number != packages->packages[0].count)
    {
        
        while(tmp_number != packages->packages[0].count)
        {
            tmp_number++;
            printf("in while after if\n");
            not_received_packages[counter + 1] = tmp_number;
            printf("tmp number in last while %ld\n",tmp_number);
            counter++;
        }
    }
    printf("after for\n");
    return not_received_packages;
}