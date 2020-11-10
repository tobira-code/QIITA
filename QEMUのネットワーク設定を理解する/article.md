---
title: QEMU�̃l�b�g���[�N�ݒ�𗝉�����
tags: QEMU Network
author: tobira-code
slide: false
---
# �͂��߂�
QEMU�̃l�b�g���[�N�ݒ��������܂��B
Host��Geust�̂������ŒʐM������@���ɐ������܂��B

# ��
Host OS��Ubuntu14.04�ł��B
Guest OS��linux kernel + Busybox�ł��B

```:HostOS
$ uname -r
4.4.0-148-generic

$ cat /etc/lsb-release 
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=14.04
DISTRIB_CODENAME=trusty
DISTRIB_DESCRIPTION="Ubuntu 14.04.6 LTS"
```

```:GuestOS
/ # uname -a
Linux (none) 4.4.202 #1 SMP Sun Nov 8 13:13:39 JST 2020 x86_64 GNU/Linux
```

```:QEMU
$ qemu-system-x86_64 --version
QEMU emulator version 2.0.0 (Debian 2.0.0+dfsg-2ubuntu1.46), Copyright (c) 2003-2008 Fabrice Bellard
```

�l�b�g���[�N�ݒ��}�ɂ��܂����B�����Q�Ƃ��������B
![Untitled Diagram-Page.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/84205/91d350bc-f544-240b-2e0b-d59e0ef01902.png)

# �ݒ�
Host��/etc/qemu-ifup�Ɏ��̓��e��ǉ����܂��B

```
sudo /sbin/ifconfig "$1" 10.0.0.1
```

Host�Ŏ��̃R�}���h�����s���܂��BGuest���N�����܂��B
���̍s�Ńl�b�g���[�N��ݒ肵�܂��BGuest�N�����Host��tap0���F������܂��B

> -net nic -net tap,ifname=tap0

```
TOP_LINUX=~/linux-4.4.202
TOP_BUSYBOX=~/busybox-1.26.2
sudo qemu-system-x86_64 -nographic \
  -kernel $TOP_LINUX/arch/x86/boot/bzImage \
  -initrd $TOP_BUSYBOX/rootfs.img \
  -append "root=/dev/ram rdinit=/sbin/init console=ttyS0" \
  -net nic -net tap,ifname=tap0
```

Geust�̃R���\�[���Ŏ������s���܂��B

```
ifconfig eth0 10.0.0.2
```

# �m�F
Host�Ŏ��̃R�}���h�����s���܂��B

```
$ ping 10.0.0.2
PING 10.0.0.2 (10.0.0.2) 56(84) bytes of data.
64 bytes from 10.0.0.2: icmp_seq=1 ttl=64 time=1078 ms
64 bytes from 10.0.0.2: icmp_seq=2 ttl=64 time=78.8 ms
```

Guest�Ŏ��̃R�}���h�����s���܂��B

```
/ # ping 10.0.0.1
PING 10.0.0.1 (10.0.0.1): 56 data bytes
64 bytes from 10.0.0.1: seq=0 ttl=64 time=7.457 ms
64 bytes from 10.0.0.1: seq=1 ttl=64 time=2.998 ms
```

# References
- [1] https://wiki.qemu.org/Documentation/Networking
- [2] https://hackmd.io/@hnkz/qemu-config
- [3] http://www15.big.or.jp/~yamamori/sun/qemu/net.html
