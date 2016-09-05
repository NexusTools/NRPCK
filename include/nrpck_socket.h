#ifndef NRPCK_SOCKET
#define NRPCK_SOCKET

#include <nrpck_devices.h>

#define nrpck_socket_drv_querymaxconnections(driver, device) \
	((uchar(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_socket_drv_connect(device, driver, address, index, tick) \
	((schar(*)(NRPCKDevice*, char*, uchar, voidmethod))driver->methods[1])(device, address, index, tick)

#define nrpck_socket_drv_disconnect(device, driver, index) \
	((void(*)(NRPCKDevice*, uchar))driver->methods[2])(device, index)

#define nrpck_socket_drv_read(device, driver, buffer, len, index, tick) \
	((int(*)(NRPCKDevice*, char*, uchar, uchar, voidmethod))driver->methods[3])(device, buffer, len, index, tick)

#define nrpck_socket_drv_write(device, driver, buffer, len, index, tick) \
	((int(*)(NRPCKDevice*, char*, uchar, uchar, voidmethod))driver->methods[4])(device, buffer, len, index, tick)

#endif