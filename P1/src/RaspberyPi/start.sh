#!/bin/bash
path=$PWD
make
cd ../path_planning/build
cmake -D BUILD_INDIVIDUAL=ON .. && make -j4
cd $path
python3 ./i2clibraries/mpu6050.py
python setupIcp.py build_ext --inplace
