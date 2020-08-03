higan.components := $(call unique,$(higan.components))

higan.objects += $(if $(findstring ay38910,$(higan.components)),higan-audio-ay38910)
higan.objects += $(if $(findstring msm5205,$(higan.components)),higan-audio-msm5205)
higan.objects += $(if $(findstring sn76489,$(higan.components)),higan-audio-sn76489)
higan.objects += $(if $(findstring t6w28,$(higan.components)),higan-audio-t6w28)
higan.objects += $(if $(findstring ym2149,$(higan.components)),higan-audio-ym2149)
higan.objects += $(if $(findstring ym2413,$(higan.components)),higan-audio-ym2413)

higan.objects += $(if $(findstring m93lcx6,$(higan.components)),higan-eeprom-m93lcx6)
higan.objects += $(if $(findstring x24c01,$(higan.components)),higan-eeprom-x24c01)

higan.objects += $(if $(findstring arm7tdmi,$(higan.components)),higan-processor-arm7tdmi)
higan.objects += $(if $(findstring gsu,$(higan.components)),higan-processor-gsu)
higan.objects += $(if $(findstring hg51b,$(higan.components)),higan-processor-hg51b)
higan.objects += $(if $(findstring huc6280,$(higan.components)),higan-processor-huc6280)
higan.objects += $(if $(findstring m68k,$(higan.components)),higan-processor-m68k)
higan.objects += $(if $(findstring mos6502,$(higan.components)),higan-processor-mos6502)
higan.objects += $(if $(findstring sm83,$(higan.components)),higan-processor-sm83)
higan.objects += $(if $(findstring spc700,$(higan.components)),higan-processor-spc700)
higan.objects += $(if $(findstring tlcs900h,$(higan.components)),higan-processor-tlcs900h)
higan.objects += $(if $(findstring upd96050,$(higan.components)),higan-processor-upd96050)
higan.objects += $(if $(findstring v30mz,$(higan.components)),higan-processor-v30mz)
higan.objects += $(if $(findstring wdc65816,$(higan.components)),higan-processor-wdc65816)
higan.objects += $(if $(findstring z80,$(higan.components)),higan-processor-z80)

higan.objects += $(if $(findstring tms9918,$(higan.components)),higan-video-tms9918)
higan.objects += $(if $(findstring v9938,$(higan.components)),higan-video-v9938)

$(object.path)/higan-audio-ay38910.o:      $(higan.path)/component/audio/ay38910/ay38910.cpp
$(object.path)/higan-audio-msm5205.o:      $(higan.path)/component/audio/msm5205/msm5205.cpp
$(object.path)/higan-audio-sn76489.o:      $(higan.path)/component/audio/sn76489/sn76489.cpp
$(object.path)/higan-audio-t6w28.o:        $(higan.path)/component/audio/t6w28/t6w28.cpp
$(object.path)/higan-audio-ym2149.o:       $(higan.path)/component/audio/ym2149/ym2149.cpp
$(object.path)/higan-audio-ym2413.o:       $(higan.path)/component/audio/ym2413/ym2413.cpp

$(object.path)/higan-eeprom-m93lcx6.o:     $(higan.path)/component/eeprom/m93lcx6/m93lcx6.cpp
$(object.path)/higan-eeprom-x24c01.o:      $(higan.path)/component/eeprom/x24c01/x24c01.cpp

$(object.path)/higan-processor-arm7tdmi.o: $(higan.path)/component/processor/arm7tdmi/arm7tdmi.cpp
$(object.path)/higan-processor-gsu.o:      $(higan.path)/component/processor/gsu/gsu.cpp
$(object.path)/higan-processor-hg51b.o:    $(higan.path)/component/processor/hg51b/hg51b.cpp
$(object.path)/higan-processor-huc6280.o:  $(higan.path)/component/processor/huc6280/huc6280.cpp
$(object.path)/higan-processor-m68k.o:     $(higan.path)/component/processor/m68k/m68k.cpp
$(object.path)/higan-processor-mos6502.o:  $(higan.path)/component/processor/mos6502/mos6502.cpp
$(object.path)/higan-processor-sm83.o:     $(higan.path)/component/processor/sm83/sm83.cpp
$(object.path)/higan-processor-spc700.o:   $(higan.path)/component/processor/spc700/spc700.cpp
$(object.path)/higan-processor-tlcs900h.o: $(higan.path)/component/processor/tlcs900h/tlcs900h.cpp
$(object.path)/higan-processor-upd96050.o: $(higan.path)/component/processor/upd96050/upd96050.cpp
$(object.path)/higan-processor-v30mz.o:    $(higan.path)/component/processor/v30mz/v30mz.cpp
$(object.path)/higan-processor-wdc65816.o: $(higan.path)/component/processor/wdc65816/wdc65816.cpp
$(object.path)/higan-processor-z80.o:      $(higan.path)/component/processor/z80/z80.cpp

$(object.path)/higan-video-tms9918.o:      $(higan.path)/component/video/tms9918/tms9918.cpp
$(object.path)/higan-video-v9938.o:        $(higan.path)/component/video/v9938/v9938.cpp
