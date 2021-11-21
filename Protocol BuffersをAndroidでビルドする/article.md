---
title: Protocol BuffersをAndroidでビルドする
tags: Android ProtocolBuffers
author: tobira-code
slide: false
---
# はじめに
Androidで実行できるProtocol Buffersをビルドする方法を説明します。
Host OSはUbuntu18.04です。

# Android NDKを取得する
次のコマンドでndk-r23bをダウンロードします。その後、展開します。
[1]を参照ください。

```bash:console
# download NDK
NDK_VER=r23b
if [ ! -e android-ndk-$NDK_VER-linux.zip ];then
  wget https://dl.google.com/android/repository/android-ndk-$NDK_VER-linux.zip
fi
if [ ! -e android-ndk-$NDK_VER ];then
  unzip android-ndk-$NDK_VER-linux.zip
fi
```

# Protocol Buffersを取得する
次のコマンドでProtocol Buffers v3.19.1をダウンロードします。その後、展開します。
[2]を参照ください。

```bash:console
# download protobuf
PROTOBUF_VER=3.18.1
if [ ! -e v$PROTOBUF_VER.tar.gz ];then
  wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v$PROTOBUF_VER.tar.gz
fi
if [ ! -e protobuf-$PROTOBUF_VER ];then
  tar xvf v$PROTOBUF_VER.tar.gz
fi
```

# Protocol Buffersをビルドする
[3]を参照して次のコマンドを実行します。

実行後に次のパスにUbuntuで実行可能なライブラリができます。
protobuf-3.18.1/install_host_dir

実行後に次のパスにandroidで実行可能なライブラリができます。
protobuf-3.18.1/install_android_dir

```bash:console
cd protobuf-$PROTOBUF_VER

# build protobuf for host
if [ ! -e install_host_dir ];then
  rm -rf install_host_dir && mkdir install_host_dir
  ./autogen.sh
  ./configure --prefix=`pwd`/install_host_dir --disable-silent-rules
  time make -j8 2>&1 | tee make2.log
  make install
  make clean
fi

# build protobuf for android
if [ ! -e install_android_dir ];then
  export TOOLCHAIN=`pwd`/../android-ndk-$NDK_VER/toolchains/llvm/prebuilt/linux-x86_64
#  export TARGET=armv7a-linux-androideabi
  export TARGET=i686-linux-android
  export API=30
  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$TARGET$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
  export CXXFLAGS=-U__ANDROID__
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip
  export SYSROOT=$TOOLCHAIN/sysroot
  
  rm -rf install_android_dir && mkdir install_android_dir
  ./autogen.sh
  make clean
  ./configure --prefix=`pwd`/install_android_dir --host $TARGET --with-sysroot=$SYSROOT --disable-silent-rules
  time make -j8 2>&1 | tee make.log
  make install
  make clean
fi
```

# Script
上記説明した内容を実行するScriptは次の通りです。

```bash:download_and_build_protobuf.sh
#!/bin/bash -x

# download NDK
NDK_VER=r23b
if [ ! -e android-ndk-$NDK_VER-linux.zip ];then
  wget https://dl.google.com/android/repository/android-ndk-$NDK_VER-linux.zip
fi
if [ ! -e android-ndk-$NDK_VER ];then
  unzip android-ndk-$NDK_VER-linux.zip
fi

# download protobuf
PROTOBUF_VER=3.18.1
if [ ! -e v$PROTOBUF_VER.tar.gz ];then
  wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v$PROTOBUF_VER.tar.gz
fi
if [ ! -e protobuf-$PROTOBUF_VER ];then
  tar xvf v$PROTOBUF_VER.tar.gz
fi

cd protobuf-$PROTOBUF_VER

# build protobuf for host
if [ ! -e install_host_dir ];then
  rm -rf install_host_dir && mkdir install_host_dir
  ./autogen.sh
  ./configure --prefix=`pwd`/install_host_dir --disable-silent-rules
  time make -j8 2>&1 | tee make2.log
  make install
  make clean
fi

# build protobuf for android
if [ ! -e install_android_dir ];then
  export TOOLCHAIN=`pwd`/../android-ndk-$NDK_VER/toolchains/llvm/prebuilt/linux-x86_64
#  export TARGET=armv7a-linux-androideabi
  export TARGET=i686-linux-android
  export API=30
  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$TARGET$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
  export CXXFLAGS=-U__ANDROID__
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip
  export SYSROOT=$TOOLCHAIN/sysroot
  
  rm -rf install_android_dir && mkdir install_android_dir
  ./autogen.sh
  make clean
  ./configure --prefix=`pwd`/install_android_dir --host $TARGET --with-sysroot=$SYSROOT --disable-silent-rules
  time make -j8 2>&1 | tee make.log
  make install
  make clean
fi
```

# Sample Code
Protocol Buffersを利用するプログラム例を示します。lite runtimeとしてcompileします。
[4]をベースにtimestampをコメントアウトした次の.protoファイルを使用します。
またPhoneNumber.numberをoptionalにします。

```proto:addressbook.proto
// See README.txt for information and build instructions.
//
// Note: START and END tags are used in comments to define sections used in
// tutorials.  They are not part of the syntax for Protocol Buffers.
//
// To get an in-depth walkthrough of this file and the related examples, see:
// https://developers.google.com/protocol-buffers/docs/tutorials

// [START declaration]
syntax = "proto3";
package tutorial;

//import "google/protobuf/timestamp.proto";
// [END declaration]

// [START java_declaration]
option java_multiple_files = true;
option java_package = "com.example.tutorial.protos";
option java_outer_classname = "AddressBookProtos";
// [END java_declaration]

// [START csharp_declaration]
option csharp_namespace = "Google.Protobuf.Examples.AddressBook";
// [END csharp_declaration]

// [START go_declaration]
option go_package = "../tutorial";
// [END go_declaration]

// [START messages]
message Person {
  string name = 1;
  int32 id = 2;  // Unique ID number for this person.
  string email = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    optional string number = 1;
    PhoneType type = 2;
  }

  repeated PhoneNumber phones = 4;

//  google.protobuf.Timestamp last_updated = 5;
}

// Our address book file is just one of these.
message AddressBook {
  repeated Person people = 1;
}
// [END messages]
```


```c++:main.cc
int main() { return 0; }
```

-static-libstdc++ を指定します。

```makefile:makefile_android_i686
CFLAGS=-I. -I../protobuf-3.18.1/install_android_dir/include -g -Wall -Werror
INCS=addressbook.pb.h
OBJS=main.o addressbook.pb.o
LIBS=../protobuf-3.18.1/install_android_dir/lib/libprotobuf-lite.a
TARGET=main

AR=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar
CC=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android30-clang
AS=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android30-clang
CXX=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android30-clang++
LD=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/ld
RANLIB=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ranlib
STRIP=../android-ndk-r23b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ranlib

%.o: %.cc $(INCS)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS) -static-libstdc++

clean:
	rm -rf $(TARGET) *.o
```

```bash:mk_android_i686.sh
#!/bin/bash -x

if [ $1 = "clean" ];then
  make -f makefile_android_i686 clean
  rm -rf addressbook.pb.cc  addressbook.pb.h 
else
  make -f makefile_android_i686 clean
  rm -rf addressbook.pb.cc  addressbook.pb.h 
  ../protobuf-3.18.1/install_host_dir/bin/protoc --proto_path=./ --cpp_out=lite:./ addressbook.proto
  make -f makefile_android_i686
fi
```

上記ファイルをprotobuf_sampleに置きます。
protobuf-3.18.1との位置関係は次の通りです。
protobuf_sampleに移動して./mk.sh を実行します。

```
├── protobuf-3.18.1
└── protobuf_sample
    ├── addressbook.proto
    ├── main.cc
    ├── makefile_android_i686
    └── mk_android_i686.sh
```

# References
- [1] https://developer.android.com/ndk/downloads
- [2] https://github.com/protocolbuffers/protobuf
- [3] https://developer.android.com/ndk/guides/other_build_systems
- [4] https://github.com/protocolbuffers/protobuf/blob/v3.18.1/examples/addressbook.proto
