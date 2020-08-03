higan.components += z80 tms9918 sn76489

higan.objects += higan-cv-interface
higan.objects += higan-cv-cpu higan-cv-vdp higan-cv-psg
higan.objects += higan-cv-system higan-cv-cartridge
higan.objects += higan-cv-controller

$(object.path)/higan-cv-interface.o:  $(higan.path)/cv/interface/interface.cpp
$(object.path)/higan-cv-cpu.o:        $(higan.path)/cv/cpu/cpu.cpp
$(object.path)/higan-cv-vdp.o:        $(higan.path)/cv/vdp/vdp.cpp
$(object.path)/higan-cv-psg.o:        $(higan.path)/cv/psg/psg.cpp
$(object.path)/higan-cv-system.o:     $(higan.path)/cv/system/system.cpp
$(object.path)/higan-cv-cartridge.o:  $(higan.path)/cv/cartridge/cartridge.cpp
$(object.path)/higan-cv-controller.o: $(higan.path)/cv/controller/controller.cpp
