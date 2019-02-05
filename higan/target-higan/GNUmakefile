name := higan

hiro.path := ../hiro
hiro.resource := $(ui)/resource/higan.rc
include $(hiro.path)/GNUmakefile

ruby.path := ../ruby
include $(ruby.path)/GNUmakefile

objects += ui-higan ui-resource ui-emulator ui-input
objects += ui-configurations ui-system ui-viewport
objects := $(objects:%=obj/%.o)

obj/ui-higan.o: $(ui)/higan.cpp
obj/ui-resource.o: $(ui)/resource/resource.cpp
obj/ui-emulator.o: $(ui)/emulator/emulator.cpp
obj/ui-input.o: $(ui)/input/input.cpp
obj/ui-configurations.o: $(ui)/configurations/configurations.cpp
obj/ui-system.o: $(ui)/system/system.cpp
obj/ui-viewport.o: $(ui)/viewport/viewport.cpp

all: $(hiro.objects) $(ruby.objects) $(objects)
	$(info Linking out/$(name) ...)
	+@$(compiler) -o out/$(name) $(hiro.objects) $(ruby.objects) $(objects) $(hiro.options) $(ruby.options) $(options)
ifeq ($(platform),macos)
	rm -rf out/$(name).app
	mkdir -p out/$(name).app/Contents/MacOS/
	mkdir -p out/$(name).app/Contents/Resources/
	mv out/$(name) out/$(name).app/Contents/MacOS/$(name)
	cp $(ui)/resource/$(name).plist out/$(name).app/Contents/Info.plist
	sips -s format icns $(ui)/resource/$(name).png --out out/$(name).app/Contents/Resources/$(name).icns
endif

verbose: hiro.verbose ruby.verbose nall.verbose all;

install: all
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macos)
	mkdir -p ~/Library/Application\ Support\$(name)/
	mkdir -p ~/Library/Application\ Support\$(name)/Systems/
	cp -R out/$(name).app /Applications/$(name).app
	cp -R Systems/* ~/Library/Application\ Support/$(name)/Systems/
else ifneq ($(filter $(platform),linux bsd),)
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/applications/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/share/$(name)/
	cp out/$(name) $(prefix)/bin/$(name)
	cp -R System/* $(prefix)/share/$(name)/
	cp $(ui)/resource/$(name).desktop $(prefix)/share/applications/$(name).desktop
	cp $(ui)/resource/$(name).png $(prefix)/share/icons/$(name).png
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macos)
	rm -rf /Applications/$(name).app
else ifneq ($(filter $(platform),linux bsd),)
	rm -f $(prefix)/bin/$(name)
	rm -f $(prefix)/share/applications/$(name).desktop
	rm -f $(prefix)/share/icons/$(name).png
endif
