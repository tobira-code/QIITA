---
title: Cuttlefish Virtual Android Deviceを使う
tags: Android cuttlefish
author: tobira-code
slide: false
---
# はじめに
Cuttlefish Virtual Android Device(CVD)の使い方を説明します。

# Version
Ubuntu20.04でCuttlefishを実行します。
Ubuntu20.04はHost Windows10 HomeのVMwareで実行します。

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
次にアクセスします。ビルド済みImageを取得します。
http://ci.android.com/

aosp_cf_x86_phone-userdebugを選択します。

次の2つのデータを取得します。6945323の部分は例です。
aosp_cf_x86_phone-img-6945323.zip
cvd-host_package.tar

次のコマンドで展開します。

```
tar xvf aosp_cf_x86_phone-img-6945323.zip
unzip cvd-host_package.tar
```

次のコマンドでCVDを実行します。

```
HOME=$PWD ./bin/launch_cvd
```

CVDにadb接続できます。
別のTerminalを起動して次のコマンドを実行します。

```
./bin/adb -e shell
```

次のコマンドでCVDを停止します。

```
HOME=$PWD ./bin/stop_cvd
```

# References
- [1] https://android.googlesource.com/device/google/cuttlefish
- [2] Kernel_Hacking_with_Cuttlefish.pdf
- [3] https://github.com/google/android-cuttlefish/
- [4] https://sites.google.com/junsun.net/how-to-run-cuttlefish/home
- [5] http://junsun.net/wordpress/tag/cuttlefish/
