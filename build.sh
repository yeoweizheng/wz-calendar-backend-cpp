#!/bin/sh
cd libs/Crow
mkdir -p build
cd build
cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF
make install

cd ../../libbcrypt
mkdir -p build
cd build
cmake ..
make
make install
ldconfig

cd ../../../
mkdir -p dist
cd dist
rm -f wzcalendar
cmake ..
make
