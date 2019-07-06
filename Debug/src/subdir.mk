################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/startup_stm32f103xb.s 

C_SRCS += \
../src/adc.c \
../src/dma.c \
../src/i2c.c \
../src/init.c \
../src/main.c \
../src/stm32f1xx_it.c \
../src/syscalls.c \
../src/system_stm32f1xx.c \
../src/tim.c \
../src/uart.c 

OBJS += \
./src/adc.o \
./src/dma.o \
./src/i2c.o \
./src/init.o \
./src/main.o \
./src/startup_stm32f103xb.o \
./src/stm32f1xx_it.o \
./src/syscalls.o \
./src/system_stm32f1xx.o \
./src/tim.o \
./src/uart.o 

C_DEPS += \
./src/adc.d \
./src/dma.d \
./src/i2c.d \
./src/init.d \
./src/main.d \
./src/stm32f1xx_it.d \
./src/syscalls.d \
./src/system_stm32f1xx.d \
./src/tim.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DNUCLEO_F103RB -DDEBUG -DSTM32F103xB -DUSE_HAL_DRIVER -I"C:/Users/Agata/workspace/F1_Mag/HAL_Driver/Inc/Legacy" -I"C:/Users/Agata/workspace/F1_Mag/Utilities/STM32F1xx_Nucleo" -I"C:/Users/Agata/workspace/F1_Mag/inc" -I"C:/Users/Agata/workspace/F1_Mag/CMSIS/device" -I"C:/Users/Agata/workspace/F1_Mag/CMSIS/core" -I"C:/Users/Agata/workspace/F1_Mag/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"C:/Users/Agata/workspace/F1_Mag/HAL_Driver/Inc/Legacy" -I"C:/Users/Agata/workspace/F1_Mag/Utilities/STM32F1xx_Nucleo" -I"C:/Users/Agata/workspace/F1_Mag/inc" -I"C:/Users/Agata/workspace/F1_Mag/CMSIS/device" -I"C:/Users/Agata/workspace/F1_Mag/CMSIS/core" -I"C:/Users/Agata/workspace/F1_Mag/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


