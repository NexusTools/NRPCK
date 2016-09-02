#include "nrpck_devices.h"
#include "nrpck_drv_modem.h"
#include "nrpck_errors.h"
#include <string.h>

NRPCKDeviceDriver nrpck_drv_modem;

bool nrpck_drv_modem_detect(NRPCKDevice* device) {
	return device->ID == MODEM_ID;
}

uchar nrpck_drv_modem_querymaxconnections(NRPCKDevice* device) {
	return 0xFF;
}

schar nrpck_drv_modem_connect(NRPCKDevice* device, char* address, uchar index) {
	strcpy(device->data.modem.buffer, address);
	device->data.modem.len = strlen(address);
	device->data.modem.index = index;
	device->data.modem.command = 1;
	return device->data.modem.command > -1 ? 0 : ERROR_GENERIC;
}

void nrpck_drv_modem_disconnect(NRPCKDevice* device, uchar index) {
	device->data.modem.index = index;
	device->data.modem.command = 2;
}

int nrpck_drv_modem_read(NRPCKDevice* device, char* buffer, uchar len, uchar index) {
	device->data.modem.len = len;
	device->data.modem.index = index;
	device->data.modem.command = 3;
	if(device->data.modem.command < 0)
		return ERROR_READ;
	len = device->data.modem.len;
	if(len > 0) {
		memcpy(buffer, device->data.modem.buffer, len);
		return len;
	}
	return ERROR_END_OF_STREAM;
}

int nrpck_drv_modem_write(NRPCKDevice* device, char* buffer, uchar len, uchar index) {
	if(index != 0)
		return ERROR_GENERIC;
	
	device->data.modem.index = index;
	memcpy(device->data.modem.buffer, buffer, device->data.modem.len = len);
	device->data.modem.command = 4;
	if(device->data.modem.command < 0)
		return ERROR_WRITE;
	len = device->data.modem.len;
	if(len > 0)
		return len;
	return ERROR_END_OF_STREAM;
}

void nrpck_drv_modem_setbaudrate(NRPCKDevice* device, uint rate) {
	memcpy(device->data.modem.buffer, &rate, 4);
	device->data.modem.command = 9;
}

void nrpck_drv_modem_describe(NRPCKDevice*, char* buffer) {
	strcpy(buffer, "Socket Capable IO Device");
}

void nrpck_init_driver_modem() {
	nrpck_drv_modem.name = "AeroScripts Modem Driver";
	nrpck_drv_modem.device_type = MODEM_TYPE;
	nrpck_drv_modem.detect = nrpck_drv_modem_detect;
	nrpck_drv_modem.methods[0] = nrpck_drv_modem_querymaxconnections;
	nrpck_drv_modem.methods[1] = nrpck_drv_modem_connect;
	nrpck_drv_modem.methods[2] = nrpck_drv_modem_disconnect;
	nrpck_drv_modem.methods[3] = nrpck_drv_modem_read;
	nrpck_drv_modem.methods[4] = nrpck_drv_modem_write;
	nrpck_drv_modem.methods[5] = nrpck_drv_modem_setbaudrate;
	nrpck_drv_modem.methods[0xF] = nrpck_drv_modem_describe;
	nrpck_device_register_driver(&nrpck_drv_modem);
}
