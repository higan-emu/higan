ifeq ($(platform),windows)
  options += -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32
  options += -Wl,-enable-auto-import
  options += -Wl,-enable-runtime-pseudo-reloc
else ifeq ($(platform),macos)
else ifneq ($(filter $(platform),linux bsd),)
  options += -Wl,-export-dynamic
  options += -lX11 -lXext
else
  $(error "unsupported platform")
endif

higan.objects := higan

$(object.path)/higan.o: $(higan.path)/higan/higan.cpp

ifneq ($(filter $(cores),fc),)
  include $(higan.path)/fc/GNUmakefile
endif

ifneq ($(filter $(cores),sfc),)
  include $(higan.path)/sfc/GNUmakefile
endif

ifneq ($(filter $(cores),sg),)
  include $(higan.path)/sg/GNUmakefile
endif

ifneq ($(filter $(cores),ms),)
  include $(higan.path)/ms/GNUmakefile
endif

ifneq ($(filter $(cores),md),)
  include $(higan.path)/md/GNUmakefile
endif

ifneq ($(filter $(cores),pce),)
  include $(higan.path)/pce/GNUmakefile
endif

ifneq ($(filter $(cores),msx),)
  include $(higan.path)/msx/GNUmakefile
endif

ifneq ($(filter $(cores),cv),)
  include $(higan.path)/cv/GNUmakefile
endif

ifneq ($(filter $(cores),gb),)
  include $(higan.path)/gb/GNUmakefile
endif

ifneq ($(filter $(cores),gba),)
  include $(higan.path)/gba/GNUmakefile
endif

ifneq ($(filter $(cores),ws),)
  include $(higan.path)/ws/GNUmakefile
endif

ifneq ($(filter $(cores),ngp),)
  include $(higan.path)/ngp/GNUmakefile
endif

include $(higan.path)/component/GNUmakefile

flags += $(foreach c,$(call strupper,$(cores)),-DCORE_$c)

higan.objects := $(higan.objects:%=$(object.path)/%.o)
