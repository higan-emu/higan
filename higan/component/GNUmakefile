components := $(call unique,$(components))

objects += $(if $(findstring arm7tdmi,$(components)),processor-arm7tdmi)
objects += $(if $(findstring gsu,$(components)),processor-gsu)
objects += $(if $(findstring hg51b,$(components)),processor-hg51b)
objects += $(if $(findstring huc6280,$(components)),processor-huc6280)
objects += $(if $(findstring m68k,$(components)),processor-m68k)
objects += $(if $(findstring mos6502,$(components)),processor-mos6502)
objects += $(if $(findstring sm83,$(components)),processor-sm83)
objects += $(if $(findstring spc700,$(components)),processor-spc700)
objects += $(if $(findstring tlcs900h,$(components)),processor-tlcs900h)
objects += $(if $(findstring upd96050,$(components)),processor-upd96050)
objects += $(if $(findstring v30mz,$(components)),processor-v30mz)
objects += $(if $(findstring wdc65816,$(components)),processor-wdc65816)
objects += $(if $(findstring z80,$(components)),processor-z80)

objects += $(if $(findstring sn76489,$(components)),audio-sn76489)

objects += $(if $(findstring tms9918,$(components)),video-tms9918)

obj/processor-arm7tdmi.o: component/processor/arm7tdmi/arm7tdmi.cpp
obj/processor-gsu.o:      component/processor/gsu/gsu.cpp
obj/processor-hg51b.o:    component/processor/hg51b/hg51b.cpp
obj/processor-huc6280.o:  component/processor/huc6280/huc6280.cpp
obj/processor-m68k.o:     component/processor/m68k/m68k.cpp
obj/processor-mos6502.o:  component/processor/mos6502/mos6502.cpp
obj/processor-sm83.o:     component/processor/sm83/sm83.cpp
obj/processor-spc700.o:   component/processor/spc700/spc700.cpp
obj/processor-tlcs900h.o: component/processor/tlcs900h/tlcs900h.cpp
obj/processor-upd96050.o: component/processor/upd96050/upd96050.cpp
obj/processor-v30mz.o:    component/processor/v30mz/v30mz.cpp
obj/processor-wdc65816.o: component/processor/wdc65816/wdc65816.cpp
obj/processor-z80.o:      component/processor/z80/z80.cpp

obj/audio-sn76489.o:      component/audio/sn76489/sn76489.cpp

obj/audio-tms9918.o:      component/video/tms9918/tms9918.cpp
