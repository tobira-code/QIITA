---
title: Cuttlefish Virtual Android Device���g��
tags: Android cuttlefish
author: tobira-code
slide: false
---
# �͂��߂�
Cuttlefish Virtual Android Device(CVD)�̎g������������܂��B

# Version
Ubuntu20.04��Cuttlefish�����s���܂��B
Ubuntu20.04��Host Windows10 Home��VMware�Ŏ��s���܂��B

CPU

```
Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz
```

Host OS

```
Windows 10 Home, 64-bit  (Build 19042.572) 10.0.19042
```

VMware

```
VMware(R) Workstation 15 Player 15.5.6 build-16341506
```

Guest OS

```
ubuntu-20.04.1-desktop-amd64
```

# Build & Install cuttlefish-common package

```
mkdir cuttlefish && cd cuttlefish

sudo apt-get update
sudo apt-get install -y git-core gnupg flex bison build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z1-dev libgl1-mesa-dev libxml2-utils xsltproc unzip fontconfig
sudo apt-get install -y config-package-dev debhelper-compat
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

# Run Image
���ɃA�N�Z�X���܂��B�r���h�ς�Image���擾���܂��B
http://ci.android.com/

aosp_cf_x86_phone-userdebug��I�����܂��B

����2�̃f�[�^���擾���܂��B6945323�̕����͗�ł��B
aosp_cf_x86_phone-img-6945323.zip
cvd-host_package.tar

���̃R�}���h�œW�J���܂��B

```
tar xvf aosp_cf_x86_phone-img-6945323.zip
unzip cvd-host_package.tar
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
