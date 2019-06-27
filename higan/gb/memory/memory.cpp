#include <gb/gb.hpp>

namespace higan::GameBoy {

Unmapped unmapped;
Bus bus;

Memory::~Memory() {
  free();
}

auto Memory::operator[](uint addr) -> uint8& {
  return data[addr];
}

auto Memory::allocate(uint size_) -> void {
  free();
  size = size_;
  data = new uint8[size];
}

auto Memory::copy(const uint8* data_, unsigned size_) -> void {
  free();
  size = size_;
  data = new uint8[size];
  memcpy(data, data_, size);
}

auto Memory::free() -> void {
  if(data) {
    delete[] data;
    data = nullptr;
  }
}

//

auto Bus::read(uint cycle, uint16 address, uint8 data) -> uint8 {
  data &= apu.readIO(cycle, address, data);
  if(cycle == 2) data &= mmio[address]->readIO(address, data);
  return data;
}

auto Bus::write(uint cycle, uint16 address, uint8 data) -> void {
  apu.writeIO(cycle, address, data);
  if(cycle == 2) mmio[address]->writeIO(address, data);
}

auto Bus::read(uint16 address, uint8 data) -> uint8 {
  data &= read(0, address, data);
  data &= read(1, address, data);
  data &= read(2, address, data);
  data &= read(3, address, data);
  data &= read(4, address, data);
  return data;
}

auto Bus::write(uint16 address, uint8 data) -> void {
  write(0, address, data);
  write(1, address, data);
  write(2, address, data);
  write(3, address, data);
  write(4, address, data);
}

auto Bus::power() -> void {
  for(uint address : range(65536)) mmio[address] = &unmapped;
}

}
