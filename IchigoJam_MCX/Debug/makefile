################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/str/subdir.mk
-include utilities/debug_console/subdir.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/timer_manager/subdir.mk
-include component/timer/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include component/led/subdir.mk
-include component/gpio/subdir.mk
-include board/subdir.mk
-include IchigoJam_BASIC/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := IchigoJam_MCX
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: IchigoJam_MCX.axf

# Tool invocations
IchigoJam_MCX.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="IchigoJam_MCX.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m33+nodsp -mthumb -T IchigoJam_MCX_Debug.ld -o "IchigoJam_MCX.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) IchigoJam_MCX.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "IchigoJam_MCX.axf"; # arm-none-eabi-objcopy -v -O binary "IchigoJam_MCX.axf" "IchigoJam_MCX.bin" ; # checksum -p MCXA153 -d "IchigoJam_MCX.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
