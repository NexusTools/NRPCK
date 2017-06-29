#include "nrpck_devices.h"
#include "nrpck_drv_disk.h"
#include "nrpck_errors.h"
#include "nrpck_types.h"

#include <string.h>
#include <stdio.h>

#define DISK_IDLE        0
#define DISK_READ_NAME     1
#define DISK_WRITE_NAME    2
#define DISK_READ_SERIAL   3
#define DISK_READ      4
#define DISK_WRITE      5
#define DISK_READ_OFFSET   6
#define DISK_WRITE_OFFSET  7
#define DISK_FAIL      0xFF

NRPCKDeviceDriver nrpck_drv_disk;

bool nrpck_drv_disk_detect(NRPCKDevice* device) {
    return device->ID == DISK_ID || device->ID == HARDDRIVE_ID;
}

uint nrpck_drv_disk_sectorcount(NRPCKDevice* device) {
    switch (device->ID) {
        case HARDDRIVE_ID:
            return 0x9FFF;

        case DISK_ID:
            return 0x7FF;

        default:
            return 0;
    }
}

int nrpck_drv_disk_read(NRPCKDevice* device, uint sector, uchar pos, char* buffer, uchar len) {
    device->data.disk.sector_num = sector;
    device->data.disk.command = DISK_READ;
    while (device->data.disk.command == DISK_READ);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_READ;
    memcpy(buffer, device->data.disk.sector + pos, len);
    return len;
}

int nrpck_drv_disk_write(NRPCKDevice* device, uint sector, uchar pos, const char* buffer, uchar len) {
    device->data.disk.sector_num = sector;
    if (len < 128) {
        device->data.disk.command = DISK_READ;
        while (device->data.disk.command == DISK_READ);
        if (device->data.disk.command == DISK_FAIL)
            return ERROR_READ;
    }
    memcpy(device->data.disk.sector + pos, buffer, len);
    device->data.disk.command = DISK_WRITE;
    while (device->data.disk.command == DISK_WRITE);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_WRITE;
    return len;
}

int nrpck_drv_disk_writeset(NRPCKDevice* device, uint sector, uchar pos, uchar byte, uchar count) {
    device->data.disk.sector_num = sector;
    if (count < 128) {
        device->data.disk.command = DISK_READ;
        while (device->data.disk.command == DISK_READ);
        if (device->data.disk.command == DISK_FAIL)
            return ERROR_READ;
    }
    memset(device->data.disk.sector + pos, byte, count);
    device->data.disk.command = DISK_WRITE;
    while (device->data.disk.command == DISK_WRITE);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_WRITE;
    return count;
}

int nrpck_drv_disk_fastwrite(NRPCKDevice* device, uint sector, uchar pos, const char* buffer, uchar len) {
    device->data.disk.sector_num = sector;
    memcpy(device->data.disk.sector + pos, buffer, len);
    device->data.disk.command = DISK_WRITE;
    while (device->data.disk.command == DISK_WRITE);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_WRITE;
    return len;
}

schar nrpck_drv_disk_getlabel(NRPCKDevice* device, char* buffer) {
    if (device->ID == HARDDRIVE_ID)
        return ERROR_GENERIC;

    device->data.disk.command = DISK_READ_NAME;
    while (device->data.disk.command == DISK_READ_NAME);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_GENERIC;
    strcpy(buffer, device->data.disk.sector);
    return strlen(buffer);
}

schar nrpck_drv_disk_setlabel(NRPCKDevice* device, const char* buffer) {
    if (device->ID == HARDDRIVE_ID)
        return ERROR_GENERIC;

    strcpy(device->data.disk.sector, buffer);
    device->data.disk.command = DISK_WRITE_NAME;
    while (device->data.disk.command == DISK_WRITE_NAME);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_GENERIC;
    return 0;
}

schar nrpck_drv_disk_getserial(NRPCKDevice* device, char* buffer) {
    device->data.disk.command = DISK_READ_SERIAL;
    while (device->data.disk.command == DISK_READ_SERIAL);
    if (device->data.disk.command == DISK_FAIL)
        return ERROR_GENERIC;
    memcpy(buffer, device->data.disk.sector, 16);
    buffer[16] = 0;
    return 16;
}

void nrpck_drv_disk_describe(NRPCKDevice* device, char* buffer) {
    char serial[17], label[128];

    strcpy(serial, "????????????????");
    if (device->ID == HARDDRIVE_ID) {
        nrpck_drv_disk_getserial(device, serial);
        sprintf(buffer, "[%-16s] 5MB Disk", serial);
        return;
    }

    strcpy(label, "no media");
    nrpck_drv_disk_getserial(device, serial);
    nrpck_drv_disk_getlabel(device, label);

    sprintf(buffer, "[%-16s] %s", serial, label);
}

uchar nrpck_drive_drv_sectorsize(NRPCKDevice*) {
    return 128;
}

void nrpck_init_driver_disk() {
    nrpck_drv_disk.name = "RedPower Disk Driver";
    nrpck_drv_disk.device_type = DISK_TYPE;
    nrpck_drv_disk.detect = nrpck_drv_disk_detect;
    nrpck_drv_disk.methods[0] = nrpck_drv_disk_sectorcount;
    nrpck_drv_disk.methods[1] = nrpck_drv_disk_read;
    nrpck_drv_disk.methods[2] = nrpck_drv_disk_write;
    nrpck_drv_disk.methods[3] = nrpck_drv_disk_fastwrite;
    nrpck_drv_disk.methods[4] = nrpck_drv_disk_getlabel;
    nrpck_drv_disk.methods[5] = nrpck_drv_disk_setlabel;
    nrpck_drv_disk.methods[6] = nrpck_drv_disk_getserial;
    nrpck_drv_disk.methods[7] = nrpck_drv_disk_writeset;
    nrpck_drv_disk.methods[0xA] = nrpck_drive_drv_sectorsize;
    nrpck_drv_disk.methods[0xF] = nrpck_drv_disk_describe;
    nrpck_device_register_driver(&nrpck_drv_disk);
}