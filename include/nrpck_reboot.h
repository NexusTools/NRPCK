#ifndef NRPCK_REBOOT
#define NRPCK_PREBOOT

#include "nrpck_types.h"

void nrpck_reboot_init(void);
void nrpck_reboot(uchar disk_port);
void nrpck_reboot_return(void);

#endif