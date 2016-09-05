#ifndef NRPCK_SORTRON
#define NRPCK_SORTRON

#include "nrpck_devices.h"

#define SORTRON_PAINT_NONE		0
#define SORTRON_PAINT_WHITE		1
#define SORTRON_PAINT_ORANGE	2
#define SORTRON_PAINT_MAGENTA	3
#define SORTRON_PAINT_LBLUE		4
#define SORTRON_PAINT_YELLOW	5
#define SORTRON_PAINT_LIME		6
#define SORTRON_PAINT_PINK		7
#define SORTRON_PAINT_GRAY		8
#define SORTRON_PAINT_LGRAY		9
#define SORTRON_PAINT_CYAN		10
#define SORTRON_PAINT_PURPLE	11
#define SORTRON_PAINT_BLUE		12
#define SORTRON_PAINT_BROWN		13
#define SORTRON_PAINT_GREEN		14
#define SORTRON_PAINT_RED		15
#define SORTRON_PAINT_BLACK		16

#define nrpck_sortron_drv_count(device, driver) \
	((long(*)(NRPCKDevice*))driver->methods[0])(device)

#define nrpck_sortron_drv_load(device, driver, slot, item) \
	((schar(*)(NRPCKDevice*, uint, SortronItem*))driver->methods[1])(device, slot, item)

#define nrpck_sortron_drv_pull(device, driver, count, color) \
	((schar(*)(NRPCKDevice*, uchar, uchar))driver->methods[2])(device, count, color)

#define nrpck_sortron_drv_match(device, driver, count, color) \
	((schar(*)(NRPCKDevice*, uchar, uchar))driver->methods[3])(device, count, color)

#endif
