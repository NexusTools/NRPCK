#include "nrpck_filesystem.h"
#include "nrpck_errors.h"

#include <string.h>

uchar nrpck_filesystems_length = 0;
NRPCKFileSystem *nrpck_filesystems[0x5];
void nrpck_filesystem_register(NRPCKFileSystem* filesystem) {
	if(nrpck_filesystems_length >= 0x5)
		return;
	
	nrpck_filesystems[nrpck_filesystems_length++] = filesystem;
}

schar nrpck_filesystem_detect(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* fs) {
	uchar i;
	for(i=0;i<nrpck_filesystems_length;i++)
		if(nrpck_filesystems[i]->detect(device, driver) > -1) {
			strcpy(fs, nrpck_filesystems[i]->name);
			return 0;
		}
	return ERROR_UNKNOWN_FS;
}

schar nrpck_filesystem_format(NRPCKDevice* device, NRPCKDeviceDriver* driver, char* fs) {
	uchar i;
	for(i=0;i<nrpck_filesystems_length;i++)
		if(strcmp(nrpck_filesystems[i]->name, fs) == 0)
			return nrpck_filesystems[i]->format(device, driver);
	return ERROR_UNKNOWN_FS;
}

NRPCKFileSystem* nrpck_filesystem_get(uchar id) {
	if(id < nrpck_filesystems_length)
		return nrpck_filesystems[id];
	return 0;
}

uchar nrpck_filesystem_count() {
	return nrpck_filesystems_length;
}