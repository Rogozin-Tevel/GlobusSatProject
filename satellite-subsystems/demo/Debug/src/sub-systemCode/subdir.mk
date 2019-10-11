################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sub-systemCode/ADCS.c \
../src/sub-systemCode/Ants.c \
../src/sub-systemCode/Butchering.c \
../src/sub-systemCode/DataBase.c \
../src/sub-systemCode/DelayedCommand_list.c \
../src/sub-systemCode/EPS.c \
../src/sub-systemCode/FRAM_Extended.c \
../src/sub-systemCode/GSC.c \
../src/sub-systemCode/Global.c \
../src/sub-systemCode/GlobalParam.c \
../src/sub-systemCode/HouseKeeping.c \
../src/sub-systemCode/Main_Init.c \
../src/sub-systemCode/ManualCameraHandler.c \
../src/sub-systemCode/Stage_Table.c \
../src/sub-systemCode/TRXVU.c \
../src/sub-systemCode/Test.c \
../src/sub-systemCode/commands.c \
../src/sub-systemCode/Hoopoe3.c \
../src/sub-systemCode/fileSystem.c 

OBJS += \
./src/sub-systemCode/ADCS.o \
./src/sub-systemCode/Ants.o \
./src/sub-systemCode/Butchering.o \
./src/sub-systemCode/DataBase.o \
./src/sub-systemCode/DelayedCommand_list.o \
./src/sub-systemCode/EPS.o \
./src/sub-systemCode/FRAM_Extended.o \
./src/sub-systemCode/GSC.o \
./src/sub-systemCode/Global.o \
./src/sub-systemCode/GlobalParam.o \
./src/sub-systemCode/HouseKeeping.o \
./src/sub-systemCode/Main_Init.o \
./src/sub-systemCode/ManualCameraHandler.o \
./src/sub-systemCode/Stage_Table.o \
./src/sub-systemCode/TRXVU.o \
./src/sub-systemCode/Test.o \
./src/sub-systemCode/commands.o \
./src/sub-systemCode/Hoopoe3.o \
./src/sub-systemCode/fileSystem.o 

C_DEPS += \
./src/sub-systemCode/ADCS.d \
./src/sub-systemCode/Ants.d \
./src/sub-systemCode/Butchering.d \
./src/sub-systemCode/DataBase.d \
./src/sub-systemCode/DelayedCommand_list.d \
./src/sub-systemCode/EPS.d \
./src/sub-systemCode/FRAM_Extended.d \
./src/sub-systemCode/GSC.d \
./src/sub-systemCode/Global.d \
./src/sub-systemCode/GlobalParam.d \
./src/sub-systemCode/HouseKeeping.d \
./src/sub-systemCode/Main_Init.d \
./src/sub-systemCode/ManualCameraHandler.d \
./src/sub-systemCode/Stage_Table.d \
./src/sub-systemCode/TRXVU.d \
./src/sub-systemCode/Test.d \
./src/sub-systemCode/commands.d \
./src/sub-systemCode/Hoopoe3.d \
./src/sub-systemCode/fileSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/sub-systemCode/%.o: ../src/sub-systemCode/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -Dsdram -Dat91sam9g20 -DTRACE_LEVEL=5 -DDEBUG=1 -D'BASE_REVISION_NUMBER=$(REV)' -D'BASE_REVISION_HASH_SHORT=$(REVHASH_SHORT)' -D'BASE_REVISION_HASH=$(REVHASH)' -I"C:\ISIS\workspace\satelite-code\satellite-subsystems\demo\src" -I"C:/ISIS/workspace/satelite-code/satellite-subsystems/demo/../..//hal/at91/include" -I"C:/ISIS/workspace/satelite-code/satellite-subsystems/demo/../..//hal/hal/include" -I"C:/ISIS/workspace/satelite-code/satellite-subsystems/demo/../..//hal/freertos/include" -I"C:/ISIS/workspace/satelite-code/satellite-subsystems/demo/../..//hal/hcc/include" -I"C:/ISIS/workspace/satelite-code/satellite-subsystems/demo/..//satellite-subsystems/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


