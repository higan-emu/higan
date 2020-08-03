higan.components += z80 tms9918 sn76489

higan.objects += higan-sg-interface
higan.objects += higan-sg-cpu higan-sg-vdp higan-sg-psg
higan.objects += higan-sg-system higan-sg-cartridge
higan.objects += higan-sg-controller

$(object.path)/higan-sg-interface.o:  $(higan.path)/sg/interface/interface.cpp
$(object.path)/higan-sg-cpu.o:        $(higan.path)/sg/cpu/cpu.cpp
$(object.path)/higan-sg-vdp.o:        $(higan.path)/sg/vdp/vdp.cpp
$(object.path)/higan-sg-psg.o:        $(higan.path)/sg/psg/psg.cpp
$(object.path)/higan-sg-system.o:     $(higan.path)/sg/system/system.cpp
$(object.path)/higan-sg-cartridge.o:  $(higan.path)/sg/cartridge/cartridge.cpp
$(object.path)/higan-sg-controller.o: $(higan.path)/sg/controller/controller.cpp
