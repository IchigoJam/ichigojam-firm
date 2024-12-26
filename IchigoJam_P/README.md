# IchigoJam P

## Setup
- Install **CMake** and **GCC**  

- Place the following libraries directly under the `IchigoJam_P` directory:  
    - **IchigoJam_BASIC**: Copy the `IchigoJam_BASIC` directory located one level up  
    - **pico-sdk**: https://github.com/NaturalStyle/pico-sdk.git (must use commit `196662b`)  
    - **pico-extras**: https://github.com/raspberrypi/pico-extras.git (development used commit `09c64d5`)  
    - **PicoDVI**: https://github.com/mlorenzati/PicoDVI.git (must use commit `579eecc`)  

- Set the following paths:  
    ```sh
    PICO_SDK_PATH=foo/IchigoJam_P/pico-sdk  
    PICO_EXTRAS_PATH=foo/IchigoJam_P/pico-extras  
    ```

Refer to the following document for detailed instructions:  
[Getting Started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)  

## Build
```sh
cd IchigoJam_P  
mkdir build  
cd build  
cmake ..  
make  
```

Write `IchigoJam_P.uf2` file to the Pico device

## Troubleshooting
If something goes wrong, delete the `build` directory and start over  
This is particularly effective if video output issues occur  

## TODO
Add support for higher resolutions (to enable use with TVs)  

## Trademark
IchigoJam is a registered trademark of jig.jp co., ltd.  
Raspberry Pi is a registered trademark of the Raspberry Pi Foundation  

NaturalStyle co., ltd.  
Created on 2024-12-25