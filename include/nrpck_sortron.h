#ifndef NRPCK_SORTRON
#define NRPCK_SORTRON

#include "nrpck_devices.h"

#define nrpck_sortron_drv_load(device, driver, slot) \
	((uint(*)(NRPCKDevice*, uchar slot))driver->methods[0])(device)

#define nrpck_ioexpander_drv_write(device, driver, out) \
	((void(*)(NRPCKDevice*, uint))driver->methods[1])(device, out)

#endif
