#include <ws/ws.hpp>

namespace higan::WonderSwan {

InternalRAM iram;
Bus bus;

auto InternalRAM::power() -> void {
  for(auto& byte : memory) byte = 0x00;
}

auto InternalRAM::serialize(serializer& s) -> void {
  s.array(memory, Model::WonderSwan() || Model::PocketChallengeV2() ? 0x4000 : 0x10000);
}

auto InternalRAM::read(uint16 address) -> uint8 {
  if(address >= 0x4000 && !system.color()) return 0x90;
  return memory[address];
}

auto InternalRAM::write(uint16 address, uint8 data) -> void {
  if(address >= 0x4000 && !system.color()) return;
  memory[address] = data;
}

auto Bus::power() -> void {
  for(auto& io : port) io = nullptr;
}

auto Bus::read(uint20 address) -> uint8 {
  uint4 bank = address.bit(16,19);
  uint1 bootup = !cpu.r.cartridgeEnable;
  if(bank == 0x0) return iram.read(address);
  if(bank == 0x1) return cartridge.ramRead(address);
  if(bank <= 0xe) return cartridge.romRead(address);
  if(bootup == 1) return system.bootROM.read(address);
  if(bank == 0xf) return cartridge.romRead(address);
  return 0x00;
}

auto Bus::write(uint20 address, uint8 data) -> void {
//if(address==0x0626) print("* w0626=",hex(data),"\n");
  uint4 bank = address.bit(16,19);
  uint1 bootup = !cpu.r.cartridgeEnable;
  if(bank == 0x0) return iram.write(address, data);
  if(bank == 0x1) return cartridge.ramWrite(address, data);
  if(bank <= 0xe) return cartridge.romWrite(address, data);
  if(bootup == 1) return system.bootROM.write(address, data);
  if(bank == 0xf) return cartridge.romWrite(address, data);
}

auto Bus::map(IO* io, uint16_t lo, maybe<uint16_t> hi) -> void {
  for(uint address = lo; address <= (hi ? hi() : lo); address++) port[address] = io;
}

auto Bus::portRead(uint16 address) -> uint8 {
  if(auto io = port[address]) return io->portRead(address);
  return 0x00;
}

auto Bus::portWrite(uint16 address, uint8 data) -> void {
  if(auto io = port[address]) return io->portWrite(address, data);
}

}
