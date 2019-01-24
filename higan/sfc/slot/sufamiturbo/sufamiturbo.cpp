SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;
#include "serialization.cpp"

auto SufamiTurboCartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

auto SufamiTurboCartridge::power() -> void {
}
