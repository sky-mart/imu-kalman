set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN arm-none-eabi)

set(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc CACHE INTERNAL "C Compiler")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++ CACHE INTERNAL "C++ Compiler")
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}-gcc CACHE INTERNAL "ASM Compiler")
set(CMAKE_OBJCOPY ${TOOLCHAIN}-objcopy CACHE FILEPATH "The toolchain objcopy command " FORCE )
set(CMAKE_OBJDUMP ${TOOLCHAIN}-objdump CACHE FILEPATH "The toolchain objdump command " FORCE )

set(COMMON_FLAGS "-mthumb -mcpu=cortex-m4 -mabi=aapcs")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} -fno-builtin -Wall -ffunction-sections -fdata-sections -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG "-ggdb")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-exceptions -fno-rtti")

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs")
