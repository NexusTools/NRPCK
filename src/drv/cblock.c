#include "nrpck_devices.h"
#include "nrpck_drv_cblock.h"
#include "nrpck_errors.h"
#include "nrpck_types.h"

#include <string.h>

NRPCKDeviceDriver nrpck_drv_cblock;

bool nrpck_drv_cblock_detect(NRPCKDevice* device) {
    return device->ID == CREATIVE_BLOCK_ID;
}

void nrpck_drv_cblock_set_block(NRPCKDevice* device, long x, uchar y, long z, uint blockId, uint blockMeta) {
    device->data.cblock.x = x;
    device->data.cblock.y = y;
    device->data.cblock.z = z;
    device->data.cblock.blockId = blockId;
    device->data.cblock.blockMeta = blockMeta;
    
    device->data.cblock.cmd = 1;
}

void nrpck_drv_cblock_describe(NRPCKDevice*, char* buffer) {
    strcpy(buffer, "AeroScripts Creative Block");
}

void nrpck_init_driver_cblock() {
    nrpck_drv_cblock.name = "AeroScripts Creative Block Driver";
    nrpck_drv_cblock.device_type = CREATIVE_TYPE;
    nrpck_drv_cblock.detect = nrpck_drv_cblock_detect;
    nrpck_drv_cblock.methods[0x0] = nrpck_drv_cblock_set_block;
    nrpck_drv_cblock.methods[0xF] = nrpck_drv_cblock_describe;
    nrpck_device_register_driver(&nrpck_drv_cblock);
}