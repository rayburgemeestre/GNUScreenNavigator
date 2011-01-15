#!/bin/bash
cd lib
tar -zxvf jsoncpp-src-0.5.0.tar.gz
cd jsoncpp-src-0.5.0
tar -zxvf ../scons-local-1.0.0.tar.gz
python scons.py platform=linux-gcc
cd libs/linux-gcc-*
mv libjson_linux-gcc*libmt.a ../libjson.a
mv libjson_linux-gcc*libmt.so ../libjson.so

echo "***********************************************************************************************"
echo "Please copy 'lib/jsoncpp-src-0.5.0/libs/libjson.so' to your lib directory (probably '/usr/lib')"
echo "Then hit 'make' to compile'"
echo "***********************************************************************************************"
