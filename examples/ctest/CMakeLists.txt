add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ctest.s PRE_LINK
  COMMAND ${CC65} -O -T -t none --cpu 65c02 -I ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${CMAKE_CURRENT_SOURCE_DIR}/ctest.c -o ${CMAKE_CURRENT_BINARY_DIR}/ctest.s --codesize 100
  COMMENT "Compile ctest.c")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ctest.o PRE_LINK
  COMMAND ${CA65} -t none --cpu 65c02 ${CMAKE_CURRENT_BINARY_DIR}/ctest.s -o ${CMAKE_CURRENT_BINARY_DIR}/ctest.o
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ctest.s
  COMMENT "Assemble ctest.o")
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ctest.img PRE_LINK
  COMMAND ${LD65} -m ${CMAKE_CURRENT_BINARY_DIR}/ctest.map -C ${CC65_DIR}/cfg/rpc8e.cfg  --obj ${CMAKE_CURRENT_BINARY_DIR}/ctest.o --obj ${CC65_DIR}/lib/runtime/lpush.o  --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_display.lib  --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_reboot.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_device.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck.lib --lib ${CC65_DIR}/lib/sprintf.lib --lib ${CC65_DIR}/lib/math.lib --lib ${CC65_DIR}/lib/errno.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_console.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_ccmonitor.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_matrix.lib --lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_cblock.lib --lib ${CC65_DIR}/lib/rpc8e.lib -o ctest.img
  COMMAND ${ALIGN} ${CMAKE_CURRENT_BINARY_DIR}/ctest.img ${CMAKE_CURRENT_BINARY_DIR}/ctest.map
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ctest.o ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_display.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_device.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_reboot.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_console.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_matrix.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_cblock.lib
  COMMENT "Link ctest.img"
  VERBATIM)
ADD_CUSTOM_TARGET(ctest_example DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ctest.img ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_display.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_device.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_reboot.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_console.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_matrix.lib ${CMAKE_CURRENT_BINARY_DIR}/../../src/nrpck_drv_cblock.lib)
ADD_DEPENDENCIES(ctest_example nrpck nrpck_display nrpck_device nrpck_reboot nrpck_drv_console nrpck_drv_ccmonitor nrpck_drv_matrix nrpck_drv_cblock)