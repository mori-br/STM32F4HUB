################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/log.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/usbh_conf.c 

OBJS += \
./src/log.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/usbh_conf.o 

C_DEPS += \
./src/log.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/usbh_conf.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/core" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/device" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc/Legacy" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities/STM32F4-Discovery" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HUB" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


