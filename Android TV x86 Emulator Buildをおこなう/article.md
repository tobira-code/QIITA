---
title: Android TV x86 Emulator Buildをおこなう
tags: Android emulator aosp
author: tobira-code
slide: false
---
# はじめに
Android TV x86 Emulator Buildの方法を説明します。

# Version / Repo sync
android-11.0.0_r3 を使用します。取得方法は次の通りです。
$HOMEで実行します。

```
mkdir android-11.0.0_r3
cd android-11.0.0_r3
repo init -u https://android.googlesource.com/platform/manifest -b android-11.0.0_r3 --depth=1
repo sync -j`nproc`
```

# Custom
次のパッチを適用します。

```c:diff.patch
diff --git a/device/google/atv/permissions/tv_core_hardware.xml b/device/google/atv/permissions/tv_core_hardware.xml
index 3f01e14..b2ad7f6 100644
--- a/device/google/atv/permissions/tv_core_hardware.xml
+++ b/device/google/atv/permissions/tv_core_hardware.xml
@@ -37,5 +37,6 @@
     <feature name="android.software.autofill" />
 
     <feature name="android.software.cts" />
+    <feature name="android.hardware.tv.tuner" />
 
 </permissions>
diff --git a/device/google/atv/permissions/tv_sdk_excluded_core_hardware.xml b/device/google/atv/permissions/tv_sdk_excluded_core_hardware.xml
index f5b57fd..1d1a95f 100644
--- a/device/google/atv/permissions/tv_sdk_excluded_core_hardware.xml
+++ b/device/google/atv/permissions/tv_sdk_excluded_core_hardware.xml
@@ -41,4 +41,7 @@
         and to date it is not designed to be rotated.
     -->
     <unavailable-feature name="android.hardware.screen.portrait" />
+
+    <unavailable-feature name="android.hardware.bluetooth" />
+    <unavailable-feature name="android.hardware.bluetooth_le" />
 </permissions>
diff --git a/device/google/atv/products/AndroidProducts.mk b/device/google/atv/products/AndroidProducts.mk
index bd75dca..c0ceca4 100644
--- a/device/google/atv/products/AndroidProducts.mk
+++ b/device/google/atv/products/AndroidProducts.mk
@@ -37,3 +37,7 @@ PRODUCT_MAKEFILES := \
     $(LOCAL_DIR)/aosp_tv_x86.mk \
     $(LOCAL_DIR)/sdk_atv_armv7.mk \
     $(LOCAL_DIR)/sdk_atv_x86.mk
+
+COMMON_LUNCH_CHOICES := \
+    sdk_atv_x86-eng \
+
diff --git a/device/google/atv/products/sdk_atv_x86.mk b/device/google/atv/products/sdk_atv_x86.mk
index d6363f3..4c07c86 100644
--- a/device/google/atv/products/sdk_atv_x86.mk
+++ b/device/google/atv/products/sdk_atv_x86.mk
@@ -30,6 +30,12 @@ $(call inherit-product, development/build/product_sdk.mk)
 PRODUCT_PACKAGES += \
     EmulatorSmokeTests
 
+PRODUCT_COPY_FILES += \
+    device/google/atv/permissions/tv_core_hardware.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/tv_core_hardware.xml
+
+# Tuner HAL
+PRODUCT_PACKAGES += android.hardware.tv.tuner@1.0-service
+
 # Overrides
 PRODUCT_BRAND := Android
 PRODUCT_NAME := sdk_atv_x86
```

上記をdiff.patchの名前で保存します。
AOSP Top Directory で次のコマンドを実行してパッチを適用します。

```bash:
patch -p1 < diff.patch
```

# Lunch

```
source build/envsetup.sh
lunch sdk_atv_x86-eng
time make -j`nproc` 2>&1 | tee make_`date +%Y%m%d%H%M%S`.log
```

# Shrink out directory

```bash:shrink_out.sh
SRC=$HOME/android-11.0.0_r3
DST=shrink_out_android-11.0.0_r3

mkdir $DST
cd $DST
cp -r $SRC/prebuilts/android-emulator/linux-x86_64 .
mkdir -p out/target/product/generic_x86
mkdir -p out/target/product/generic_x86/system

cp $SRC/out/target/product/generic_x86/kernel-ranchu-64             out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/config.ini                   out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/system-qemu.img              out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/system_ext-qemu.img          out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/ramdisk-qemu.img             out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/userdata-qemu.img            out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/advancedFeatures.ini         out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/VerifiedBootParams.textproto out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/encryptionkey.img            out/target/product/generic_x86
cp $SRC/out/target/product/generic_x86/system/build.prop            out/target/product/generic_x86/system
cd ..

tar Jcvf ${DST}.tar.xz ${DST}
```

# Start emulator
Emulatorを開始します。Shrinkしたoutに移動して次のコマンドを実行します。

```bash:
export ANDROID_BUILD_TOP=$PWD
export ANDROID_PRODUCT_OUT=$PWD/out/target/product/generic_x86
export PATH=$PATH:$PWD/linux-x86_64

emulator -show-kernel -writable-system -selinux disabled
```

emulatorを終了する際には以下を実行します。

```
reboot -p
```
