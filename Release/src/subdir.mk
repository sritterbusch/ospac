################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Analyzer.cpp \
../src/Channel.cpp \
../src/CrosstalkFilter.cpp \
../src/CrosstalkGate.cpp \
../src/Encode.cpp \
../src/Equalizer.cpp \
../src/Frequency.cpp \
../src/Log.cpp \
../src/Maximizer.cpp \
../src/Merge.cpp \
../src/MonoMix.cpp \
../src/OspacMain.cpp \
../src/Physics.cpp \
../src/Plot.cpp \
../src/SelectiveLeveler.cpp \
../src/Skip.cpp \
../src/StereoMix.cpp \
../src/Wave.cpp 

OBJS += \
./src/Analyzer.o \
./src/Channel.o \
./src/CrosstalkFilter.o \
./src/CrosstalkGate.o \
./src/Encode.o \
./src/Equalizer.o \
./src/Frequency.o \
./src/Log.o \
./src/Maximizer.o \
./src/Merge.o \
./src/MonoMix.o \
./src/OspacMain.o \
./src/Physics.o \
./src/Plot.o \
./src/SelectiveLeveler.o \
./src/Skip.o \
./src/StereoMix.o \
./src/Wave.o 

CPP_DEPS += \
./src/Analyzer.d \
./src/Channel.d \
./src/CrosstalkFilter.d \
./src/CrosstalkGate.d \
./src/Encode.d \
./src/Equalizer.d \
./src/Frequency.d \
./src/Log.d \
./src/Maximizer.d \
./src/Merge.d \
./src/MonoMix.d \
./src/OspacMain.d \
./src/Physics.d \
./src/Plot.d \
./src/SelectiveLeveler.d \
./src/Skip.d \
./src/StereoMix.d \
./src/Wave.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DCLI -I/usr/local/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


