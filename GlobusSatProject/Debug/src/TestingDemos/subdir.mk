################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TestingDemos/CommandsTestingDemo.c \
../src/TestingDemos/EpsTestingDemo.c \
../src/TestingDemos/FileSystemTestingDemo.c \
../src/TestingDemos/MainTest.c \
../src/TestingDemos/MaintenanceTestingDemo.c \
../src/TestingDemos/TelemetryTestingDemo.c \
../src/TestingDemos/TrxvuTestingDemo.c 

OBJS += \
./src/TestingDemos/CommandsTestingDemo.o \
./src/TestingDemos/EpsTestingDemo.o \
./src/TestingDemos/FileSystemTestingDemo.o \
./src/TestingDemos/MainTest.o \
./src/TestingDemos/MaintenanceTestingDemo.o \
./src/TestingDemos/TelemetryTestingDemo.o \
./src/TestingDemos/TrxvuTestingDemo.o 

C_DEPS += \
./src/TestingDemos/CommandsTestingDemo.d \
./src/TestingDemos/EpsTestingDemo.d \
./src/TestingDemos/FileSystemTestingDemo.d \
./src/TestingDemos/MainTest.d \
./src/TestingDemos/MaintenanceTestingDemo.d \
./src/TestingDemos/TelemetryTestingDemo.d \
./src/TestingDemos/TrxvuTestingDemo.d 


# Each subdirectory must supply rules for building sources it contributes
src/TestingDemos/%.o: ../src/TestingDemos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -std=c99 -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -std=c99 -Dsdram -Dat91sam9g20 -DTRACE_LEVEL=5 -DDEBUG=1 -D'BASE_REVISION_NUMBER=$(REV)' -D'BASE_REVISION_HASH_SHORT=$(REVHASH_SHORT)' -D'BASE_REVISION_HASH=$(REVHASH)' -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//satellite-subsystems/satellite-subsystems/include" -I"C:\ISIS\workspace\GlobusSatProjectNew\GlobusSatProject\src" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hcc/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/freertos/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/hal/include" -I"C:/ISIS/workspace/GlobusSatProjectNew/GlobusSatProject/..//hal/at91/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


