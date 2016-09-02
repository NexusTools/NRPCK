#ifndef NRPCK_DRIVE
#define NRPCK_DRIVE 1

#include "nrpck_devices.h"

#define nrpck_drive_drv_size(device, driver) \
	return ((uint(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_drive_drv_read(device, driver, pos, buffer, len) \
	((int(*)(NRPCKDevice*, uint, char*, uint))driver->methods[1])(device, pos, buffer, len)

#define nrpck_drive_drv_write(device, driver, pos, buffer, len) \
	((int(*)(NRPCKDevice*, uint, char*, uint))driver->methods[2])(device, pos, buffer, len)

#define nrpck_drive_drv_fastwrite(device, driver, pos, buffer, len) \
	((int(*)(NRPCKDevice*, uint, char*, uint))driver->methods[3])(device, pos, buffer, len)

#define nrpck_drive_drv_getlabel(device, driver, buffer) \
	((schar(*)(NRPCKDevice*, char*))driver->methods[4])(device, buffer)

#define nrpck_drive_drv_setlabel(device, driver, buffer) \
	((schar(*)(NRPCKDevice*, char*))driver->methods[5])(device, buffer)

#define nrpck_drive_drv_getserial(device, driver, buffer) \
	((schar(*)(NRPCKDevice*, char*))driver->methods[6])(device, buffer)

#endif
