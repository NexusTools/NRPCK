#include "nrpck.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void nrpck_display_print0(const char* line, uchar len, uchar* y, uchar width, uchar height, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver) {
	if(len > width) {
		nrpck_display_drv_line(display->device, displayDriver, 0, *y, line, width);
		*y = *y + 1;
		if(*y >= height)
			*y = 0;
		nrpck_display_drv_set(display->device, displayDriver, 0, *y, ' ', width);
		nrpck_display_print0(line+width, len-width, y, width, height, display, displayDriver);
	} else {
		nrpck_display_drv_line(display->device, displayDriver, 0, *y, line, len);
		*y = *y + 1;
		if(*y >= height)
			*y = 0;
		nrpck_display_drv_set(display->device, displayDriver, 0, *y, ' ', width);
	}
}
void nrpck_display_print(const char* line, uchar* y, uchar width, uchar height, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver) {
	nrpck_display_print0(line, strlen(line), y, width, height, display, displayDriver);
}

uchar nrpck_display_bootscreen(const char* banner, NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver, uchar* widthOut, uchar* heightOut) {
	uchar width, height, y, i;
	char str[80];
	
	y = 0;
	nrpck_display_drv_setcursor(display->device, displayDriver, false);
	nrpck_display_drv_size(display->device, displayDriver, &width, &height);
	for(i=0;i<height;i++)
		nrpck_display_drv_set(display->device, displayDriver, 0, i, ' ', width);
	
	nrpck_display_print(banner, &y, width, height, display, displayDriver);
	nrpck_display_print("Powered by NRPCK " NRPCK_KERNEL_VERSION, &y, width, height, display, displayDriver);
	nrpck_display_print("", &y, width, height, display, displayDriver);
	
	sprintf(str, "Available Memory: %uB", nrpck_memory_available());
	nrpck_display_print(str, &y, width, height, display, displayDriver);
	sprintf(str, "Display Device: %s (0x%02X)", nrpck_device_name(display->device->ID), display->rbport);
	nrpck_display_print(str, &y, width, height, display, displayDriver);
	sprintf(str, "Display Driver: %s (0x%04X)", displayDriver->name, displayDriver);
	nrpck_display_print(str, &y, width, height, display, displayDriver);
	nrpck_display_print("", &y, width, height, display, displayDriver);

	if(widthOut)
		*widthOut = width;
	if(heightOut)
		*heightOut = height;
	
	return y;
}

schar nrpck_display_boot(const char* banner, uchar port_start, uchar port_end, NRPCKDeviceRef** displayOut, NRPCKDeviceDriver** displayDriverOut, uchar* widthOut, uchar* heightOut, uchar* yOut, ...) {
	NRPCKDeviceDriver* displayDriver, **refDriver;
	NRPCKDeviceRef* display, **ref;
	uchar device_type, width, height;
	char str[80];
	va_list ap;
	uchar i, y;
	
	nrpck_init();
	for(i=port_start;i<=port_end;i++)
		if((displayDriver = nrpck_device_lookupdriver(display = nrpck_device_map(i), TEXT_DISPLAY_TYPE))) {
			if(displayDriverOut)
				*displayDriverOut = displayDriver;
			if(displayOut)
				*displayOut = display;
			goto founddisplay;
		}
	
	
	if(yOut)
		*yOut = y;
	return ERROR_GENERIC;
founddisplay:
	y = nrpck_display_bootscreen(banner, display, displayDriver, &width, &height);
	
	va_start(ap, yOut);
	while(true) {
		device_type = va_arg(ap, uchar);
		if(!device_type)
			break;
		
		ref = va_arg(ap, NRPCKDeviceRef**);
		if(!ref)
			break;

		refDriver = va_arg(ap, NRPCKDeviceDriver**);
		if(!refDriver)
			break;
		
		*ref = 0;
		sprintf(str, "Scanning for %s Compatible Device", nrpck_driver_type(device_type));
		nrpck_display_print(str, &y, width, height, display, displayDriver);
		for(i=port_start;i<=port_end;i++)
			if((*refDriver = nrpck_device_lookupdriver(*ref = nrpck_device_map(i), device_type))) {
				sprintf(str, "Found %s on port 0x%02X!", nrpck_device_name((*ref)->device->ID), (*ref)->rbport);
				nrpck_device_remap(display);
				nrpck_display_print(str, &y, width, height, display, displayDriver);
				nrpck_display_print("", &y, width, height, display, displayDriver);
				break;
			}
		
		if(!*ref) {
			nrpck_device_remap(display);
			nrpck_display_print("Device Not Found...", &y, width, height, display, displayDriver);
			if(widthOut)
				*widthOut = width;
			if(heightOut)
				*heightOut = height;
			if(yOut)
				*yOut = y;
			va_end(ap);
			return ERROR_GENERIC;
		}
	}
	va_end(ap);
		
	if(widthOut)
		*widthOut = width;
	if(heightOut)
		*heightOut = height;
	if(yOut)
		*yOut = y;
	return 0;
}

