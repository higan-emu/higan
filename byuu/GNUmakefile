name := byuu
build := performance
openmp := false
local := true
flags += -I. -I.. -I../higan -DMIA_LIBRARY

ifeq ($(local),true)
  flags += -march=native
endif

nall.path := ../nall
include $(nall.path)/GNUmakefile

libco.path := ../libco
include $(libco.path)/GNUmakefile

ruby.path := ../ruby
include $(ruby.path)/GNUmakefile

hiro.path := ../hiro
hiro.resource := resource/byuu.rc
include $(hiro.path)/GNUmakefile

profile := performance
cores := fc sfc n64 sg ms md ps1 pce msx gb gba ws ngp

higan.path := ../higan
include $(higan.path)/GNUmakefile

icarus.path := ../icarus

icarus.objects := icarus icarus-resource
icarus.objects := $(icarus.objects:%=$(object.path)/%.o)

$(object.path)/icarus.o: $(icarus.path)/icarus.cpp
$(object.path)/icarus-resource.o: $(icarus.path)/resource/resource.cpp

byuu.path = ../byuu

byuu.objects += byuu byuu-resource byuu-emulator byuu-program byuu-input
byuu.objects += byuu-presentation byuu-settings byuu-tools
byuu.objects := $(byuu.objects:%=$(object.path)/%.o)

$(object.path)/byuu.o: $(byuu.path)/byuu.cpp
$(object.path)/byuu-resource.o: $(byuu.path)/resource/resource.cpp
$(object.path)/byuu-emulator.o: $(byuu.path)/emulator/emulator.cpp
$(object.path)/byuu-program.o: $(byuu.path)/program/program.cpp
$(object.path)/byuu-input.o: $(byuu.path)/input/input.cpp
$(object.path)/byuu-presentation.o: $(byuu.path)/presentation/presentation.cpp
$(object.path)/byuu-settings.o: $(byuu.path)/settings/settings.cpp
$(object.path)/byuu-tools.o: $(byuu.path)/tools/tools.cpp

all.objects := $(libco.objects) $(ruby.objects) $(hiro.objects) $(higan.objects) $(icarus.objects) $(byuu.objects)
all.options := $(libco.options) $(ruby.options) $(hiro.options) $(higan.options) $(icarus.options) $(byuu.options) $(options)

all: $(all.objects)
	$(info Linking $(output.path)/$(name) ...)
	+@$(compiler) -o $(output.path)/$(name) $(all.objects) $(all.options)
ifeq ($(platform),macos)
	rm -rf $(output.path)/$(name).app
	mkdir -p $(output.path)/$(name).app/Contents/MacOS/
	mkdir -p $(output.path)/$(name).app/Contents/Resources/
	mv $(output.path)/$(name) $(output.path)/$(name).app/Contents/MacOS/$(name)
	cp resource/$(name).plist $(output.path)/$(name).app/Contents/Info.plist
	sips -s format icns resource/$(name).png --out $(output.path)/$(name).app/Contents/Resources/$(name).icns
endif

verbose: nall.verbose ruby.verbose hiro.verbose all;

clean:
ifeq ($(platform),macos)
	rm -rf $(output.path)/$(name).app
endif
	$(call delete,$(object.path)/*)
	$(call delete,$(output.path)/*)

install: all
ifeq ($(platform),windows)
	mkdir -p $(prefix)/$(name)/
else ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),macos)
	mkdir -p ~/Library/Application\ Support/$(name)/
	cp -R $(output.path)/$(name).app /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	cp $(output.path)/$(name) $(prefix)/bin/$(name)
	cp resource/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp resource/$(name).png $(prefix)/share/icons/$(name).png
endif

uninstall:
ifeq ($(platform),windows)
else ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),macos)
	rm -rf /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	rm -f $(prefix)/bin/$(name)
	rm -f $(prefix)/share/applications/$(name).desktop
	rm -f $(prefix)/share/icons/$(name).png
endif

-include $(object.path)/*.d
