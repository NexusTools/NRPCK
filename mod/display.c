#include "nrpck.h"

#include <string.h>
#include <stdio.h>

void nrpck_display_bootprint0(const char* line, uchar len, uchar* y, uchar width, uchar height, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver) {
	//nrpck_display_drv_set(display->device, displayDriver, 0, *y, ' ', width);
	
	if(len > width) {
		nrpck_display_drv_line(display->device, displayDriver, 0, *y, line, width);
		*y = *y + 1;
		if(*y >= height)
			*y = 0;
		nrpck_display_drv_set(display->device, displayDriver, 0, *y, ' ', width);
		nrpck_display_bootprint0(line+width, len-width, y, width, height, display, displayDriver);
	} else {
		nrpck_display_drv_line(display->device, displayDriver, 0, *y, line, len);
		*y = *y + 1;
		if(*y >= height)
			*y = 0;
		nrpck_display_drv_set(display->device, displayDriver, 0, *y, ' ', width);
	}
}
void nrpck_display_bootprint(const char* line, uchar* y, uchar width, uchar height, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver) {
	nrpck_display_bootprint0(line, strlen(line), y, width, height, display, displayDriver);
}

void nrpck_display_bootscreen(const char* banner, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver, uchar* widthOut, uchar* heightOut) {
	uchar width, height, y, i;
	char str[80];
	
	y = 0;
	nrpck_display_drv_setcursor(display->device, displayDriver, false);
	nrpck_display_drv_size(display->device, displayDriver, &width, &height);
	for(i=0;i<height;i++)
		nrpck_display_drv_set(display->device, displayDriver, 0, i, ' ', width);
	
	nrpck_display_bootprint(banner, &y, width, height, display, displayDriver);
	nrpck_display_bootprint("Powered by NRPCK " NRPCK_KERNEL_VERSION, &y, width, height, display, displayDriver);
	nrpck_display_bootprint("", &y, width, height, display, displayDriver);
	
	sprintf(str, "Available Memory: %uB", nrpck_memory_available());
	nrpck_display_bootprint(str, &y, width, height, display, displayDriver);
	sprintf(str, "Display Device: %s (0x%02X)", nrpck_device_name(display->device->ID), display->rbport);
	nrpck_display_bootprint(str, &y, width, height, display, displayDriver);
	sprintf(str, "Display Driver: %s (0x%04X)", displayDriver->name, displayDriver);
	nrpck_display_bootprint(str, &y, width, height, display, displayDriver);

	if(widthOut)
		*widthOut = width;
	if(heightOut)
		*heightOut = height;
}

schar nrpck_display_boot(const char* banner, uchar port_start, uchar port_end, NRPCKDeviceRef** displayOut, NRPCKDeviceDriver** displayDriverOut, uchar* width, uchar* height) {
	NRPCKDeviceDriver* displayDriver;
	NRPCKDeviceRef* display;
	uchar i;
	
	nrpck_init();
	for(i=port_start;i<=port_end;i++)
		if((displayDriver = nrpck_device_lookupdriver(display = nrpck_device_map(i), TEXT_DISPLAY_TYPE))) {
			nrpck_display_bootscreen(banner, display, displayDriver, width, height);
			if(displayDriverOut)
				*displayDriverOut = displayDriver;
			if(displayOut)
				*displayOut = display;
			return 0;
		}
	
	return ERROR_GENERIC;
}

