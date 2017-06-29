#ifndef NRPCK_CBLOCK
#define NRPCK_CBLOCK

#include "nrpck_devices.h"

#define nrpck_cblock_drv_setblock(device, driver, x, y, z, blockId, blockMeta) \
	((schar(*)(NRPCKDevice*, long, uchar, long, uint, uint))driver->methods[0])(device, x, y, z, blockId, blockMeta)

#endif
