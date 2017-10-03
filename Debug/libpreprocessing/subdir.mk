################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libpreprocessing/FITS_Interface.c \
../libpreprocessing/ana.c \
../libpreprocessing/arith.c \
../libpreprocessing/flatfield.c \
../libpreprocessing/main.c \
../libpreprocessing/vmem.c 

OBJS += \
./libpreprocessing/FITS_Interface.o \
./libpreprocessing/ana.o \
./libpreprocessing/arith.o \
./libpreprocessing/flatfield.o \
./libpreprocessing/main.o \
./libpreprocessing/vmem.o 

C_DEPS += \
./libpreprocessing/FITS_Interface.d \
./libpreprocessing/ana.d \
./libpreprocessing/arith.d \
./libpreprocessing/flatfield.d \
./libpreprocessing/main.d \
./libpreprocessing/vmem.d 


# Each subdirectory must supply rules for building sources it contributes
libpreprocessing/%.o: ../libpreprocessing/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


