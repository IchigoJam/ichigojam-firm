################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/gpio/fsl_adapter_gpio.c 

C_DEPS += \
./component/gpio/fsl_adapter_gpio.d 

OBJS += \
./component/gpio/fsl_adapter_gpio.o 


# Each subdirectory must supply rules for building sources it contributes
component/gpio/%.o: ../component/gpio/%.c component/gpio/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXA153VLH -DCPU_MCXA153VLH_cm33_nodsp -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -DTIMER_PORT_TYPE_CTIMER=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/board" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/source" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/lists" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/drivers" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/CMSIS" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/CMSIS/m-profile" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/device" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/device/periph" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/utilities" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/serial_manager" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/utilities/debug_console" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/utilities/str" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/utilities/debug_console/config" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/uart" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/led" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/timer" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/gpio" -I"/Users/fukuno/data/c/ichigojam-firm/IchigoJam_MCX/component/timer_manager" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-gpio

clean-component-2f-gpio:
	-$(RM) ./component/gpio/fsl_adapter_gpio.d ./component/gpio/fsl_adapter_gpio.o

.PHONY: clean-component-2f-gpio

