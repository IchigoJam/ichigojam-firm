#!/bin/sh
USER=$(whoami)
OBJ=obj

set -e

#DEBUG=-ggdb
DEBUG=

/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/gcc-arm-none-eabi/9.2.1/macosx/bin/arm-none-eabi-g++ -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/include/libcxx -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/include -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/arch -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/arch/chip -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/sdk/include -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/sdk/modules/include -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/sdk/apps/include -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/sdk/system/include -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/sdk/externals/include -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/sdk/externals/include/cmsis -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/variants/spresense -I/Users/fukuno/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/libraries/SDHCI/src -I/Users/fukuno/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/libraries/Storage/src -I/Users/fukuno/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/libraries/File/src -I/Users/fukuno/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/libraries/Audio -I/Users/fukuno/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/libraries/MemoryUtil -DCONFIG_WCHAR_BUILTIN -DCONFIG_HAVE_DOUBLE -D__NuttX__ -c -w -MMD -std=gnu++11 -c -fno-builtin -mabi=aapcs -Wall -Os -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -pipe -ffunction-sections -fno-exceptions -fno-rtti $DEBUG -gdwarf-3 -ffunction-sections -fdata-sections -fpermissive -DF_CPU=156000000L -DARDUINO=10819 -DARDUINO_spresense_ast -DARDUINO_ARCH_SPRESENSE "-DARDUINO_BOARD=\"spresense_ast\"" -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/cores/spresense -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/variants/spresense -I/Users/$USER/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/libraries/SPI -I/Users/$USER/Documents/Arduino/libraries/Adafruit-GFX-Library-spresense -I/Users/$USER/Documents/Arduino/libraries/Adafruit_ILI9341-spresense  -I../IchigoJam_BASIC/ -I./src/ ../IchigoJam_BASIC/main.c -o main.o

/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/gcc-arm-none-eabi/9.2.1/macosx/bin/arm-none-eabi-g++ $DEBUG -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Xlinker --entry=__start -nodefaultlibs -nostartfiles -Wl,--defsym,__reserved_ramsize=1572864-786432 -T/Users/$USER/Library/Arduino15/packages/SPRESENSE/hardware/spresense/3.0.1/variants/spresense/ramconfig.ld -Wl,--gc-sections -Xlinker -Map=$OBJ/output.map -o main.elf -Wl,--start-group main.o $OBJ/SPI.cpp.o $OBJ/glcdfont.c.o $OBJ/Adafruit_GFX.cpp.o $OBJ/Adafruit_SPITFT.cpp.o $OBJ/Adafruit_ILI9341.cpp.o \
$OBJ/SDHCI.cpp.o \
$OBJ/Storage.cpp.o \
$OBJ/File.cpp.o \
$OBJ/FrontEnd.cpp.o \
$OBJ/Audio.cpp.o \
$OBJ/MediaPlayer.cpp.o \
$OBJ/OutputMixer.cpp.o \
$OBJ/MediaRecorder.cpp.o \
$OBJ/MemoryUtil.cpp.o \
-u spresense_main $OBJ/core.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libapps.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libarch.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libarm_cortexM4lf_math.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libaudio.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libbinfmt.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libboard.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libboards.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libc.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libcmsis_nn.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libdrivers.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libfs.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libmm.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libnet.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libnnablart.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libsched.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libsslutils.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libxx.a /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/3.0.1/spresense/release/nuttx/libs/libmossfw.a -lgcc -lm -lsupc++_nano -Wl,--end-group -L$OBJ

/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-tools/3.0.1/mkspk/macosx/mkspk -c 2 main.elf nuttx ichigojam-spr.spk

# info
/Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/spresense-tools/3.0.1/getspkinfo/macosx/getspkinfo ichigojam-spr.spk
# info size check
# /Users/$USER/Library/Arduino15/packages/SPRESENSE/tools/gcc-arm-none-eabi/9.2.1/macosx/bin/arm-none-eabi-size -A main.elf

# write
# sh write.sh
