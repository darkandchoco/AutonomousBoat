################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../arduinolib/Sd2Card.cpp \
../arduinolib/SdBaseFile.cpp \
../arduinolib/SdFat.cpp \
../arduinolib/SdFatUtil.cpp \
../arduinolib/SdFile.cpp \
../arduinolib/SdVolume.cpp \
../arduinolib/SoftwareSerial.cpp \
../arduinolib/XBee.cpp \
../arduinolib/istream.cpp \
../arduinolib/ostream.cpp 

OBJS += \
./arduinolib/Sd2Card.o \
./arduinolib/SdBaseFile.o \
./arduinolib/SdFat.o \
./arduinolib/SdFatUtil.o \
./arduinolib/SdFile.o \
./arduinolib/SdVolume.o \
./arduinolib/SoftwareSerial.o \
./arduinolib/XBee.o \
./arduinolib/istream.o \
./arduinolib/ostream.o 

CPP_DEPS += \
./arduinolib/Sd2Card.d \
./arduinolib/SdBaseFile.d \
./arduinolib/SdFat.d \
./arduinolib/SdFatUtil.d \
./arduinolib/SdFile.d \
./arduinolib/SdVolume.d \
./arduinolib/SoftwareSerial.d \
./arduinolib/XBee.d \
./arduinolib/istream.d \
./arduinolib/ostream.d 


# Each subdirectory must supply rules for building sources it contributes
arduinolib/%.o: ../arduinolib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Thesis\Arduino Workspace\arduino_core\src" -I"D:\Thesis\Arduino Workspace\thesis\arduinolib" -DARDUINO=100 -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -fno-exceptions --pedantic -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


