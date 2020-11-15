---
title: Cuttlefish Virtual Android Deviceを使う
tags: Android cuttlefish
author: tobira-code
slide: false
---
# はじめに
Cuttlefish Virtual Android Device(CVD)の使い方を説明します。

次の環境でCVDを作成します。

- Ubuntu 18.04
- Ubuntu 18.04 + Docker(Ubuntu 18.04)

それぞれの環境でCVDを使う方法を説明します。
CPU は Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz を使いました。

# Ubuntu 18.04
Ubuntu 18.04でCVDを使用します。Versionは以下の通りです。[E1]とします。

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
次の手順でcuttlefish-common packageをBuild & Installします。

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

次の手順は"Run Image"を参照してください。

# Ubuntu 18.04 + Docker(Ubuntu 18.04)
Host Ubuntu 18.04にDockerをInstallします。
DockerにUbuntu 18.04のImageを作成します。DockerImage上でCVDを使用します。
Host Ubuntu 18.04は[E1]です。Docker Versionは以下の通りです。

```
$ sudo apt-get install -y docker docker.io
$ docker --version
Docker version 19.03.6, build 369ce74a3c
```

Hostに"user"という名前のユーザを作成します。
以降は"user"で実行します。

/etc/modules を編集します。次を追加します。追加後再起動します。

```
vhci-hcd
vhost_net
vhost_vsock
```

## Build & Install cuttlefish-common package
次のDockerfileを作成します。

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

次のコマンドでImageを作成します。

```bash:console
sudo docker build -t ubuntu:18.04_cuttlefish .
```

次のコマンドでContainerを実行します。
Hostの/home/userとGuestの/home/userを共有します。

```
sudo docker run --name=cvd_container -u="user" --privileged -it -v /home/$USER:/home/$USER ubuntu:18.04_cuttlefish
```

run後に以下を実行します。

```
sudo /etc/init.d/rsyslog start
sudo /etc/init.d/cuttlefish-common start
```

同一Containerに対して別のTerminalを起動する方法は次の通りです。

```
sudo docker exec -it cvd_container /bin/bash
```

次の手順は"Run Image"を参照してください。

# Run Image
次にアクセスします。ビルド済みImageを取得します。
http://ci.android.com/

aosp_cf_x86_phone-userdebugを選択します。

次の2つのデータを取得します。6945323の部分は例です。
aosp_cf_x86_phone-img-6945323.zip
cvd-host_package.tar

次のコマンドで展開します。

```
unzip aosp_cf_x86_phone-img-6945323.zip
tar xvf cvd-host_package.tar
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

Dockerの場合は次の手順でCVDにadb接続します。

```
./bin/adb connect 127.0.0.1:6520
./bin/adb shell
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
