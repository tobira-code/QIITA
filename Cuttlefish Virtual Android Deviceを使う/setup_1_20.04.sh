#!/bin/sh -x

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

