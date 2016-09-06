#ifndef NRPCK_DISPLAY
#define NRPCK_DISPLAY 1

#include "nrpck_devices.h"
#include "nrpck_types.h"

schar nrpck_display_boot(const char* banner, uchar port_start, uchar port_end, NRPCKDeviceRef** displayDevice, NRPCKDeviceDriver** displayDriver, uchar* widthOut, uchar* heightOut, uchar* yOut, ...);

#define nrpck_display_print0(display, displayDriver, line, len, y, width, height) \
	nrpck_device_remap(display); \
	nrpck_display_drv_print0(display->device, displayDriver, line, len, y, width, height)

#define nrpck_display_print(display, displayDriver, line, y, width, height) \
	nrpck_device_remap(display); \
	nrpck_display_drv_print(display->device, displayDriver, line, y, width, height)

uchar nrpck_display_drv_bootscreen(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* banner, uchar* widthOut, uchar* heightOut);

uchar nrpck_display_readline(NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver, NRPCKDeviceRef* input, NRPCKDeviceDriver* inputDriver, const char* prefix, char* buffer, uchar bufferSize, uchar y, uchar width, bool(*cantype)(char,char*,uchar), void(*tick)(void));

void nrpck_display_drv_print0(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* line, uchar len, uchar* y, uchar width, uchar height);
void nrpck_display_drv_print(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* line, uchar* y, uchar width, uchar height);

#define nrpck_display_drv_clear(display, driver, y, width, height) \
	for(y=0;y<height;y++) \
		nrpck_display_drv_set(display, displayDriver, 0, y, ' ', width)

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

#define nrpck_display_drv_initcolour(displayDevice, driver, palette) \
	((void(*)(NRPCKDevice*, uchar*))driver->methods[6])(displayDevice, palette)

#define nrpck_display_drv_initcolourcga(displayDevice, driver) \
	((void(*)(NRPCKDevice*, uchar*))driver->methods[6])(displayDevice, "\x10\xff\xff\xff\x00\x00\xaa\x00\xaa\x00\x00\xaa\xaa\xaa\x00\x00\xaa\x00\xaa\xaa\x55\x00\xaa\xaa\xaa\x55\x55\x55\x55\x55\xff\x55\xff\x55\x55\xff\xff\xff\x55\x55\xff\x55\xff\xff\xff\x55\x00\x00\x00")

#define nrpck_display_drv_setcursorcolour(displayDevice, driver, colour) \
	((void(*)(NRPCKDevice*, uchar))driver->methods[7])(displayDevice, colour)

#define nrpck_display_drv_setcursorbgcolour(displayDevice, driver, colour) \
	((void(*)(NRPCKDevice*, uchar))driver->methods[8])(displayDevice, colour)

#define nrpck_display_drv_scrollup(displayDevice, driver, by) \
	((schar(*)(NRPCKDevice*, uchar))driver->methods[9])(displayDevice, by)

#endif