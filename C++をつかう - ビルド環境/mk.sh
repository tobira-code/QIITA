#!/bin/bash -x

PRE_IFS=$IFS
IFS=$'\n'

if [ $# -eq 1 ] && [ $1 = "clean" ]; then
  rm -rf build
  exit 0
fi

GTYPE="MSYS Makefiles"
if [ $# -eq 1 ] && [ $1 = "u" ]; then
  GTYPE="Ninja"
fi

cmake \
  -H. \
  -G$GTYPE \
  -Bbuild \

IFS=$PRE_IFS
