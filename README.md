# imu-kalman
Inertial Measurement Unit using Kalman filtering

## Build
```
make build
```

## Flash
```
make flash
```

## Debug
```
# openocd -f board/stm32f3discovery.cfg
make debug
```

## Tests
```
cmake . -B build/
make -C build/ testMathmart
./build/testMathmart
```
