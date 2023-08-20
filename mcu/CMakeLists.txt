add_library(mcu
  src/system_stm32f3xx.c
  src/startup_stm32f303xc.S
)

target_include_directories(mcu
  PUBLIC
  include/
)
