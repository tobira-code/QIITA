---
title: LiveTVをEmulatorで動作させる
tags: Android AndroidTV livetv emulator
author: tobira-code
slide: false
---
# はじめに
LiveTVをEmulatorで動作させる手順を説明します。

# 手順

1. Android Q Emulator を準備します。
2. LiveTVをAndroid 10.0.0_r25でビルドします。
3. ビルドしたapk と Emulatorのapk を差し替えます。

# Android Q Emulator を準備する
Android Studioを使って次のAVDを動作させます。詳細は省略します。
OSはWindows10を使用しました。

```
Name: Android_TV_720p_API_Q
CPU/ABI: Android TV Intel Atom (x86)
```

Graphics設定を"Software - GLES 2.0"に設定します。設定しないと画面表示されません。

# LiveTVをandroid-10.0.0_r25でビルドする

android-10.0.0_r25を取得してビルドします。Ubuntu14.04でビルドします。
詳細は省略します。

[1]からmasterをcloneして package/apps/TVに置き換えます。
commit f6ef153abc8b35796ba50e4eb78ec35c84ad678f を使用しました。

```
cd packages/apps
rm -rf TV
git clone https://android.googlesource.com/platform/packages/apps/TV
cd TV
git checkout f6ef153abc8b35796ba50e4eb78ec35c84ad678f
rm -rf tests tuner/tests common/tests
```

testsはビルドエラーになるため削除しました。
以下コメントアウトします。

```
diff --git a/jni/Android.bp b/jni/Android.bp
index c409a1e..9653efe 100644
--- a/jni/Android.bp
+++ b/jni/Android.bp
@@ -26,6 +26,6 @@ cc_library_shared {
     ],
     sdk_version: "23",
     stl: "c++_static",
-    header_libs: ["jni_headers"],
+//    header_libs: ["jni_headers"],
     shared_libs: ["liblog"],
 }
diff --git a/jni/minijail/Android.mk b/jni/minijail/Android.mk
index 1096089..4c3ba5d 100644
--- a/jni/minijail/Android.mk
+++ b/jni/minijail/Android.mk
@@ -23,7 +23,7 @@ LOCAL_MODULE := libminijail_jni
 LOCAL_SRC_FILES := minijail.cpp
 LOCAL_CFLAGS := -Wall -Werror
 LOCAL_CXX_STL := none
-LOCAL_HEADER_LIBRARIES := jni_headers
+#LOCAL_HEADER_LIBRARIES := jni_headers
 LOCAL_STATIC_LIBRARIES := libc++_static libminijail
 LOCAL_LDLIBS := -llog
```

ビルドします。次のパスにapkができます。
out/target/product/generic_x86/system/product/priv-app/LiveTv/LiveTv.apk

# ビルドしたapk とEmulatorのapk を差し替える

remountできる状態でbootします。\(\*1\)

```
emulator -avd Android_TV_720p_API_Q -writable-system
```

次のコマンドでLiveTv.apkを差し替えて再起動します。

```
adb root
adb remount
adb shell "find data system | grep LiveTv | xargs rm -rf"
adb shell "mkdir /system/priv-app/LiveTv"
adb push LiveTv.apk /system/priv-app/LiveTv
adb reboot
```

[Settings] - [Apps] - [See all apps] - [LiveTv]を選択して
Version が 1.24-aosp になっていれば差し替えできています。

差し替え後のLiveTv.apkを使用する場合、\(\*1\)で起動する必要があります。

# References
- [1] https://android.googlesource.com/platform/packages/apps/TV/
