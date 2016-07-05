################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CBoat.cpp \
../src/CMotor.cpp \
../src/CSdCard.cpp \
../src/CSensor.cpp \
../src/CStateMachine.cpp \
../src/CZigbee.cpp \
../src/main.cpp 

OBJS += \
./src/CBoat.o \
./src/CMotor.o \
./src/CSdCard.o \
./src/CSensor.o \
./src/CStateMachine.o \
./src/CZigbee.o \
./src/main.o 

CPP_DEPS += \
./src/CBoat.d \
./src/CMotor.d \
./src/CSdCard.d \
./src/CSensor.d \
./src/CStateMachine.d \
./src/CZigbee.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Thesis\Arduino Workspace\arduino_core\src" -I"D:\Thesis\Arduino Workspace\thesis\arduinolib" -DARDUINO=100 -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -fno-exceptions --pedantic -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


