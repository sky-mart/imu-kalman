add_executable(app
  src/main.cc
)

target_include_directories(app
  PRIVATE
  include/
)

target_link_libraries(app
  mcu
  stm32f3xx_hal_driver
)

target_link_options(app PRIVATE -Wl,-Map=${PROJECT_NAME}.map)

set_target_properties(app PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_target(app.bin
	COMMAND ${CMAKE_OBJCOPY} -O binary app app.bin
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS app
)

add_custom_target(app-flash
	COMMAND st-flash write app.bin 0x8000000
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS app.bin
)

# openocd -f board/stm32f3discovery.cfg in another window
add_custom_target(app-debug
	COMMAND arm-none-eabi-gdb app -ex 'target remote localhost:3333' -ex 'monitor reset halt'
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS app
)
