################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SubSystemModules/Communication/SubsystemCommands/EPS_Commands.c \
../src/SubSystemModules/Communication/SubsystemCommands/FS_Commands.c \
../src/SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.c \
../src/SubSystemModules/Communication/SubsystemCommands/TRXVU_Commands.c 

OBJS += \
./src/SubSystemModules/Communication/SubsystemCommands/EPS_Commands.o \
./src/SubSystemModules/Communication/SubsystemCommands/FS_Commands.o \
./src/SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.o \
./src/SubSystemModules/Communication/SubsystemCommands/TRXVU_Commands.o 

C_DEPS += \
./src/SubSystemModules/Communication/SubsystemCommands/EPS_Commands.d \
./src/SubSystemModules/Communication/SubsystemCommands/FS_Commands.d \
./src/SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.d \
./src/SubSystemModules/Communication/SubsystemCommands/TRXVU_Commands.d 


# Each subdirectory must supply rules for building sources it contributes
src/SubSystemModules/Communication/SubsystemCommands/%.o: ../src/SubSystemModules/Communication/SubsystemCommands/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -std=c99 -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -std=c99 -Dsdram -Dat91sam9g20 -DTRACE_LEVEL=5 -DDEBUG=1 -D'BASE_REVISION_NUMBER=$(REV)' -D'BASE_REVISION_HASH_SHORT=$(REVHASH_SHORT)' -D'BASE_REVISION_HASH=$(REVHASH)' -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//satellite-subsystems/satellite-subsystems/include" -I"C:\ISIS\workspace\GlobusSatProjectNew\GlobusSatProject\src" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hcc/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/freertos/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hal/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/at91/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


