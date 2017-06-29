#ifndef NRPCK_MOUSE
#define NRPCK_MOUSE 1

#include "nrpck_devices.h"

#define nrpck_mouse_drv_position(mouseDevice, driver, x, y) \
	((void(*)(NRPCKDevice*, uchar*, uchar*))driver->methods[0])(mouseDevice, x, y)

#define nrpck_mouse_drv_clicked(mouseDevice, driver, x, y) \
	((bool(*)(NRPCKDevice*, uchar*, uchar*))driver->methods[1])(mouseDevice, x, y)

#endif