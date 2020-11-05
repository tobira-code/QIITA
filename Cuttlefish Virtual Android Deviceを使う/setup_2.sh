#!/bin/sh -x

SOURCE=/mnt/hgfs/share/cuttlefish

cd cuttlefish
tar xvf ${SOURCE}/cvd-host_package.tar
unzip ${SOURCE}/aosp_cf_x86_phone-img-6945323.zip 
#unzip ${SOURCE}/aosp_cf_x86_tv-img-eng.user.zip

HOME=$PWD ./bin/launch_cvd

# ./bin/adb -e shell
