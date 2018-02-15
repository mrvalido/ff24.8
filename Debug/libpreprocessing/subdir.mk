################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libpreprocessing/ana.c \
../libpreprocessing/arith.c \
../libpreprocessing/flatfield.c \
../libpreprocessing/vmem.c 

OBJS += \
./libpreprocessing/ana.o \
./libpreprocessing/arith.o \
./libpreprocessing/flatfield.o \
./libpreprocessing/vmem.o 

C_DEPS += \
./libpreprocessing/ana.d \
./libpreprocessing/arith.d \
./libpreprocessing/flatfield.d \
./libpreprocessing/vmem.d 


# Each subdirectory must supply rules for building sources it contributes
libpreprocessing/%.o: ../libpreprocessing/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


