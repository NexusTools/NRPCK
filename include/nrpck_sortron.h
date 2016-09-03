#ifndef NRPCK_SORTRON
#define NRPCK_SORTRON

#include "nrpck_devices.h"

#define nrpck_sortron_drv_count(device, driver) \
	((long(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_sortron_drv_load(device, driver, slot, item) \
	((schar(*)(NRPCKDevice*, uint, SortronItem*))driver->methods[1])(device, slot, item)

#define nrpck_sortron_drv_pull(device, driver, count, color) \
	((schar(*)(NRPCKDevice*, uchar, uchar))driver->methods[2])(device, count, color)

#define nrpck_sortron_drv_match(device, driver, count, color) \
	((schar(*)(NRPCKDevice*, uchar, uchar))driver->methods[3])(device, count, color)

#endif
