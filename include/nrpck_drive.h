#ifndef NRPCK_DRIVE
#define NRPCK_DRIVE 1

#include "nrpck_devices.h"

uint nrpck_drive_drv_size(NRPCKDevice* device, NRPCKDeviceDriver* driver);

int nrpck_drive_drv_read(NRPCKDevice* device, NRPCKDeviceDriver* driver, uint pos, char* buffer, uchar len);

int nrpck_drive_drv_write(NRPCKDevice* device, NRPCKDeviceDriver* driver, uint pos, char* buffer, uchar len);

int nrpck_drive_drv_fastwrite(NRPCKDevice* device, NRPCKDeviceDriver* driver, uint pos, char* buffer, uchar len);

schar nrpck_drive_drv_getlabel(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer);

schar nrpck_drive_drv_setlabel(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer);

schar nrpck_drive_drv_getserial(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* buffer);

#endif
