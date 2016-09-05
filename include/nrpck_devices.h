#ifndef NRPCK_DEVICES
#define NRPCK_DEVICES 1

#include "nrpck_types.h"
#include "nrpck_device_structs.h"
#include "nrpck_device_types.h"
#include "nrpck_device_ids.h"

typedef struct NRPCKDevice {
	union NRPCKDeviceData {
		Disk disk;
		Console console;
		IOExpander ioexpander;
		Activator activator;
		Sortron sortron;
		Matrix matrix;
		Modem modem;
		
		uchar raw[0xFF];
	} data;
	uchar ID;
} NRPCKDevice;

typedef struct NRPCKDeviceDriver {
	const char* name;
	uchar device_type;
	bool (*detect)(NRPCKDevice*);
	void* methods[0x10];
} NRPCKDeviceDriver;

typedef struct NRPCKDeviceRef {
	NRPCKDevice* device;
	NRPCKDeviceDriver* drivers[0xF];
	uchar rbport;
} NRPCKDeviceRef;

/**
 * Initialize the Device Module
 * Must be called before nrpck_init() and anything else that depends on the device module.
 */
void nrpck_device_init(void);

const char* nrpck_driver_type(uchar driver_type);
const char* nrpck_device_name(uchar device_id);

NRPCKDeviceRef* nrpck_device_map(uchar port);

/**
 * Ensures this device is mapped if its on the redbus
 */
void nrpck_device_remap(NRPCKDeviceRef* device);


void nrpck_device_register_driver(NRPCKDeviceDriver* driver);

NRPCKDeviceDriver* nrpck_device_lookupdriver(NRPCKDeviceRef* device, uchar device_type);

schar nrpck_device_scanfordriver(uchar min_port, uchar max_port, uchar device_type, NRPCKDeviceRef** device, NRPCKDeviceDriver** driver);

/**
 * Fetch the last known id for the device on this port.
 * The known id is updated whenever nrpck_device_(re)?map is called.
 */
uchar nrpck_device_knownid(uchar port);

#define nrpck_device_nextkey(device, driver) \
	nrpck_device_remap(device); \
	nrpck_device_drv_nextkey(device->device, driver)

#define nrpck_device_drv_nextkey(device, driver) \
	((char(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_device_drv_describe(device, driver, buffer) \
	((void(*)(NRPCKDevice*, char*))driver->methods[0xF])(device, buffer)

#endif