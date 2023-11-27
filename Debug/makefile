################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 16
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include src/Modules/subdir.mk
-include src/Interrupts/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
MPC5744P_FS65_example.elf \

EXECUTABLES_QUOTED += \
"MPC5744P_FS65_example.elf" \

EXECUTABLES_OS_FORMAT += \
MPC5744P_FS65_example.elf \


# All Target
all: warning-messages explicit-dependencies MPC5744P_FS65_example.elf secondary-outputs

# Tool invocations
MPC5744P_FS65_example.elf: $(OBJS) C:/Users/BRNy001/OneDrive\ -\ NXP/6.WORK/1.MPC57xx-Projects/Panther/NXP_FS65xx_drivers_v1/Example/MPC5744P_FS65_example/Project_Settings/Linker_Files/mem.ld C:/Users/BRNy001/OneDrive\ -\ NXP/6.WORK/1.MPC57xx-Projects/Panther/NXP_FS65xx_drivers_v1/Example/MPC5744P_FS65_example/Project_Settings/Linker_Files/sections.ld C:/Users/BRNy001/OneDrive\ -\ NXP/6.WORK/1.MPC57xx-Projects/Panther/NXP_FS65xx_drivers_v1/Example/MPC5744P_FS65_example/Project_Settings/Linker_Files/libs.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #17 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -o "MPC5744P_FS65_example.elf" "@MPC5744P_FS65_example.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*/*.o ./*/*.o  $(EXECUTABLES_QUOTED) ./*/*/*.d ./*/*.d  C:\Users\BRNy001\OneDrive - NXP\6.WORK\1.MPC57xx-Projects\Panther\NXP_FS65xx_drivers_v1\Example\MPC5744P_FS65_example\Debug\MPC5744P_FS65_example.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

