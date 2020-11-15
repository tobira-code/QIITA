#!/bin/sh -x

sudo docker build -t ubuntu:18.04_cuttlefish .
sudo docker run --name=cvd_container -u="user" --privileged -it -v /home/$USER:/home/$USER ubuntu:18.04_cuttlefish

#sudo /etc/init.d/rsyslog start
#sudo /etc/init.d/cuttlefish-common start
#
#mkdir cuttlefish && cd cuttlefish
#tar xvf ../cvd-host_package.tar
#unzip ../aosp_cf_x86_phone-img-6945323.zip
#
## console A
#HOME=$PWD ./bin/launch_cvd
#
## console B
## sudo docker exec -it cvd_container /bin/bash
#cd home/user/DockerGuest/cuttlefish
#./bin/adb connect 127.0.0.1:6520
#./bin/adb shell

