#ifndef NRPCK_IO_EXPANDER
#define NRPCK_IO_EXPANDER

#include "nrpck_devices.h"

#define nrpck_ioexpander_drv_read(device, driver) \
	((uint(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_ioexpander_drv_write(device, driver, out) \
	((void(*)(NRPCKDevice*, uint))driver->methods[1])(device, out)

#endif