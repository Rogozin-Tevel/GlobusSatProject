################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SubSystemModules/Communication/AckHandler.c \
../src/SubSystemModules/Communication/ActUponCommand.c \
../src/SubSystemModules/Communication/CommandDictionary.c \
../src/SubSystemModules/Communication/SatCommandHandler.c \
../src/SubSystemModules/Communication/TRXVU.c 

OBJS += \
./src/SubSystemModules/Communication/AckHandler.o \
./src/SubSystemModules/Communication/ActUponCommand.o \
./src/SubSystemModules/Communication/CommandDictionary.o \
./src/SubSystemModules/Communication/SatCommandHandler.o \
./src/SubSystemModules/Communication/TRXVU.o 

C_DEPS += \
./src/SubSystemModules/Communication/AckHandler.d \
./src/SubSystemModules/Communication/ActUponCommand.d \
./src/SubSystemModules/Communication/CommandDictionary.d \
./src/SubSystemModules/Communication/SatCommandHandler.d \
./src/SubSystemModules/Communication/TRXVU.d 


# Each subdirectory must supply rules for building sources it contributes
src/SubSystemModules/Communication/%.o: ../src/SubSystemModules/Communication/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -std=c99 -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -std=c99 -Dsdram -Dat91sam9g20 -DTRACE_LEVEL=5 -DDEBUG=1 -D'BASE_REVISION_NUMBER=$(REV)' -D'BASE_REVISION_HASH_SHORT=$(REVHASH_SHORT)' -D'BASE_REVISION_HASH=$(REVHASH)' -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//satellite-subsystems/satellite-subsystems/include" -I"C:\ISIS\workspace\GlobusSatProjectNew\GlobusSatProject\src" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hcc/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/freertos/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hal/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/at91/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


