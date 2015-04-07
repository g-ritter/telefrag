################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/__telefrag.cpp \
../src/_errorlog.cpp \
../src/_render_shorthand.cpp \
../src/_stringfxns.cpp \
../src/animation.cpp \
../src/game.cpp \
../src/keymap.cpp \
../src/particle.cpp \
../src/player.cpp 

OBJS += \
./src/__telefrag.o \
./src/_errorlog.o \
./src/_render_shorthand.o \
./src/_stringfxns.o \
./src/animation.o \
./src/game.o \
./src/keymap.o \
./src/particle.o \
./src/player.o 

CPP_DEPS += \
./src/__telefrag.d \
./src/_errorlog.d \
./src/_render_shorthand.d \
./src/_stringfxns.d \
./src/animation.d \
./src/game.d \
./src/keymap.d \
./src/particle.d \
./src/player.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++4.7 -std=c++0x -I/Library/Frameworks/boost/geometry -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


