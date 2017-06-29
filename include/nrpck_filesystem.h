#ifndef NRPCK_FILESYSTEM
#define NRPCK_FILESYSTEM

#include "nrpck_devices.h"

typedef struct NRPCKFileSystem {
	const char* name;
	schar (*detect)(NRPCKDevice* device, NRPCKDeviceDriver* driver);
	schar (*format)(NRPCKDevice* device, NRPCKDeviceDriver* driver);
} NRPCKFileSystem;

schar nrpck_filesystem_detect(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* fs);

schar nrpck_filesystem_format(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* fs);

void nrpck_filesystem_register(NRPCKFileSystem* filesystem);

NRPCKFileSystem* nrpck_filesystem_get(uchar id);

uchar nrpck_filesystem_count(void);

#endif