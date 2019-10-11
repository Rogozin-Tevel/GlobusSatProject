################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/TestingDemos/subdir.mk
-include src/SubSystemModules/PowerManagment/subdir.mk
-include src/SubSystemModules/Maintenance/subdir.mk
-include src/SubSystemModules/Housekepping/subdir.mk
-include src/SubSystemModules/Communication/SubsystemCommands/subdir.mk
-include src/SubSystemModules/Communication/subdir.mk
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
GlobusSatProject.bin \

SECONDARY_SIZE += \
GlobusSatProject.siz \


# All Target
all: GlobusSatProject.elf secondary-outputs

# Tool invocations
GlobusSatProject.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=arm926ej-s -std=c99 -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -std=c99 -T C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/at91/linker-scripts/sdram.lds -nostartfiles -Xlinker --gc-sections -L"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/at91/lib" -L"C:/ISIS/workspace/satellite-subsystems/satellite-subsystems/lib" -L"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hcc/lib" -L"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/freertos/lib" -L"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hal/lib" -Wl,-Map,"GlobusSatProject.map" --specs=nano.specs -lc -u _printf_float -u _scanf_float -o "GlobusSatProject.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

GlobusSatProject.bin: GlobusSatProject.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary "GlobusSatProject.elf"  "GlobusSatProject.bin"
	@echo 'Finished building: $@'
	@echo ' '

GlobusSatProject.siz: GlobusSatProject.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "GlobusSatProject.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(S_UPPER_DEPS) GlobusSatProject.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
