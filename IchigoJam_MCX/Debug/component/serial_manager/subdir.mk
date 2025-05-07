################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/serial_manager/fsl_component_serial_manager.c \
../component/serial_manager/fsl_component_serial_port_uart.c 

C_DEPS += \
./component/serial_manager/fsl_component_serial_manager.d \
./component/serial_manager/fsl_component_serial_port_uart.d 

OBJS += \
./component/serial_manager/fsl_component_serial_manager.o \
./component/serial_manager/fsl_component_serial_port_uart.o 


# Each subdirectory must supply rules for building sources it contributes
component/serial_manager/%.o: ../component/serial_manager/%.c component/serial_manager/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXA153VLH -DCPU_MCXA153VLH_cm33_nodsp -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -DTIMER_PORT_TYPE_CTIMER=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/board" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/source" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/lists" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/drivers" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/CMSIS" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/CMSIS/m-profile" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/device" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/device/periph" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/serial_manager" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities/debug_console" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities/str" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/utilities/debug_console/config" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/uart" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/led" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/timer" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/gpio" -I"/Users/tedd/dev/mcuxpresso/__ij_mcx/IchigoJam_MCX/component/timer_manager" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-serial_manager

clean-component-2f-serial_manager:
	-$(RM) ./component/serial_manager/fsl_component_serial_manager.d ./component/serial_manager/fsl_component_serial_manager.o ./component/serial_manager/fsl_component_serial_port_uart.d ./component/serial_manager/fsl_component_serial_port_uart.o

.PHONY: clean-component-2f-serial_manager

