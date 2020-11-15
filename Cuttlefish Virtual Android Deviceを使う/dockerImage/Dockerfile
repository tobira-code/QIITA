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

