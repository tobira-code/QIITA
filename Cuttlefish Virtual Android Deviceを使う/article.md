---
title: Cuttlefish Virtual Android Device���g��
tags: Android cuttlefish
author: tobira-code
slide: false
---
# �͂��߂�
Cuttlefish Virtual Android Device(CVD)�̎g������������܂��B

���̊���CVD���쐬���܂��B

- Ubuntu 18.04
- Ubuntu 18.04 + Docker(Ubuntu 18.04)

���ꂼ��̊���CVD���g�����@��������܂��B
CPU �� Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz ���g���܂����B

# Ubuntu 18.04
Ubuntu 18.04��CVD���g�p���܂��BVersion�͈ȉ��̒ʂ�ł��B[E1]�Ƃ��܂��B

```
$ cat /etc/lsb-release 
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=18.04
DISTRIB_CODENAME=bionic
DISTRIB_DESCRIPTION="Ubuntu 18.04.5 LTS"
$ uname -r
5.4.0-53-generic
```

## Build & Install cuttlefish-common package
���̎菇��cuttlefish-common package��Build & Install���܂��B

```
mkdir cuttlefish && cd cuttlefish

sudo apt-get update
sudo apt-get install -y git-core gnupg flex bison build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z1-dev libgl1-mesa-dev libxml2-utils xsltproc unzip fontconfig
sudo apt-get install -y config-package-dev debhelper-compat
sudo apt-get install init-system-helpers=1.56+nmu1~ubuntu18.04.1
sudo apt-get install -y qemu-kvm
git clone https://github.com/google/android-cuttlefish
cd android-cuttlefish
git checkout 8b3b2686a49b52ce6a67868684b8d1c0eda7583c
dpkg-buildpackage --no-sign
sudo dpkg -i ../cuttlefish-common_*_amd64.deb
sudo apt --fix-broken install -y
sudo dpkg -i ../cuttlefish-common_*_amd64.deb
sudo usermod -aG kvm $USER
sudo usermod -aG cvdnetwork $USER

sudo reboot
```

���̎菇��"Run Image"���Q�Ƃ��Ă��������B

# Ubuntu 18.04 + Docker(Ubuntu 18.04)
Host Ubuntu 18.04��Docker��Install���܂��B
Docker��Ubuntu 18.04��Image���쐬���܂��BDockerImage���CVD���g�p���܂��B
Host Ubuntu 18.04��[E1]�ł��BDocker Version�͈ȉ��̒ʂ�ł��B

```
$ sudo apt-get install -y docker docker.io
$ docker --version
Docker version 19.03.6, build 369ce74a3c
```

Host��"user"�Ƃ������O�̃��[�U���쐬���܂��B
�ȍ~��"user"�Ŏ��s���܂��B

/etc/modules ��ҏW���܂��B����ǉ����܂��B�ǉ���ċN�����܂��B

```
vhci-hcd
vhost_net
vhost_vsock
```

## Build & Install cuttlefish-common package
����Dockerfile���쐬���܂��B

```:Dockerfile
FROM ubuntu:18.04
RUN apt-get update && apt-get install -y \
  git-core \
  gnupg \
  zip \
  curl \
  unzip \
  rsyslog \
  iproute2 \
  kmod \
  lsof \
  sudo \
  iputils-ping \
  qemu-kvm \
  config-package-dev \
  debhelper-compat \
  init-system-helpers=1.56+nmu1~ubuntu18.04.1 \
  debhelper=12.1.1ubuntu1~ubuntu18.04.1 \
  && adduser --disabled-password --gecos "" user && echo "user:user" | chpasswd \
  && usermod -G sudo user \
  && cd /home/user \
  && git clone https://github.com/google/android-cuttlefish \
  && cd android-cuttlefish \
  && git checkout 8b3b2686a49b52ce6a67868684b8d1c0eda7583c \
  && dpkg-buildpackage --no-sign \
  && dpkg -i ../cuttlefish-common_*_amd64.deb \
  ; apt --fix-broken install -y \
  && dpkg -i ../cuttlefish-common_*_amd64.deb \
  && usermod -aG kvm user \
  && usermod -aG cvdnetwork user \
  && cd .. \
  && rm -rf android-cuttlefish cuttlefish-* 
```

���̃R�}���h��Image���쐬���܂��B

```bash:console
sudo docker build -t ubuntu:18.04_cuttlefish .
```

���̃R�}���h��Container�����s���܂��B
Host��/home/user��Guest��/home/user�����L���܂��B

```
sudo docker run --name=cvd_container -u="user" --privileged -it -v /home/$USER:/home/$USER ubuntu:18.04_cuttlefish
```

run��Ɉȉ������s���܂��B

```
sudo /etc/init.d/rsyslog start
sudo /etc/init.d/cuttlefish-common start
```

����Container�ɑ΂��ĕʂ�Terminal���N��������@�͎��̒ʂ�ł��B

```
sudo docker exec -it cvd_container /bin/bash
```

���̎菇��"Run Image"���Q�Ƃ��Ă��������B

# Run Image
���ɃA�N�Z�X���܂��B�r���h�ς�Image���擾���܂��B
http://ci.android.com/

aosp_cf_x86_phone-userdebug��I�����܂��B

����2�̃f�[�^���擾���܂��B6945323�̕����͗�ł��B
aosp_cf_x86_phone-img-6945323.zip
cvd-host_package.tar

���̃R�}���h�œW�J���܂��B

```
unzip aosp_cf_x86_phone-img-6945323.zip
tar xvf cvd-host_package.tar
```

���̃R�}���h��CVD�����s���܂��B

```
HOME=$PWD ./bin/launch_cvd
```

CVD��adb�ڑ��ł��܂��B
�ʂ�Terminal���N�����Ď��̃R�}���h�����s���܂��B

```
./bin/adb -e shell
```

Docker�̏ꍇ�͎��̎菇��CVD��adb�ڑ����܂��B

```
./bin/adb connect 127.0.0.1:6520
./bin/adb shell
```

���̃R�}���h��CVD���~���܂��B

```
HOME=$PWD ./bin/stop_cvd
```

# References
- [1] https://android.googlesource.com/device/google/cuttlefish
- [2] Kernel_Hacking_with_Cuttlefish.pdf
- [3] https://github.com/google/android-cuttlefish/
- [4] https://sites.google.com/junsun.net/how-to-run-cuttlefish/home
- [5] http://junsun.net/wordpress/tag/cuttlefish/
