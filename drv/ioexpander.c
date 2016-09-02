#include "nrpck_devices.h"
#include "nrpck_drv_ioexpander.h"

NRPCKDeviceDriver nrpck_drv_ioexpander;

bool nrpck_drv_ioexpander_detect(NRPCKDevice* device) {
	return device->ID == IO_EXPANDER_ID;
}

uint nrpck_drv_ioexpander_in(NRPCKDevice* device) {
	return device->data.ioExpander.input;
}

void nrpck_drv_ioexpander_out(NRPCKDevice* device, uint out) {
	device->data.ioExpander.output = out;
}

void nrpck_init_driver_ioexpander() {
	nrpck_drv_ioexpander.name = "RedPower IO Expander Driver";
	nrpck_drv_ioexpander.device_type = IO_EXPANDER_TYPE;
	nrpck_drv_ioexpander.detect = nrpck_drv_ioexpander_detect;
	nrpck_drv_ioexpander.methods[0] = nrpck_drv_ioexpander_in;
	nrpck_drv_ioexpander.methods[1] = nrpck_drv_ioexpander_out;
	nrpck_device_register_driver(&nrpck_drv_ioexpander);
}
