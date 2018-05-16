################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Ipcctl.cpp \
../src/Message.cpp \
../src/MessageQueueWrapper.cpp \
../src/SemaphoreWrapper.cpp \
../src/SharedMemoryWrapper.cpp \
../src/TimerWrapper.cpp 

OBJS += \
./src/Ipcctl.o \
./src/Message.o \
./src/MessageQueueWrapper.o \
./src/SemaphoreWrapper.o \
./src/SharedMemoryWrapper.o \
./src/TimerWrapper.o 

CPP_DEPS += \
./src/Ipcctl.d \
./src/Message.d \
./src/MessageQueueWrapper.d \
./src/SemaphoreWrapper.d \
./src/SharedMemoryWrapper.d \
./src/TimerWrapper.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -I../inc -O3 -Xcompiler -fPIC -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_53,code=sm_53 -m64 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -I../inc -O3 -Xcompiler -fPIC --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


