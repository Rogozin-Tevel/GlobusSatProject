################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Demos/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
satellite-subsystems-demo.bin \

SECONDARY_SIZE += \
satellite-subsystems-demo.siz \


# All Target
all: satellite-subsystems-demo.elf secondary-outputs

# Tool invocations
satellite-subsystems-demo.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=arm926ej-s -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -T C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/at91/linker-scripts/sdram.lds -nostartfiles -Xlinker --gc-sections -L"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/at91/lib" -L"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/freertos/lib" -L"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/hal/lib" -L"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/hcc/lib" -L"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/..//satellite-subsystems/lib" -Wl,-Map,"satellite-subsystems-demo.map" -lc -o "satellite-subsystems-demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

satellite-subsystems-demo.bin: satellite-subsystems-demo.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary "satellite-subsystems-demo.elf"  "satellite-subsystems-demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

satellite-subsystems-demo.siz: satellite-subsystems-demo.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "satellite-subsystems-demo.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(S_UPPER_DEPS) satellite-subsystems-demo.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
