# はじめに
Linux BusyboxにTelnetで接続する方法を説明します。

# Busybox rootfs

```:rootfs
#!/bin/sh -x

TOP_BUSYBOX=~/busybox-1.26.2
cd $TOP_BUSYBOX
make defconfig
make -j4

rm -rf _install
make install
cd _install
mkdir proc sys dev etc etc/init.d
cat > "etc/init.d/rcS" <<- EOM
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
mkdir /dev/pts
mknod -m 666 /dev/ptmx c 5 2
mount -t devpts devpts /dev/pts
chown root.root /bin/busybox
chmod 4755 /bin/busybox
/sbin/mdev -s
chown root:tty /dev/ptmx
ifconfig eth0 10.0.0.2
/usr/sbin/telnetd -p 23
EOM
cat > "etc/passwd" <<- EOM
root::0:0:root:/root:/bin/sh
EOM
cat > "etc/group" <<- EOM
root:x:0:
tty:x:5:
EOM
cp ../examples/inittab etc
chmod +x etc/init.d/rcS
mkdir -pv lib/x86_64-linux-gnu
mkdir -pv lib64
cp -av /lib/x86_64-linux-gnu/lib[mc].so.6 lib/x86_64-linux-gnu
cp -av /lib/x86_64-linux-gnu/lib[mc]-2.19.so lib/x86_64-linux-gnu
cp -av /lib/x86_64-linux-gnu/ld-2.19.so lib/x86_64-linux-gnu
cp -av /lib64/ld-linux-x86-64.so.2 lib64
cp -av /lib/x86_64-linux-gnu/libpthread-2.19.so lib/x86_64-linux-gnu
cp -av /lib/x86_64-linux-gnu/libpthread.so.0 lib/x86_64-linux-gnu
cp /mnt/hgfs/share/binder_sample/test bin
find . | cpio -o --format=newc > ../rootfs.img
```

上記の内、telnet接続に必要となる部分は次の通りです。

```
cat > "etc/init.d/rcS" <<- EOM
mkdir /dev/pts
mknod -m 666 /dev/ptmx c 5 2
mount -t devpts devpts /dev/pts
chown root.root /bin/busybox
chmod 4755 /bin/busybox
/sbin/mdev -s
chown root:tty /dev/ptmx
/usr/sbin/telnetd -p 23
EOM

cat > "etc/passwd" <<- EOM
root::0:0:root:/root:/bin/sh
EOM

cat > "etc/group" <<- EOM
root:x:0:
tty:x:5:
EOM
```

# QEMU(Guest)
スクリプトを用意します。

```bash:x86.sh
#!/bin/sh

TOP_LINUX=~/linux-4.4.202
TOP_BUSYBOX=~/busybox-1.26.2
qemu-system-x86_64 -nographic \
  -kernel $TOP_LINUX/arch/x86/boot/bzImage \
  -initrd $TOP_BUSYBOX/rootfs.img \
  -append "root=/dev/ram rdinit=/sbin/init console=ttyS0" \
  -net nic -net tap,ifname=tap0 \
```

/etc/qemu-ifupの最後に次を追加します。

```:/etc/qemu-ifup 
sudo /sbin/ifconfig "$1" 10.0.0.1
```

スクリプトをsudoで実行します。QEMUが起動します。

```
sudo ./x86.sh
```

QEMUで実行されたLinuxのconsoleでtelnetdを起動します。

```
/usr/sbin/telnetd -F
```

# Host
Guestに接続します。

```
$ telnet 10.0.0.2
```

# References
- [1] https://git.busybox.net/busybox/plain/networking/telnetd.c