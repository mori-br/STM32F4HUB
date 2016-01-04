################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL_Driver/Src/stm32f4xx_hal.c \
../HAL_Driver/Src/stm32f4xx_hal_cortex.c \
../HAL_Driver/Src/stm32f4xx_hal_dma.c \
../HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_flash.c \
../HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
../HAL_Driver/Src/stm32f4xx_hal_gpio.c \
../HAL_Driver/Src/stm32f4xx_hal_hcd.c \
../HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_pcd.c \
../HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_pwr.c \
../HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_rcc.c \
../HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_tim.c \
../HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
../HAL_Driver/Src/stm32f4xx_hal_uart.c \
../HAL_Driver/Src/stm32f4xx_hal_usart.c \
../HAL_Driver/Src/stm32f4xx_ll_usb.c 

OBJS += \
./HAL_Driver/Src/stm32f4xx_hal.o \
./HAL_Driver/Src/stm32f4xx_hal_cortex.o \
./HAL_Driver/Src/stm32f4xx_hal_dma.o \
./HAL_Driver/Src/stm32f4xx_hal_dma_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_flash.o \
./HAL_Driver/Src/stm32f4xx_hal_flash_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o \
./HAL_Driver/Src/stm32f4xx_hal_gpio.o \
./HAL_Driver/Src/stm32f4xx_hal_hcd.o \
./HAL_Driver/Src/stm32f4xx_hal_i2c_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_pcd.o \
./HAL_Driver/Src/stm32f4xx_hal_pcd_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_pwr.o \
./HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_rcc.o \
./HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_tim.o \
./HAL_Driver/Src/stm32f4xx_hal_tim_ex.o \
./HAL_Driver/Src/stm32f4xx_hal_uart.o \
./HAL_Driver/Src/stm32f4xx_hal_usart.o \
./HAL_Driver/Src/stm32f4xx_ll_usb.o 

C_DEPS += \
./HAL_Driver/Src/stm32f4xx_hal.d \
./HAL_Driver/Src/stm32f4xx_hal_cortex.d \
./HAL_Driver/Src/stm32f4xx_hal_dma.d \
./HAL_Driver/Src/stm32f4xx_hal_dma_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_flash.d \
./HAL_Driver/Src/stm32f4xx_hal_flash_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.d \
./HAL_Driver/Src/stm32f4xx_hal_gpio.d \
./HAL_Driver/Src/stm32f4xx_hal_hcd.d \
./HAL_Driver/Src/stm32f4xx_hal_i2c_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_pcd.d \
./HAL_Driver/Src/stm32f4xx_hal_pcd_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_pwr.d \
./HAL_Driver/Src/stm32f4xx_hal_pwr_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_rcc.d \
./HAL_Driver/Src/stm32f4xx_hal_rcc_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_tim.d \
./HAL_Driver/Src/stm32f4xx_hal_tim_ex.d \
./HAL_Driver/Src/stm32f4xx_hal_uart.d \
./HAL_Driver/Src/stm32f4xx_hal_usart.d \
./HAL_Driver/Src/stm32f4xx_ll_usb.d 


# Each subdirectory must supply rules for building sources it contributes
HAL_Driver/Src/%.o: ../HAL_Driver/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/core" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/device" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc/Legacy" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities/STM32F4-Discovery" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HUB" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


