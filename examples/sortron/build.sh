#!/bin/sh
set -e

cd ../../../lib/errno
./build.sh
cd ../math
./build.sh
cd ../sprintf
./build.sh
cd ../../nrpckernel
./build.sh
cd examples/sortron

echo "Compiling Test...";
../../../../bin/cc65 -O -t none --cpu 65c02 -I../../../../include -I../../include sortron.c -o sortron.s --codesize 100
../../../../bin/ca65 -t none --cpu 65c02 -I../../../../lib/asminc sortron.s -o sortron.o
../../../../bin/ld65 -C ../../../../lib/rpc8e.cfg  --obj sortron.o --lib ../../bin/nrpck_display.lib --lib ../../bin/nrpck_device.lib --lib ../../bin/nrpck.lib --lib ../../../lib/sprintf/bin/sprintf.lib --lib ../../../lib/math/bin/math.lib --lib ../../../lib/errno/bin/errno.lib --lib ../../bin/nrpck_drv_console.lib --lib ../../bin/nrpck_drv_matrix.lib --lib ../../bin/nrpck_drv_sortron.lib --lib ../../../../lib/rpc8e.lib -o sortron.img

echo "Aligning Test...";
../../../../bin/align sortron.img
