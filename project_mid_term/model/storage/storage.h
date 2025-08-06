#ifndef _STORAGE_H
#define _STORAGE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/statvfs.h>
#include <time.h>


typedef struct {
    uint64_t total_storage;
    uint64_t used_storage;
    uint64_t free_storage;

    double read_speed;
    double write_speed;

    uint64_t sectors_read;
    uint64_t sectors_written;

    uint64_t iops_read;
    uint64_t iops_written;
} Storage_info;

/*
*@function: get_storage_info
*@description: get the storage information from /proc/diskstats
*@param: storage_info: a pointer to a Storage_info struct to store the storage information
*/
void get_storage_info(Storage_info *storage_info);

/*
*@function: get_speed_and_iops_info
*@description: get the read and write speed of the storage, as well as the IOPS
*@param: storage_info: a pointer to a storage_info struct to store the read and write speed, as well as the IOPS
* this function creates a file named "storage_speed.bin" in the /home/shunkun
*/
void get_speed_and_iops_info(Storage_info *storage_info);

/*
*@function: get_sectors_info
*@description: get the number of sectors read and written
*@param: storage_info: a pointer to a storage_info struct to store the sectors read and written
*/
void get_sectors_info(Storage_info *storage_info);



#endif // _STORAGE_H