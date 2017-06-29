#ifndef NRPCK_DISPLAY
#define NRPCK_DISPLAY 1

#include "nrpck_devices.h"
#include "nrpck_types.h"

#define DISPLAY_CAPABILITY_COLOUR   0x1
#define DISPLAY_CAPABILITY_CURSOR   0x2

#define DISPLAY_CURSOR_MODE_OFF     0x0
#define DISPLAY_CURSOR_MODE_ON      0x1
#define DISPLAY_CURSOR_MODE_BLINK   0x2

schar nrpck_display_boot(const char* banner, uchar port_start, uchar port_end, NRPCKDeviceRef** displayDevice, NRPCKDeviceDriver** displayDriver, uchar* widthOut, uchar* heightOut, uchar* yOut, ...);

#define nrpck_display_print0(display, displayDriver, line, len, y, width, height) \
	nrpck_device_remap(display); \
	nrpck_display_drv_print0(display->device, displayDriver, line, len, y, width, height)

#define nrpck_display_print(display, displayDriver, line, y, width, height) \
	nrpck_device_remap(display); \
	nrpck_display_drv_print(display->device, displayDriver, line, y, width, height)

uchar nrpck_display_drv_bootscreen(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* banner, uchar* widthOut, uchar* heightOut);

uchar nrpck_display_readline(NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver, NRPCKDeviceRef* input, NRPCKDeviceDriver* inputDriver, const char* prefix, char* buffer, uchar bufferSize, uchar y, uchar width, bool(*cantype)(char,char*,uchar), void(*tick)(void));
bool nrpck_display_readline_anything(char, char*, uchar);
bool nrpck_display_readline_hex(char, char*, uchar);

void nrpck_display_drv_print0(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* line, uchar len, uchar* y, uchar width, uchar height);
void nrpck_display_drv_print(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* line, uchar* y, uchar width, uchar height);

#define nrpck_display_drv_clear(display, driver, y, width, height) \
	for(y=0;y<height;y++) \
		nrpck_display_drv_set(display, displayDriver, 0, y, ' ', width)

#define nrpck_display_drv_set(displayDevice, driver, x, y, c, count) \
	((void(*)(NRPCKDevice*, uchar, uchar, uchar, uchar))driver->methods[0])(displayDevice, x, y, c, count)

#define nrpck_display_drv_line(displayDevice, driver, x, y, str, len) \
	((void(*)(NRPCKDevice*, uchar, uchar, const char*, uchar))driver->methods[1])(displayDevice, x, y, str, len)

#define nrpck_display_drv_capabilities(displayDevice, driver) \
	((uchar(*)(NRPCKDevice*))driver->methods[2])(displayDevice)

#define nrpck_display_drv_movecursor(displayDevice, driver, x, y) \
	((void(*)(NRPCKDevice*, uchar, uchar))driver->methods[3])(displayDevice, x, y)

#define nrpck_display_drv_setcursormode(displayDevice, driver, mode) \
	((void(*)(NRPCKDevice*, bool))driver->methods[4])(displayDevice, mode)

#define nrpck_display_drv_setcursor(displayDevice, driver, visible) \
	nrpck_display_drv_setcursormode(displayDevice, driver, visible ? DISPLAY_CURSOR_MODE_BLINK : DISPLAY_CURSOR_MODE_OFF)

#define nrpck_display_drv_size(displayDevice, driver, width, height) \
	((void(*)(NRPCKDevice*, uchar*, uchar*))driver->methods[5])(displayDevice, width, height)

#define nrpck_display_drv_initcolour(displayDevice, driver, palette) \
	((void(*)(NRPCKDevice*, uchar*))driver->methods[6])(displayDevice, palette)

#define nrpck_display_drv_initcolourcga(displayDevice, driver) \
	((void(*)(NRPCKDevice*, uchar*))driver->methods[6])(displayDevice, "\x10\x00\x00\x00\x00\x00\xaa\x00\xaa\x00\x00\xaa\xaa\xaa\x00\x00\xaa\x00\xaa\xaa\x55\x00\xaa\xaa\xaa\x55\x55\x55\x55\x55\xff\x55\xff\x55\x55\xff\xff\xff\x55\x55\xff\x55\xff\xff\xff\x55\xff\xff\xff")

#define nrpck_display_drv_setcursorcolour(displayDevice, driver, colour) \
	((void(*)(NRPCKDevice*, uchar))driver->methods[7])(displayDevice, colour)

#define nrpck_display_drv_setcursorbgcolour(displayDevice, driver, colour) \
	((void(*)(NRPCKDevice*, uchar))driver->methods[8])(displayDevice, colour)

#define nrpck_display_drv_scroll(displayDevice, driver, sx, sy, dx, dy, w, h) \
	((schar(*)(NRPCKDevice*, uchar, uchar, uchar, uchar, uchar, uchar))driver->methods[9])(displayDevice, sx, sy, dx, dy, w, h)

#define nrpck_display_drv_reset(display, driver) \
	if(driver->methods[0xE]) \
            ((void(*)(NRPCKDevice*))driver->methods[0xE])(display); \
        else { \
            uchar width, height, y; \
            nrpck_display_drv_setcursor(display, displayDriver, false); \
            nrpck_display_drv_initcolourcga(display, displayDriver); \
            nrpck_display_drv_setcursorcolour(display, displayDriver, 15); \
            nrpck_display_drv_setcursorbgcolour(display, displayDriver, 0); \
            nrpck_display_drv_size(display, displayDriver, &width, &height); \
            nrpck_display_drv_clear(display, driver, y, width, height); \
        }

#endif