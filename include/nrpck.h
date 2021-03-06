#ifndef NRPCK_KERNEL_VERSION
#define NRPCK_KERNEL_VERSION "V0.6-beta"

#include "nrpck_types.h"
#include "nrpck_errors.h"

void nrpck_enable_rb(void);
void __fastcall__ nrpck_map_rb(uchar port);

void nrpck_init(void);
void nrpck_oninit(void (*oninit)(void));

void nrpck_hault(void);
void nrpck_sleep10ms(void);
void nrpck_sleep50ms(void);
void nrpck_sleep100ms(void);
void nrpck_noop(void);

uint nrpck_memory_detect(void);
uint nrpck_bss_end(void);
uint nrpck_memory_available(void);
void* nrpck_memory_alloc(unsigned char);
void* nrpck_memory_realloc(void*, unsigned char);
void nrpck_memory_dealloc(void*);

#ifndef NRPCK_NO_DEVICES
#include "nrpck_devices.h"
#ifndef NRPCK_NO_DISPLAY
#include "nrpck_display.h"
#endif
#ifndef NRPCK_NO_DRIVE
#include "nrpck_drive.h"
#endif
#ifndef NRPCK_NO_FILESYSTEM
#include "nrpck_filesystem.h"
#endif
#ifndef NRPCK_NO_IOEXPANDER
#include "nrpck_ioexpander.h"
#endif
#ifndef NRPCK_NO_SORTRON
#include "nrpck_sortron.h"
#endif
#ifndef NRPCK_NO_SOCKET
#include "nrpck_socket.h"
#endif
#ifndef NRPCK_NO_REBOOT
#include "nrpck_reboot.h"
#endif
#endif

#endif