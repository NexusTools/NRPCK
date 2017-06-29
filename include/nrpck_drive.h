#ifndef NRPCK_DRIVE
#define NRPCK_DRIVE 1

#include "nrpck_devices.h"

#define nrpck_drive_drv_sectorcount(device, driver) \
	return ((uint(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_drive_drv_read(device, driver, sector, pos, buffer, len) \
	((int(*)(NRPCKDevice*, uint, uchar, char*, uchar))driver->methods[1])(device, sector, pos, buffer, len)

#define nrpck_drive_drv_write(device, driver, sector, pos, buffer, len) \
	((int(*)(NRPCKDevice*, uint, uchar, const char*, uchar))driver->methods[2])(device, sector, pos, buffer, len)

#define nrpck_drive_drv_writeset(device, driver, sector, pos, byte, count) \
	((int(*)(NRPCKDevice*, uint, uchar, uchar, uchar))driver->methods[7])(device, sector, pos, byte, count)

#define nrpck_drive_drv_fastwrite(device, driver, sector, pos, buffer, len) \
	((int(*)(NRPCKDevice*, uint, uchar, const char*, uchar))driver->methods[3])(device, sector, pos, buffer, len)

#define nrpck_drive_drv_getlabel(device, driver, buffer) \
	((schar(*)(NRPCKDevice*, char*))driver->methods[4])(device, buffer)

#define nrpck_drive_drv_setlabel(device, driver, buffer) \
	((schar(*)(NRPCKDevice*, const char*))driver->methods[5])(device, buffer)

#define nrpck_drive_drv_getserial(device, driver, buffer) \
	((schar(*)(NRPCKDevice*, char*))driver->methods[6])(device, buffer)

#define nrpck_drive_drv_sectorsize(device, driver) \
	((uchar(*)(NRPCKDevice*))driver->methods[0xA])(device)

#endif
