#include "nrpck.h"

#include <string.h>
#include <stdio.h>

NRPCKDeviceDriver nrpck_drv_ccmonitor_display;

bool nrpck_drv_ccmonitor_detect(NRPCKDevice* device) {
    return device->ID == CCMONITOR_ID;
}

void nrpck_drv_ccmonitor_set(NRPCKDevice* device, uchar x, uchar y, uchar c, uchar count) {
    device->data.ccmonitor.x = x;
    device->data.ccmonitor.y = y;
    memset(device->data.ccmonitor.buffer, c, count);
    device->data.ccmonitor.buffer[count] = 0;

    device->data.ccmonitor.cmd = 1;
    while (device->data.ccmonitor.cmd == 1);
}

void nrpck_drv_ccmonitor_line(NRPCKDevice* device, uchar x, uchar y, char* str, uchar len) {
    device->data.ccmonitor.x = x;
    device->data.ccmonitor.y = y;
    memcpy(device->data.ccmonitor.buffer, str, len);
    device->data.ccmonitor.buffer[len] = 0;

    device->data.ccmonitor.cmd = 1;
    while (device->data.ccmonitor.cmd == 1);
}

bool nrpck_drv_ccmonitor_hascursor(NRPCKDevice*) {
    return false;
}

void nrpck_drv_ccmonitor_movecursor(NRPCKDevice*, uchar, uchar) {
}

void nrpck_drv_ccmonitor_setcursor(NRPCKDevice*, bool) {
}

void nrpck_drv_ccmonitor_size(NRPCKDevice*, uchar* width, uchar* height) {
    if (width)
        *width = 80;
    if (height)
        *height = 24;
}

void nrpck_drv_ccmonitor_describe(NRPCKDevice* device, char* buffer) {
    uchar width, height;

    nrpck_drv_ccmonitor_size(device, &width, &height);
    sprintf(buffer, "%ux%u Text Display Device", width, height);
}

void nrpck_drv_ccmonitor_setcursorcolour(NRPCKDevice* device, uchar colour) {
    device->data.ccmonitor.color = colour;
}

void nrpck_drv_ccmonitor_setcursorbgcolour(NRPCKDevice* device, uchar colour) {
    device->data.ccmonitor.background = colour;
}

void nrpck_drv_ccmonitor_reset(NRPCKDevice* device) {
    device->data.ccmonitor.scale = 2;

    device->data.ccmonitor.cmd = 3;
    while (device->data.ccmonitor.cmd == 3);
}

void nrpck_init_driver_ccmonitor() {
    nrpck_drv_ccmonitor_display.name = "ComputerCraft Display Driver";
    nrpck_drv_ccmonitor_display.device_type = TEXT_DISPLAY_TYPE;
    nrpck_drv_ccmonitor_display.detect = nrpck_drv_ccmonitor_detect;
    nrpck_drv_ccmonitor_display.methods[0] = nrpck_drv_ccmonitor_set;
    nrpck_drv_ccmonitor_display.methods[1] = nrpck_drv_ccmonitor_line;
    nrpck_drv_ccmonitor_display.methods[2] = nrpck_drv_ccmonitor_hascursor;
    nrpck_drv_ccmonitor_display.methods[3] = nrpck_drv_ccmonitor_movecursor;
    nrpck_drv_ccmonitor_display.methods[4] = nrpck_drv_ccmonitor_setcursor;
    nrpck_drv_ccmonitor_display.methods[5] = nrpck_drv_ccmonitor_size;
    nrpck_drv_ccmonitor_display.methods[6] = nrpck_noop;
    nrpck_drv_ccmonitor_display.methods[7] = nrpck_drv_ccmonitor_setcursorcolour;
    nrpck_drv_ccmonitor_display.methods[8] = nrpck_drv_ccmonitor_setcursorbgcolour;
    nrpck_drv_ccmonitor_display.methods[9] = nrpck_noop;
    nrpck_drv_ccmonitor_display.methods[0xE] = nrpck_drv_ccmonitor_reset;
    nrpck_drv_ccmonitor_display.methods[0xF] = nrpck_drv_ccmonitor_describe;
    nrpck_device_register_driver(&nrpck_drv_ccmonitor_display);
}