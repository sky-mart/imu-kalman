cmake_minimum_required(VERSION 3.14)
project(imu-kalman CXX C ASM)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F303XC_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

include_directories(${CMSIS_DIR})

add_subdirectory(app)
add_subdirectory(mcu)

add_custom_target(erase-flash
	COMMAND st-flash erase
)
