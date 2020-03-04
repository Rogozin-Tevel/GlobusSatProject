################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SubSystemModules/Housekeeping/TelemetryCollector.c 

OBJS += \
./src/SubSystemModules/Housekeeping/TelemetryCollector.o 

C_DEPS += \
./src/SubSystemModules/Housekeeping/TelemetryCollector.d 


# Each subdirectory must supply rules for building sources it contributes
src/SubSystemModules/Housekeeping/%.o: ../src/SubSystemModules/Housekeeping/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


