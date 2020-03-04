################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Demos/GomEPSdemo.c \
../src/Demos/IsisAntSdemo.c \
../src/Demos/IsisMTQv1demo.c \
../src/Demos/IsisMTQv2demo.c \
../src/Demos/IsisSPdemo.c \
../src/Demos/IsisSPv2demo.c \
../src/Demos/IsisTRXUVdemo.c \
../src/Demos/IsisTRXVUdemo.c \
../src/Demos/IsisTxSdemo.c \
../src/Demos/ScsGeckoDemo.c \
../src/Demos/common.c \
../src/Demos/cspaceADCSdemo.c \
../src/Demos/input.c \
../src/Demos/isis_eps_demo.c \
../src/Demos/trxvu_frame_ready.c 

OBJS += \
./src/Demos/GomEPSdemo.o \
./src/Demos/IsisAntSdemo.o \
./src/Demos/IsisMTQv1demo.o \
./src/Demos/IsisMTQv2demo.o \
./src/Demos/IsisSPdemo.o \
./src/Demos/IsisSPv2demo.o \
./src/Demos/IsisTRXUVdemo.o \
./src/Demos/IsisTRXVUdemo.o \
./src/Demos/IsisTxSdemo.o \
./src/Demos/ScsGeckoDemo.o \
./src/Demos/common.o \
./src/Demos/cspaceADCSdemo.o \
./src/Demos/input.o \
./src/Demos/isis_eps_demo.o \
./src/Demos/trxvu_frame_ready.o 

C_DEPS += \
./src/Demos/GomEPSdemo.d \
./src/Demos/IsisAntSdemo.d \
./src/Demos/IsisMTQv1demo.d \
./src/Demos/IsisMTQv2demo.d \
./src/Demos/IsisSPdemo.d \
./src/Demos/IsisSPv2demo.d \
./src/Demos/IsisTRXUVdemo.d \
./src/Demos/IsisTRXVUdemo.d \
./src/Demos/IsisTxSdemo.d \
./src/Demos/ScsGeckoDemo.d \
./src/Demos/common.d \
./src/Demos/cspaceADCSdemo.d \
./src/Demos/input.d \
./src/Demos/isis_eps_demo.d \
./src/Demos/trxvu_frame_ready.d 


# Each subdirectory must supply rules for building sources it contributes
src/Demos/%.o: ../src/Demos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -O0 -fmessage-length=0 -ffunction-sections -Wall -Wextra  -g -Dsdram -Dat91sam9g20 -DTRACE_LEVEL=5 -DDEBUG=1 -D'BASE_REVISION_NUMBER=$(REV)' -D'BASE_REVISION_HASH_SHORT=$(REVHASH_SHORT)' -D'BASE_REVISION_HASH=$(REVHASH)' -I"C:\ISIS\workspace\new-lib\satellite-subsystems\demo\src" -I"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/at91/include" -I"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/hal/include" -I"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/freertos/include" -I"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/../..//hal/hcc/include" -I"C:/ISIS/workspace/new-lib/satellite-subsystems/demo/..//satellite-subsystems/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


