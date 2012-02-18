################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/balancer/balancer.c 

OBJS += \
./src/balancer/balancer.o 

C_DEPS += \
./src/balancer/balancer.d 


# Each subdirectory must supply rules for building sources it contributes
src/balancer/%.o: ../src/balancer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


