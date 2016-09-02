
#include "nrpck_devices.h"
#include "nrpck_drv_disk.h"
#include "nrpck_errors.h"

#include <string.h>

NRPCKDeviceDriver nrpck_drv_disk;

bool nrpck_drv_disk_detect(NRPCKDevice* device) {
	return device->ID == DISK_ID;
}

unsigned int nrpck_drv_disk_size(NRPCKDevice* device) {
	device->data.disk.sector_num = 0;
	while(1) {
		device->data.disk.sector_num++;
		device->data.disk.command = DISK_READ;
		while(device->data.disk.command == DISK_READ);
		if(device->data.disk.command == DISK_FAIL)
			return device->data.disk.sector_num*128;
	}
}

int nrpck_drv_disk_read(NRPCKDevice* device, unsigned int pos, char* buffer, unsigned char len) {
	unsigned int a, b, c, d;
	a = pos / 128;
	b = pos - (a*128);
	c = (pos+len-1) / 128;

	if(a == c) {
		device->data.disk.sector_num = a;
		device->data.disk.command = DISK_READ;
		while(device->data.disk.command == DISK_READ);
		if(device->data.disk.command == DISK_FAIL)
			return ERROR_READ;
		memcpy(buffer, device->data.disk.sector+b, len);
	} else {
		d = (pos+len-1) - (c*128);
		return ERROR_GENERIC;
	}
	return len;
}

int nrpck_drv_disk_write(NRPCKDevice* device, unsigned int pos, char* buffer, unsigned char len) {
	unsigned int a, b, c, d;
	a = pos / 128;
	b = pos - (a*128);
	c = (pos+len-1) / 128;

	if(a == c) {
		device->data.disk.sector_num = a;
		if(len < 127) {
			device->data.disk.command = DISK_READ;
			while(device->data.disk.command == DISK_READ);
			if(device->data.disk.command == DISK_FAIL)
				return ERROR_READ;
		}
		memcpy(device->data.disk.sector+b, buffer, len);
		device->data.disk.command = DISK_WRITE;
		while(device->data.disk.command == DISK_WRITE);
		if(device->data.disk.command == DISK_FAIL)
			return ERROR_WRITE;
	} else {
		d = (pos+len-1) - (c*128);
		return ERROR_GENERIC;
	}
	return len;
}

int nrpck_drv_disk_fastwrite(NRPCKDevice* device, unsigned int pos, char* buffer, unsigned char len) {
	unsigned int a, b, c, d;
	
	a = pos / 128;
	b = pos - (a*128);
	c = (pos+len-1) / 128;

	if(a == c) {
		device->data.disk.sector_num = a;
		memcpy(device->data.disk.sector+b, buffer, len);
		device->data.disk.command = DISK_WRITE;
		while(device->data.disk.command == DISK_WRITE);
		if(device->data.disk.command == DISK_FAIL)
			return ERROR_WRITE;
	} else {
		d = (pos+len-1) - (c*128);
		return ERROR_GENERIC;
	}
	return len;
}
signed char nrpck_drv_disk_getlabel(NRPCKDevice* device, char* buffer) {
	device->data.disk.command = DISK_READ_NAME;
	while(device->data.disk.command == DISK_READ_NAME);
	if(device->data.disk.command == DISK_FAIL)
		return ERROR_GENERIC;
	strcpy(buffer, device->data.disk.sector);
	return strlen(buffer);
}
signed char nrpck_drv_disk_setlabel(NRPCKDevice* device, char* buffer) {
	strcpy(device->data.disk.sector, buffer);
	device->data.disk.command = DISK_WRITE_NAME;
	while(device->data.disk.command == DISK_WRITE_NAME);
	if(device->data.disk.command == DISK_FAIL)
		return ERROR_GENERIC;
	return 0;
}
signed char nrpck_drv_disk_getserial(NRPCKDevice* device, char* buffer) {
	device->data.disk.command = DISK_READ_SERIAL;
	while(device->data.disk.command == DISK_READ_SERIAL);
	if(device->data.disk.command == DISK_FAIL)
		return ERROR_GENERIC;
	memcpy(buffer, device->data.disk.sector, 16);
	buffer[16] = 0;
	return 16;
}
void nrpck_init_driver_disk() {
	nrpck_drv_disk.name = "RedPower Disk Driver";
	nrpck_drv_disk.device_type = DISK_TYPE;
	nrpck_drv_disk.detect = nrpck_drv_disk_detect;
	nrpck_drv_disk.methods[0] = nrpck_drv_disk_size;
	nrpck_drv_disk.methods[1] = nrpck_drv_disk_read;
	nrpck_drv_disk.methods[2] = nrpck_drv_disk_write;
	nrpck_drv_disk.methods[3] = nrpck_drv_disk_fastwrite;
	nrpck_drv_disk.methods[4] = nrpck_drv_disk_getlabel;
	nrpck_drv_disk.methods[5] = nrpck_drv_disk_setlabel;
	nrpck_drv_disk.methods[6] = nrpck_drv_disk_getserial;
	nrpck_device_register_driver(&nrpck_drv_disk);
}