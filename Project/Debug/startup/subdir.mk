################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f407xx.s 

OBJS += \
./startup/startup_stm32f407xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/core" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/device" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc/Legacy" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities/STM32F4-Discovery" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


