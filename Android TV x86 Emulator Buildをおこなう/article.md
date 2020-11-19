---
title: Android TV x86 Emulator Buildをおこなう
tags: Android emulator aosp
author: tobira-code
slide: false
---
# はじめに
Android TV x86 Emulator Buildの方法を説明します。

# Version / Repo sync
android-10.0.0_r25 を使用します。取得方法は次の通りです。

```
mkdir android-10.0.0_r25
cd android-10.0.0_r25
repo init -u https://android.googlesource.com/platform/manifest -b android-10.0.0_r25 --depth=1
repo sync -j8
```

# Custom
次のパッチを適用します。

```
diff --git a/device/google/atv/permissions/privapp-permissions-atv-gsi.xml b/device/google/atv/permissions/privapp-permissions-atv-gsi.xml
index 92550a9..30a01ef 100644
--- a/device/google/atv/permissions/privapp-permissions-atv-gsi.xml
+++ b/device/google/atv/permissions/privapp-permissions-atv-gsi.xml
@@ -25,6 +25,7 @@
         <permission name="android.permission.USE_RESERVED_DISK"/>
         <permission name="android.permission.WRITE_MEDIA_STORAGE"/>
         <permission name="android.permission.WRITE_SECURE_SETTINGS"/>
+        <permission name="android.permission.MANAGE_DEBUGGING"/>
     </privapp-permissions>
 
     <privapp-permissions package="com.google.android.tv">
diff --git a/device/google/atv/products/AndroidProducts.mk b/device/google/atv/products/AndroidProducts.mk
index 8c1ed22..3268100 100644
--- a/device/google/atv/products/AndroidProducts.mk
+++ b/device/google/atv/products/AndroidProducts.mk
@@ -34,3 +34,9 @@
 PRODUCT_MAKEFILES := \
     $(LOCAL_DIR)/aosp_atv_arm_ab.mk \
     $(LOCAL_DIR)/aosp_atv_arm64_ab.mk
+
+PRODUCT_MAKEFILES := \
+    $(LOCAL_DIR)/sdk_atv_x86.mk \
+
+COMMON_LUNCH_CHOICES := \
+    sdk_atv_x86-eng \
diff --git a/device/google/atv/products/sdk_atv_x86.mk b/device/google/atv/products/sdk_atv_x86.mk
index b897416..8d58a9d 100644
--- a/device/google/atv/products/sdk_atv_x86.mk
+++ b/device/google/atv/products/sdk_atv_x86.mk
@@ -24,6 +24,17 @@ PRODUCT_COPY_FILES += \
     device/generic/goldfish/data/etc/encryptionkey.img:encryptionkey.img \
     prebuilts/qemu-kernel/x86_64/4.14/kernel-qemu2:kernel-ranchu-64
 
+# privapp-permissions whitelisting
+PRODUCT_COPY_FILES += \
+    device/google/atv/permissions/privapp-permissions-atv-gsi.xml:system/etc/permissions/privapp-permissions-atv-gsi.xml
+
+# Packages required for ATV GSI
+PRODUCT_PACKAGES += \
+    FrameworkPackageStubs \
+    LatinIMEGoogleTvPrebuilt \
+    TvProvision \
+    TvSampleLeanbackLauncher
+
 # TODO: separate out a common base for arm/x86 atv SDK build.
 $(call inherit-product, device/google/atv/products/sdk_atv_armv7.mk)
```

# Lunch

```
source build/envsetup.sh
lunch sdk_atv_x86-eng
time make -j8 2>&1 | tee make_`date +%Y%m%d%H%M%S`.log
```
