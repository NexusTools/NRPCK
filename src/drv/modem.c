#include "nrpck.h"
#include "nrpck_drv_modem.h"
#include <string.h>

NRPCKDeviceDriver nrpck_drv_modem;

bool nrpck_drv_modem_detect(NRPCKDevice* device) {
    return device->ID == MODEM_ID;
}

uchar nrpck_drv_modem_querymaxconnections(NRPCKDevice*) {
    return 0xFF;
}

schar nrpck_drv_modem_connect(NRPCKDevice* device, char* address, uchar index, void(*tick)(void)) {
    if (!tick)
        tick = nrpck_sleep10ms;
    device->data.modem.index = index;
    device->data.modem.inlen = strlen(address);
    strcpy(device->data.modem.buffer.duplex.in, address);
    device->data.modem.incmd = 1;
    while (device->data.modem.incmd == 1) {
        tick();
    }
    return device->data.modem.incmd == 0 ? 0 : ERROR_GENERIC;
}

void nrpck_drv_modem_disconnect(NRPCKDevice* device, uchar index) {
    device->data.modem.index = index;
    device->data.modem.outcmd = 2;
    while (device->data.modem.outcmd == 2);
}

int nrpck_drv_modem_read(NRPCKDevice* device, char* buffer, uchar len, void(*tick)(void)) {
    if (!tick)
        tick = nrpck_sleep10ms;
    device->data.modem.inlen = len;
    device->data.modem.incmd = 3;
    while (device->data.modem.incmd == 3) {
        tick();
    }
    if (device->data.modem.incmd != 0)
        return ERROR_READ;
    len = device->data.modem.inlen;
    if (len > 0) {
        memcpy(buffer, device->data.modem.buffer.duplex.in, len);
        return len;
    }
    return ERROR_END_OF_STREAM;
}

int nrpck_drv_modem_write(NRPCKDevice* device, char* buffer, uchar len, void(*tick)(void)) {
    if (!tick)
        tick = nrpck_sleep10ms;

    memcpy(device->data.modem.buffer.duplex.out, buffer, device->data.modem.outlen = len);
    device->data.modem.outcmd = 4;
    while (device->data.modem.outcmd == 4) {
        tick();
    }
    if (device->data.modem.outcmd != 0)
        return ERROR_WRITE;
    len = device->data.modem.outlen;
    if (len > 0)
        return len;
    return ERROR_END_OF_STREAM;
}

void nrpck_drv_modem_switch(NRPCKDevice* device, uchar index) {
    device->data.modem.index = index;
}

void nrpck_drv_modem_describe(NRPCKDevice*, char* buffer) {
    strcpy(buffer, "Socket Capable IO Device");
}

void nrpck_init_driver_modem() {
    nrpck_drv_modem.name = "AeroScripts Modem Driver";
    nrpck_drv_modem.device_type = MODEM_TYPE;
    nrpck_drv_modem.detect = nrpck_drv_modem_detect;
    nrpck_drv_modem.methods[0] = nrpck_drv_modem_querymaxconnections;
    nrpck_drv_modem.methods[1] = nrpck_drv_modem_connect;
    nrpck_drv_modem.methods[2] = nrpck_drv_modem_disconnect;
    nrpck_drv_modem.methods[3] = nrpck_drv_modem_read;
    nrpck_drv_modem.methods[4] = nrpck_drv_modem_write;
    nrpck_drv_modem.methods[5] = nrpck_drv_modem_switch;
    nrpck_drv_modem.methods[0xF] = nrpck_drv_modem_describe;
    nrpck_device_register_driver(&nrpck_drv_modem);
}
