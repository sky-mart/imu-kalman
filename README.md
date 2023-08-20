# imu-kalman
Inertial Measurement Unit using Kalman filtering

## Build
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake -DCMSIS_DIR="/path/to/cmsis/include/". -B build/ && VERBOSE=1 make -C build/ app
```

## Flash
```
make -C build/ app-flash
```
