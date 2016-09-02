#include "nrpck_devices.h"
#include "nrpck_drv_sortron.h"
#include "nrpck_errors.h"
#include "nrpck_types.h"

#include <string.h>

#define SORT_SUCCEEDED	0
#define SORT_COUNT		1
#define SORT_LOAD		2
#define SORT_PULL		3
#define SORT_MATCH		4
#define SORT_FAILED		0xFF

NRPCKDeviceDriver nrpck_drv_sortron;
NRPCKDeviceDriver nrpck_drv_sortron_powered;

bool nrpck_drv_sortron_detect(NRPCKDevice* device) {
	return device->ID == SORT_TRON_ID;
}

uchar nrpck_drv_sortron_powered_level(NRPCKDevice* device) {
	return 100;
}

void nrpck_drv_sortron_describe(NRPCKDevice*, char* buffer) {
	strcpy(buffer, "RedStone Tube Sorting Device");
}

void nrpck_init_driver_sortron() {
	nrpck_drv_sortron.name = "RedPower Sorttron Driver";
	nrpck_drv_sortron.device_type = SORTRON_TYPE;
	nrpck_drv_sortron.detect = nrpck_drv_sortron_detect;
	nrpck_drv_sortron.methods[0xF] = nrpck_drv_sortron_describe;
	nrpck_device_register_driver(&nrpck_drv_sortron);
	
	nrpck_drv_sortron_powered.name = "RedPower Sorttron Power Level Driver";
	nrpck_drv_sortron_powered.device_type = POWERED_TYPE;
	nrpck_drv_sortron_powered.detect = nrpck_drv_sortron_detect;
	nrpck_drv_sortron_powered.methods[0] = nrpck_drv_sortron_powered_level;
	nrpck_drv_sortron_powered.methods[0xF] = nrpck_drv_sortron_describe;
	nrpck_device_register_driver(&nrpck_drv_sortron_powered);
}