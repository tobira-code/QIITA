#!/bin/sh

source build/envsetup.sh
lunch sdk_atv_x86-eng
time make -j8 2>&1 | tee make_`date +%Y%m%d%H%M%S`.log

