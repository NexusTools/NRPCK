#include "nrpck_devices.h"
#include "nrpck_drv_matrix.h"

#include <string.h>

NRPCKDeviceDriver nrpck_drv_matrix_display;

bool nrpck_drv_matrix_detect(NRPCKDevice* device) {
	return device->ID == MATRIX_ID;
}

void nrpck_drv_matrix_set(NRPCKDevice* device, uchar x, uchar y, uchar c, uchar count) {
	device->data.matrix.blitx = x;
	device->data.matrix.blity = y;
	memset(device->data.matrix.buffer, c, count);
	device->data.matrix.buffer[count] = 0;
	
	device->data.matrix.command = 3;
    while(device->data.matrix.command == 3);
}

void nrpck_drv_matrix_line(NRPCKDevice* device, uchar x, uchar y, char* str, uchar len) {
	device->data.matrix.blitx = x;
    device->data.matrix.blity = y;
    memcpy(device->data.matrix.buffer, str, len);
	device->data.matrix.buffer[len] = 0;
	
    device->data.matrix.command = 3;
    while(device->data.matrix.command == 3);
}

bool nrpck_drv_matrix_hascursor(NRPCKDevice*) {
	return false;
}

void nrpck_drv_matrix_movecursor(NRPCKDevice*, uchar, uchar) {
}

void nrpck_drv_matrix_setcursor(NRPCKDevice*, bool) {
}

void nrpck_drv_matrix_size(NRPCKDevice* device, uchar* width, uchar* height) {
	device->data.matrix.command = 1;
    while(device->data.matrix.command == 1);
	
	if(width)
		*width = device->data.matrix.width;
	if(height)
		*height = device->data.matrix.height;
}

void nrpck_init_driver_matrix() {
	nrpck_drv_matrix_display.name = "AeroScripts Matrix Display Driver";
	nrpck_drv_matrix_display.device_type = TEXT_DISPLAY_TYPE;
	nrpck_drv_matrix_display.detect = nrpck_drv_matrix_detect;
	nrpck_drv_matrix_display.methods[0] = nrpck_drv_matrix_set;
	nrpck_drv_matrix_display.methods[1] = nrpck_drv_matrix_line;
	nrpck_drv_matrix_display.methods[2] = nrpck_drv_matrix_hascursor;
	nrpck_drv_matrix_display.methods[3] = nrpck_drv_matrix_movecursor;
	nrpck_drv_matrix_display.methods[4] = nrpck_drv_matrix_setcursor;
	nrpck_drv_matrix_display.methods[5] = nrpck_drv_matrix_size;
	nrpck_device_register_driver(&nrpck_drv_matrix_display);
}