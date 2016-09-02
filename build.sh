#!/bin/sh
set -e

mkdir -p bin

echo "Compiling NRPCKernel...";
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude mod/core.c -o bin/nrpck.s --codesize 100

echo "Assembling NRPCKernel...";
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck.s -o bin/nrpck.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc mod/hault.s -o bin/nrpck_hault.o

echo "Compiling Modules..."
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude mod/device.c -o bin/device.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude mod/display.c -o bin/display.s --codesize 100
#../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude mod/drive.c -o bin/drive.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude mod/filesystem.c -o bin/filesystem.s --codesize 100
#../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude mod/socket.c -o bin/socket.s --codesize 100

echo "Assembling Modules..."
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/device.s -o bin/device.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/display.s -o bin/display.o
#../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/drive.s -o bin/drive.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/filesystem.s -o bin/filesystem.o
#../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/socket.s -o bin/socket.o

echo "Compiling Drivers..."
#../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude drv/modem.c -o bin/nrpck_drv_modem.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude drv/sortron.c -o bin/nrpck_drv_sortron.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude drv/ioexpander.c -o bin/nrpck_drv_ioexpander.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude drv/console.c -o bin/nrpck_drv_console.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude drv/matrix.c -o bin/nrpck_drv_matrix.s --codesize 100
../../bin/cc65 -O -T -t none --cpu 65c02 -I../../include -Iinclude drv/disk.c -o bin/nrpck_drv_disk.s --codesize 100

echo "Assembling Drivers...";
#../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck_drv_modem.s -o bin/nrpck_drv_modem.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck_drv_sortron.s -o bin/nrpck_drv_sortron.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck_drv_ioexpander.s -o bin/nrpck_drv_ioexpander.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck_drv_console.s -o bin/nrpck_drv_console.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck_drv_matrix.s -o bin/nrpck_drv_matrix.o
../../bin/ca65 -t none --cpu 65c02 -I../../lib/asminc bin/nrpck_drv_disk.s -o bin/nrpck_drv_disk.o

echo "Combining into Libs...";
../../bin/ar65 a bin/nrpck.lib bin/nrpck.o bin/nrpck_hault.o
../../bin/ar65 a bin/nrpck_device.lib bin/device.o
../../bin/ar65 a bin/nrpck_display.lib bin/display.o
#../../bin/ar65 a bin/nrpck_drive.lib bin/drive.o
../../bin/ar65 a bin/nrpck_filesystem.lib bin/filesystem.o
#../../bin/ar65 a bin/nrpck_socket.lib bin/socket.o
#../../bin/ar65 a bin/nrpck_drv_modem.lib bin/nrpck_drv_modem.o
../../bin/ar65 a bin/nrpck_drv_sortron.lib bin/nrpck_drv_sortron.o
../../bin/ar65 a bin/nrpck_drv_console.lib bin/nrpck_drv_console.o
../../bin/ar65 a bin/nrpck_drv_matrix.lib bin/nrpck_drv_matrix.o
../../bin/ar65 a bin/nrpck_drv_disk.lib bin/nrpck_drv_disk.o
