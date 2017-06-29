add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/telnet.s PRE_LINK
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/telnet.c -o ${CMAKE_CURRENT_BINARY_DIR}/telnet.s --codesize 100
  COMMENT "Compile telnet.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/telnet.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/telnet.s -o ${CMAKE_CURRENT_BINARY_DIR}/telnet.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/telnet.s
  COMMENT "Assemble telnet.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/telnet.img PRE_LINK
  COMMAND ${LD65} -m ${CMAKE_CURRENT_BINARY_DIR}/telnet.map -C ${CC65_DIR}/cfg/rpc8e.cfg  --obj ${CMAKE_CURRENT_BINARY_DIR}/telnet.o  --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_display.lib  --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_reboot.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_device.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck.lib --lib ${CC65_DIR}/lib/sprintf.lib --lib ${CC65_DIR}/lib/math.lib --lib ${CC65_DIR}/lib/errno.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_console.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_ccmonitor.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_matrix.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_modem.lib --lib ${CC65_DIR}/lib/rpc8e.lib -o telnet.img
  COMMAND ${ALIGN} ${CMAKE_CURRENT_BINARY_DIR}/telnet.img ${CMAKE_CURRENT_BINARY_DIR}/telnet.map
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/telnet.o ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_display.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_device.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_reboot.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_console.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_matrix.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_modem.lib
  COMMENT "Link telnet.img"
  VERBATIM)
ADD_CUSTOM_TARGET(telnet_example DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/telnet.img ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_display.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_device.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_reboot.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_console.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_matrix.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_modem.lib)
ADD_DEPENDENCIES(telnet_example nrpck nrpck_display nrpck_device nrpck_reboot nrpck_drv_console nrpck_drv_ccmonitor nrpck_drv_matrix nrpck_drv_modem)