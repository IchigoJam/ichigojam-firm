################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXA153VLH -DCPU_MCXA153VLH_cm33_nodsp -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -DTIMER_PORT_TYPE_CTIMER=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/board" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/source" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/lists" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/drivers" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/CMSIS" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/CMSIS/m-profile" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/device" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/device/periph" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/serial_manager" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities/debug_console" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities/str" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities/debug_console/config" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/uart" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/led" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/timer" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/gpio" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/timer_manager" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

