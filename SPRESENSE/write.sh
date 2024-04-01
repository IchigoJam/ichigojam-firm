#!/bin/sh

# Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

USBSERIAL=/dev/cu.usbserial-110
#USBSERIAL=/dev/cu.usbserial-10
BPS=230400
PATH=~/Library/Arduino15/packages/SPRESENSE/tools/spresense-tools/3.0.1/flash_writer/macosx
${PATH}/flash_writer -s -b ${BPS} -c ${USBSERIAL} -d -n ichigojam-spr.spk
