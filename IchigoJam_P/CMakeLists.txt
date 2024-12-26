cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(IchigoJam_P)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(DVI_DEFAULT_SERIAL_CONFIG "pico_sock_cfg" CACHE STRING
	"Select a default pin configuration from common_dvi_pin_configs.h" FORCE)
include_directories(
	PicoDVI/software/include
)
add_subdirectory(PicoDVI/software/libdvi)
add_subdirectory(PicoDVI/software/libsprite)

add_executable(IchigoJam_P
	src/main.c
)

pico_set_linker_script(IchigoJam_P ${CMAKE_CURRENT_LIST_DIR}/memmap_ichigojam_p.ld)

target_include_directories(IchigoJam_P PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	# Make sure TinyUSB can find tusb_config.h
	${CMAKE_CURRENT_LIST_DIR}/src
)

# PicoDVI/software/apps/mandelbrot/CMakeLists.txt から引用
target_compile_definitions(IchigoJam_P PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
)

target_link_libraries(IchigoJam_P PUBLIC
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	libsprite
	tinyusb_host
	tinyusb_board
	hardware_flash
	hardware_watchdog
	hardware_adc
	hardware_pwm
	hardware_i2c
	hardware_sleep
)

# pico_enable_stdio_usb(IchigoJam_P 0)
pico_enable_stdio_uart(IchigoJam_P 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(IchigoJam_P)