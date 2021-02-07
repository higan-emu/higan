name := higan
build := performance
openmp := false
local := true
flags += -I. -I.. -I../higan

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
hiro.resource := resource/higan.rc
include $(hiro.path)/GNUmakefile

cores := fc sfc sg ms md pce msx cv gb gba ws ngp

higan.path := ../higan
include $(higan.path)/GNUmakefile

higan-ui.path := ../higan-ui

higan-ui.objects += higan-ui higan-ui-resource higan-ui-emulator
higan-ui.objects += higan-ui-input higan-ui-menus higan-ui-program higan-ui-settings
higan-ui.objects := $(higan-ui.objects:%=$(object.path)/%.o)

$(object.path)/higan-ui.o: $(higan-ui.path)/higan-ui.cpp
$(object.path)/higan-ui-resource.o: $(higan-ui.path)/resource/resource.cpp
$(object.path)/higan-ui-emulator.o: $(higan-ui.path)/emulator/emulator.cpp
$(object.path)/higan-ui-input.o: $(higan-ui.path)/input/input.cpp
$(object.path)/higan-ui-menus.o: $(higan-ui.path)/menus/menus.cpp
$(object.path)/higan-ui-program.o: $(higan-ui.path)/program/program.cpp
$(object.path)/higan-ui-settings.o: $(higan-ui.path)/settings/settings.cpp

all.objects := $(libco.objects) $(ruby.objects) $(hiro.objects) $(higan.objects) $(higan-ui.objects)
all.options := $(libco.options) $(ruby.options) $(hiro.options) $(higan.options) $(higan-ui.options) $(options)

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
	cp -R $(higan.path)/System/* $(prefix)/$(name)/
else ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),macos)
	mkdir -p ~/Library/Application\ Support/$(name)/
	cp -R $(output.path)/$(name).app /Applications/$(name).app
	cp -R $(higan.path)/System/* ~/Library/Application\ Support/$(name)/
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	cp $(output.path)/$(name) $(prefix)/bin/$(name)
	cp -R $(higan.path)/System/* $(prefix)/share/$(name)/
	sed "/^Exec/s:$(name):$(prefix)/bin/$(name):" resource/$(name).desktop >$(prefix)/share/applications/$(name).desktop
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
