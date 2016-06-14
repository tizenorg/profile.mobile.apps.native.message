# Add inputs and outputs from these tool invocations to the build variables

C_DEPS +=

USES_EFL = yes

SYSROOT = $(SBI_SYSROOT)

ifneq ($(strip $(SYSROOT)),)

ifeq ($(strip $(USES_EFL)),yes)
_EFL_INCS = ecore-1 \
            e_dbus-1 edje-1 eet-1 \
            efreet-1 eina-1 "eina-1/eina" \
            eio-1 embryo-1 \
            evas-1 elementary-1 ethumb-1 \
            efl-extension
_EFL_INC_ROOT = $(SYSROOT)/usr/include
EFL_INCS = $(addprefix -I $(_EFL_INC_ROOT)/,$(_EFL_INCS))
endif

FLATFORM_INCS = $(EFL_INCS) \
  -I"$(SYSROOT)/usr/include/libxml2" \
  -I"$(SDK_PATH)/library" \
  -I"$(SYSROOT)/usr/include" \
  -I"$(SYSROOT)/usr/include/appcore-agent" \
  -I"$(SYSROOT)/usr/include/appfw" \
  -I"$(SYSROOT)/usr/include/badge" \
  -I"$(SYSROOT)/usr/include/base" \
  -I"$(SYSROOT)/usr/include/cairo" \
  -I"$(SYSROOT)/usr/include/calendar-service2" \
  -I"$(SYSROOT)/usr/include/ckm" \
  -I"$(SYSROOT)/usr/include/contacts-svc" \
  -I"$(SYSROOT)/usr/include/content" \
  -I"$(SYSROOT)/usr/include/context-manager" \
  -I"$(SYSROOT)/usr/include/device" \
  -I"$(SYSROOT)/usr/include/dlog" \
  -I"$(SYSROOT)/usr/include/dbus-1.0" \
  -I"$(SYSROOT)/usr/include/ewebkit2-0" \
  -I"$(SYSROOT)/usr/include/fontconfig" \
  -I"$(SYSROOT)/usr/include/freetype2" \
  -I"$(SYSROOT)/usr/include/gio-unix-2.0" \
  -I"$(SYSROOT)/usr/include/glib-2.0" \
  -I"$(SYSROOT)/usr/include/json-glib-1.0" \
  -I"$(SYSROOT)/usr/include/json-glib-1.0/json-glib" \
  -I"$(SYSROOT)/usr/include/location" \
  -I"$(SYSROOT)/usr/include/media" \
  -I"$(SYSROOT)/usr/include/media-content" \
  -I"$(SYSROOT)/usr/include/messaging" \
  -I"$(SYSROOT)/usr/include/minizip" \
  -I"$(SYSROOT)/usr/include/network" \
  -I"$(SYSROOT)/usr/include/notification" \
  -I"$(SYSROOT)/usr/include/sensor" \
  -I"$(SYSROOT)/usr/include/shortcut" \
  -I"$(SYSROOT)/usr/include/storage" \
  -I"$(SYSROOT)/usr/include/system" \
  -I"$(SYSROOT)/usr/include/telephony" \
  -I"$(SYSROOT)/usr/include/ui" \
  -I"$(SYSROOT)/usr/include/web" \
  -I"$(SYSROOT)/usr/include/wifi-direct" \
  -I"$(SYSROOT)/usr/lib/dbus-1.0/include" \
  -I"$(SYSROOT)/usr/lib/glib-2.0/include" \
  -I"$(SYSROOT)/usr/include/appcore-watch" \
  -I"$(SYSROOT)/usr/include/widget_service"

endif
