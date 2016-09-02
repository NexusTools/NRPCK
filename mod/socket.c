#include "nrpck_socket.h"
#include <string.h>

schar nrpck_drv_socket_connect(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* address, uchar index) {
	return ((schar(*)(NRPCKDevice*, char*, uchar))driver->methods[1])(device, address, index);
}

void nrpck_drv_socket_disconnect(NRPCKDevice* device, NRPCKDeviceDriver* driver, uchar index) {
	((void(*)(NRPCKDevice*, uchar))driver->methods[2])(device, index);
}

int nrpck_drv_socket_read(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer, uchar len, uchar index) {
	return ((int(*)(NRPCKDevice*, char*, uchar, uchar))driver->methods[3])(device, buffer, len, index);
}

int nrpck_drv_socket_write(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer, uchar len, uchar index) {
	if(len == 0)
		len = strlen(buffer);
	return ((int(*)(NRPCKDevice*, char*, uchar, uchar))driver->methods[4])(device, buffer, len, index);
}

void nrpck_drv_socket_setbaudrate(NRPCKDevice* device, NRPCKDeviceDriver* driver, uint rate) {
	((void(*)(NRPCKDevice*, uint))driver->methods[5])(device, rate);
}