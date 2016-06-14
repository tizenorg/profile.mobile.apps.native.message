# Add inputs and outputs from these tool invocations to the build variables

ifneq ($(strip $(MKDIR_BIN)),)
MKDIR = $(MKDIR_BIN)
MKDIR_OP = -p
else
MKDIR = mkdir
MKDIR_OP = -p
endif

EDJE_CC = edje_cc
