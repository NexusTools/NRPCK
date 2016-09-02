#include "nrpck_drive.h"

int nrpck_drive_drv_read(NRPCKDevice* device, NRPCKDeviceDriver* driver, uint pos, char* buffer, uchar len) {
	return ((int(*)(NRPCKDevice*, uint, char*, uint))driver->methods[1])(device, pos, buffer, len);
}

schar nrpck_drive_drv_getlabel(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer) {
	return ((schar(*)(NRPCKDevice*, char*))driver->methods[4])(device, buffer);
}

schar nrpck_drive_drv_getserial(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer) {
	return ((schar(*)(NRPCKDevice*, char*))driver->methods[6])(device, buffer);
}