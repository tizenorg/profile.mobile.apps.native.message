APPTYPE = $(type)
PROJ_ROOT = $(PROJPATH)

-include $(PROJ_ROOT)/project_def.prop
-include basedef.mk
-include tooldef.mk

ifeq ($(strip $(BUILD_CONFIG)),)
BUILD_CONFIG = Debug
endif

OUTPUT_DIR = $(PROJ_ROOT)/$(BUILD_CONFIG)
OBJ_OUTPUT = $(OUTPUT_DIR)/objs

SRCS = $(USER_SRCS)
EDCS = $(USER_EDCS)
CDEFS += $(addprefix -D ,$(USER_DEFS))
INCS += $(addprefix -I $(PROJ_ROOT)/,$(USER_INC_DIRS))
LIBS += $(addprefix -l,$(USER_LIBS))
UOBJS += $(addprefix $(PROJ_ROOT)/,$(USER_OBJS))

OBJS = $(patsubst %.c,$(OBJ_OUTPUT)/%.o,$(SRCS))
EDJ_FILES = $(patsubst %.edc,$(OUTPUT_DIR)/%.edj,$(EDCS))
DEPS = $(OBJS:.o=.d)
M_OPT = -MMD -MP -MF"$(@:%.o=%.d)"

ifeq ($(strip $(APPTYPE)),app)
APPFILE = $(OUTPUT_DIR)/$(APPNAME)
endif
ifeq ($(strip $(APPTYPE)),staticLib)
APPFILE = $(OUTPUT_DIR)/lib$(APPNAME).a
endif
ifeq ($(strip $(APPTYPE)),sharedLib)
APPFILE = $(OUTPUT_DIR)/lib$(APPNAME).so
endif

all : $(APPFILE) secondary-outputs

ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif

ifeq ($(strip $(APPTYPE)),app)
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo ' > Building target: $@'
	@echo ' > Invoking: C++ Linker'
	$(MKDIR) $(MKDIR_OP) $(@D)
	$(CXX) -o "$(APPFILE)" $(OBJS) $(UOBJS) $(LIBS) -L"$(PROJ_PATH)/lib" $(TC_LINKER_MISC) $(RS_LINKER_MISC) -pie -lpthread -Xlinker -rpath="/home/developer/sdk_tools/lib" --sysroot="$(SYSROOT)" -Xlinker --version-script="$(PROJ_PATH)/.exportMap" -L"$(SBI_SYSROOT)/usr/lib" $(RS_LIBRARIES) -Xlinker -rpath="/opt/usr/apps/$(APPID)/lib" -Werror-implicit-function-declaration
	@echo ' > Finished building target: $@'
endif
ifeq ($(strip $(APPTYPE)),staticLib)
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo ' > Building target: $@'
	@echo ' > Invoking: C++ Linker'
	$(MKDIR) $(MKDIR_OP) $(@D)
	$(AR) -r "$(APPFILE)" $(OBJS) $(UOBJS) $(LIBS)
	@echo ' > Finished building target: $@'
endif
ifeq ($(strip $(APPTYPE)),sharedLib)
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo ' > Building target: $@'
	@echo ' > Invoking: C++ Linker'
	$(MKDIR) $(MKDIR_OP) $(@D)
	$(CXX) -o "$(APPFILE)" $(OBJS) $(UOBJS) $(LIBS) -L"$(PROJ_PATH)/lib" -shared -Wl,--no-undefined $(TC_LINKER_MISC) $(RS_LINKER_MISC) -lpthread --sysroot="$(SYSROOT)" -L"$(SYSROOT)/usr/lib" $(RS_LIBRARIES)
	@echo ' > Finished building target: $@'
endif

$(OBJS) :
	@echo ' > Building file: $(subst $(OBJ_OUTPUT)/,,$(@:.o=.c))'
	@echo ' > Invoking: C Compiler'
	$(MKDIR) $(MKDIR_OP) $(@D)
	$(CC) -c "$(PROJ_ROOT)/$(subst $(OBJ_OUTPUT)/,,$(@:.o=.c))" -o $@ $(CDEFS) $(INCS) -I"pch" $(OPTIMIZATION_OP) $(DEBUG_OP) -Wall -c -fmessage-length=0 $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -fPIE --sysroot="$(SYSROOT)" $(FLATFORM_INCS) -Werror-implicit-function-declaration $(M_OPT)
	@echo ' > Finished building: $(subst $(OBJ_OUTPUT)/,,$(@:.o=.c))'


secondary-outputs : $(MO_FILES) $(EDJ_FILES)

ifneq ($(strip $(EDJ_FILES)),)
$(EDJ_FILES) :
	@echo ' > Building file: $(subst $(OBJ_OUTPUT)/,,$(@:.edj=.edc))'
	@echo ' > Invoking: EDC Resource Compiler'
	$(MKDIR) $(MKDIR_OP) $(@D)
	$(EDJE_CC) -id "$(PROJ_PATH)/edje/images" -id "$(SDK_TOOLPATH)/enventor/share/enventor/images" -sd "$(PROJ_PATH)/edje/sounds" -sd "$(SDK_TOOLPATH)/enventor/share/enventor/sounds" -fd "$(PROJ_PATH)/edje/fonts" -fd "$(SDK_TOOLPATH)/enventor/share/enventor/fonts" $(PROJ_ROOT)/$(subst $(OUTPUT_DIR)/,,$(@:.edj=.edc)) "$@"
	@echo ' > Finished building: $(subst $(OBJ_OUTPUT)/,,$(@:.edj=.edc))'
endif

clean :
	rm -f "$(APPFILE)"
	rm -rf "$(OUTPUT_DIR)"
