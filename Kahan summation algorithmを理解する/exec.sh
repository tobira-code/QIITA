#!/bin/bash

# ubuntu
#echo "disable kahan"
#./a.out 10 d q
#./a.out 100 d q
#./a.out 1000 d q
#./a.out 10000 d q
#./a.out 100000 d q
#./a.out 1000000 d q
#./a.out 10000000 d q
#
#echo "enable kahan"
#./a.out 10 e q
#./a.out 100 e q
#./a.out 1000 e q
#./a.out 10000 e q
#./a.out 100000 e q
#./a.out 1000000 e q
#./a.out 10000000 e q

# MINGW64
echo "disable kahan"
./a.exe 10 d q
./a.exe 100 d q
./a.exe 1000 d q
./a.exe 10000 d q
./a.exe 100000 d q
./a.exe 1000000 d q
./a.exe 10000000 d q

echo "enable kahan"
./a.exe 10 e q
./a.exe 100 e q
./a.exe 1000 e q
./a.exe 10000 e q
./a.exe 100000 e q
./a.exe 1000000 e q
./a.exe 10000000 e q
