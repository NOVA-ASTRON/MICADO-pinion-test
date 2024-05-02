#!/bin/bash


git clone https://github.com/jr-peterzon/mcc-libusb.git

cd mcc-libusb

mkdir BUILD
cd BUILD
cmake -DCMAKE_INSTALL_PREFIX=/opt/micado ..
make -j5 install
cd ../..
rm -Rf BUILD
mkdir BUILD

cd BUILD
qmake ..
make -j5 install


