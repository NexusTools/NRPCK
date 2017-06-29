#ifndef NRPCK_DEVICES
#define NRPCK_DEVICES 1

#include "nrpck_types.h"
#include "nrpck_device_structs.h"
#include "nrpck_device_types.h"
#include "nrpck_device_ids.h"

typedef struct MMU {
    unsigned char padding;
    unsigned char mode;
    unsigned char rootpage;
    unsigned char childpage;
    unsigned int childoffset;
    unsigned int rootoffset;
    unsigned int len;
    unsigned char special;
    unsigned char crystal;
} MMU;

typedef struct NRPCKDevice {
    union NRPCKDeviceData {
        Disk disk;
        Console console;
        CCMonitor ccmonitor;
        HiFiConsole hificonsole;
        IOExpander ioexpander;
        Activator activator;
        Sortron sortron;
        CBlock cblock;
        Matrix matrix;
        Modem modem;
        MMU mmu;

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

uchar nrpck_installed_driver_count(void);
NRPCKDeviceDriver* nrpck_installed_driver(uchar id);
uchar nrpck_device_driver_count(NRPCKDeviceRef* ref);

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