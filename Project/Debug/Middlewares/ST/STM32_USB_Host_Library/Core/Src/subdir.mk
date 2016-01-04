################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Core/Src/%.o: ../Middlewares/ST/STM32_USB_Host_Library/Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/core" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/CMSIS/device" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc/Legacy" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/HAL_Driver/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Utilities/STM32F4-Discovery" -I"F:/GITHUB_PROJECTS/STM32F4HUB/Project/Middlewares/ST/STM32_USB_Host_Library/Class/HUB" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


