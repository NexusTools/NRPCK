add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/core.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/mod
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod/core.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/mod/core.c -o ${CMAKE_CURRENT_BINARY_DIR}/mod/core.s --codesize 100
  COMMENT "Compile core.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/core.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/mod/core.s -o ${CMAKE_CURRENT_BINARY_DIR}/mod/core.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/core.s
  COMMENT "Assemble core.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/asm.o PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/mod
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_SOURCE_DIR}/mod/asm.s -o ${CMAKE_CURRENT_BINARY_DIR}/mod/asm.o
  COMMENT "Assemble asm.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck.lib ${CMAKE_CURRENT_BINARY_DIR}/mod/core.o ${CMAKE_CURRENT_BINARY_DIR}/mod/asm.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/core.o ${CMAKE_CURRENT_BINARY_DIR}/mod/asm.o ${CC65_DIR}/lib/rpc8e/crt0.o
  COMMENT "Link nrpck.lib")
ADD_CUSTOM_TARGET(nrpck ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/device.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/mod
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod/device.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/mod/device.c -o ${CMAKE_CURRENT_BINARY_DIR}/mod/device.s --codesize 100
  COMMENT "Compile device.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/device.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/mod/device.s -o ${CMAKE_CURRENT_BINARY_DIR}/mod/device.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/device.s
  COMMENT "Assemble device.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_device.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_device.lib ${CMAKE_CURRENT_BINARY_DIR}/mod/device.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/device.o
  COMMENT "Link nrpck_device.lib")
ADD_CUSTOM_TARGET(nrpck_device ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_device.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/display.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/mod
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod/display.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/mod/display.c -o ${CMAKE_CURRENT_BINARY_DIR}/mod/display.s --codesize 100
  COMMENT "Compile display.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/display.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/mod/display.s -o ${CMAKE_CURRENT_BINARY_DIR}/mod/display.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/display.s
  COMMENT "Assemble display.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_display.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_display.lib ${CMAKE_CURRENT_BINARY_DIR}/mod/display.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/display.o
  COMMENT "Link nrpck_display.lib")
ADD_CUSTOM_TARGET(nrpck_display ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_display.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/mod
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod/filesystem.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/mod/filesystem.c -o ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.s --codesize 100
  COMMENT "Compile filesystem.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.s -o ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.s
  COMMENT "Assemble filesystem.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_filesystem.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_filesystem.lib ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/filesystem.o
  COMMENT "Link nrpck_filesystem.lib")
ADD_CUSTOM_TARGET(nrpck_filesystem ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_filesystem.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/mod
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod/reboot.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/mod/reboot.c -o ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.s --codesize 100
  COMMENT "Compile reboot.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.s -o ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.s
  COMMENT "Assemble reboot.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_reboot.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_reboot.lib ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mod/reboot.o
  COMMENT "Link nrpck_reboot.lib")
ADD_CUSTOM_TARGET(nrpck_reboot ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_reboot.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/modem.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/modem.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.s --codesize 100
  COMMENT "Compile modem.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.s
  COMMENT "Assemble modem.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_modem.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_modem.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/modem.o
  COMMENT "Link nrpck_drv_modem.lib")
ADD_CUSTOM_TARGET(nrpck_drv_modem ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_modem.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/sortron.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/sortron.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.s --codesize 100
  COMMENT "Compile sortron.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.s
  COMMENT "Assemble sortron.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_sortron.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_sortron.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/sortron.o
  COMMENT "Link nrpck_drv_sortron.lib")
ADD_CUSTOM_TARGET(nrpck_drv_sortron ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_sortron.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/console.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/console.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/console.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/console.s --codesize 100
  COMMENT "Compile console.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/console.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/console.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/console.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/console.s
  COMMENT "Assemble console.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_console.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_console.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/console.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/console.o
  COMMENT "Link nrpck_drv_console.lib")
ADD_CUSTOM_TARGET(nrpck_drv_console ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_console.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/ccmonitor.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/ccmonitor.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.s --codesize 100
  COMMENT "Compile ccmonitor.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.s
  COMMENT "Assemble ccmonitor.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_ccmonitor.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_ccmonitor.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/ccmonitor.o
  COMMENT "Link nrpck_drv_ccmonitor.lib")
ADD_CUSTOM_TARGET(nrpck_drv_ccmonitor ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_ccmonitor.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/matrix.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/matrix.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.s --codesize 100
  COMMENT "Compile matrix.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.s
  COMMENT "Assemble matrix.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_matrix.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_matrix.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/matrix.o
  COMMENT "Link nrpck_drv_matrix.lib")
ADD_CUSTOM_TARGET(nrpck_drv_matrix ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_matrix.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/ioexpander.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/ioexpander.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.s --codesize 100
  COMMENT "Compile ioexpander.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.s
  COMMENT "Assemble ioexpander.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_ioexpander.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_ioexpander.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/ioexpander.o
  COMMENT "Link nrpck_drv_ioexpander.lib")
ADD_CUSTOM_TARGET(nrpck_drv_ioexpander ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_ioexpander.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/disk.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/disk.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.s --codesize 100
  COMMENT "Compile disk.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.s
  COMMENT "Assemble disk.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_disk.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_disk.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/disk.o
  COMMENT "Link nrpck_drv_disk.lib")
ADD_CUSTOM_TARGET(nrpck_drv_disk ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_disk.lib)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.s PRE_LINK
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/drv
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/drv/cblock.c
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/drv/cblock.c -o ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.s --codesize 100
  COMMENT "Compile cblock.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.s -o ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.s
  COMMENT "Assemble cblock.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_cblock.lib PRE_LINK
  COMMAND ${AR65} a ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_cblock.lib ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/drv/cblock.o
  COMMENT "Link nrpck_drv_cblock.lib")
ADD_CUSTOM_TARGET(nrpck_drv_cblock ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nrpck_drv_cblock.lib)