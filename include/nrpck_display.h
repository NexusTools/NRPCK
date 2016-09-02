#ifndef NRPCK_DISPLAY
#define NRPCK_DISPLAY 1

#include "nrpck_devices.h"
#include "nrpck_types.h"

schar nrpck_display_boot(const char* banner, uchar port_start, uchar port_end, NRPCKDeviceRef** displayDevice, NRPCKDeviceDriver** displayDriver, uchar* widthOut, uchar* heightOut);
void nrpck_display_bootscreen(const char* banner, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver, uchar* widthOut, uchar* heightOut);

schar nrpck_display_clear(NRPCKDeviceRef* displayDevice);
schar nrpck_display_clearline(NRPCKDeviceRef* displayDevice, uchar y);
schar nrpck_display_set(NRPCKDeviceRef* displayDevice, uchar x, uchar y, uchar c, uchar count);
schar nrpck_display_line(NRPCKDeviceRef* displayDevice, uchar x, uchar y, const char* str, uchar len);
schar nrpck_display_size(NRPCKDeviceRef* displayDevice, uchar* width, uchar* height);
schar nrpck_display_movecursor(NRPCKDeviceRef* displayDevice, uchar x, uchar y);
schar nrpck_display_setcursor(NRPCKDeviceRef* displayDevice, bool visible);
schar nrpck_display_hascursor(NRPCKDeviceRef* displayDevice);

#define nrpck_display_drv_set(displayDevice, driver, x, y, c, count) \
	((void(*)(NRPCKDevice*, uchar, uchar, uchar, uchar))driver->methods[0])(displayDevice, x, y, c, count)

#define nrpck_display_drv_line(displayDevice, driver, x, y, str, len) \
	((void(*)(NRPCKDevice*, uchar, uchar, const char*, uchar))driver->methods[1])(displayDevice, x, y, str, len)

#define nrpck_display_drv_hascursor(displayDevice, driver) \
	((bool(*)(NRPCKDevice*))driver->methods[2])(displayDevice)

#define nrpck_display_drv_movecursor(displayDevice, driver, x, y) \
	((void(*)(NRPCKDevice*, uchar, uchar))driver->methods[3])(displayDevice, x, y)

#define nrpck_display_drv_setcursor(displayDevice, driver, visible) \
	((void(*)(NRPCKDevice*, bool))driver->methods[4])(displayDevice, visible)

#define nrpck_display_drv_size(displayDevice, driver, width, height) \
	((void(*)(NRPCKDevice*, uchar*, uchar*))driver->methods[5])(displayDevice, width, height)

#endif