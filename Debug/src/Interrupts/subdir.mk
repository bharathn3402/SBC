################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS_QUOTED += \
"../src/Interrupts/intc_sw_handlers.S" \

S_UPPER_SRCS += \
../src/Interrupts/intc_sw_handlers.S \

C_SRCS_QUOTED += \
"../src/Interrupts/MPC57xx__Interrupt_Init.c" \
"../src/Interrupts/Vector.c" \
"../src/Interrupts/flashrchw.c" \
"../src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.c" \

C_SRCS += \
../src/Interrupts/MPC57xx__Interrupt_Init.c \
../src/Interrupts/Vector.c \
../src/Interrupts/flashrchw.c \
../src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.c \

OBJS += \
./src/Interrupts/MPC57xx__Interrupt_Init.o \
./src/Interrupts/Vector.o \
./src/Interrupts/flashrchw.o \
./src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.o \
./src/Interrupts/intc_sw_handlers.o \

C_DEPS += \
./src/Interrupts/MPC57xx__Interrupt_Init.d \
./src/Interrupts/Vector.d \
./src/Interrupts/flashrchw.d \
./src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.d \

OBJS_QUOTED += \
"./src/Interrupts/MPC57xx__Interrupt_Init.o" \
"./src/Interrupts/Vector.o" \
"./src/Interrupts/flashrchw.o" \
"./src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.o" \
"./src/Interrupts/intc_sw_handlers.o" \

OBJS_OS_FORMAT += \
./src/Interrupts/MPC57xx__Interrupt_Init.o \
./src/Interrupts/Vector.o \
./src/Interrupts/flashrchw.o \
./src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.o \
./src/Interrupts/intc_sw_handlers.o \

C_DEPS_QUOTED += \
"./src/Interrupts/MPC57xx__Interrupt_Init.d" \
"./src/Interrupts/Vector.d" \
"./src/Interrupts/flashrchw.d" \
"./src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.d" \


# Each subdirectory must supply rules for building sources it contributes
src/Interrupts/MPC57xx__Interrupt_Init.o: ../src/Interrupts/MPC57xx__Interrupt_Init.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Interrupts/MPC57xx__Interrupt_Init.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/Interrupts/MPC57xx__Interrupt_Init.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Interrupts/Vector.o: ../src/Interrupts/Vector.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Interrupts/Vector.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/Interrupts/Vector.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Interrupts/flashrchw.o: ../src/Interrupts/flashrchw.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Interrupts/flashrchw.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/Interrupts/flashrchw.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.o: ../src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/Interrupts/intc_SW_mode_isr_vectors_MPC5744P.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Interrupts/intc_sw_handlers.o: ../src/Interrupts/intc_sw_handlers.S
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc "@src/Interrupts/intc_sw_handlers.args" -c -o "src/Interrupts/intc_sw_handlers.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


