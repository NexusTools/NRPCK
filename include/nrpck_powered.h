#ifndef NRPCK_POWERED
#define NRPCK_POWERED

#include "nrpck_devices.h"

#define nrpck_powered_drv_readlevel(device, driver) \
	((uchar(*)(NRPCKDevice*))driver->methods[0])(device)

#endif