#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

VPU vpu;
#include "memory.cpp"
#include "serialization.cpp"

auto VPU::main() -> void {
  for(uint hclock : range(480)) {
    io.hcounter++;
    step(1);
  }
  if(io.vcounter <= 150) {
    io.hblankActive = 1;
    cpu.setInterruptHblank(io.hblankEnableIRQ);
  }
  for(uint hclock : range(35)) {
    io.hcounter++;
    step(1);
  }

  io.hcounter = 0;
  io.hblankActive = 0;
  cpu.setInterruptHblank(0);

  io.vcounter++;
  if(io.vcounter == 152) {
    io.vblankActive = 1;
    cpu.setInterruptVblank(io.vblankEnableIRQ);
    scheduler.exit(Scheduler::Event::Frame);
  }
  if(io.vcounter == 198) {
    io.hblankActive = 1;
    cpu.setInterruptHblank(io.hblankEnableIRQ);
  }
  if(io.vcounter == 199) {
    io.vcounter = 0;
    io.vblankActive = 0;
    io.characterOver = 0;
    cpu.setInterruptVblank(0);
  }
}

auto VPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VPU::refresh() -> void {
  for(uint address : range(0x1000)) buffer[address] = scrollRAM[address];
  for(uint address : range(0x2000)) buffer[address + 0x1000] = characterRAM[address];
  for(uint address : range(0x2f00)) buffer[address + 0x3000] = cpu.ram[address + 0x100];
  display.screen->refresh(buffer, 160 * sizeof(uint32), 160, 152);
}

auto VPU::power() -> void {
  Thread::create(system.frequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
  spriteRAM.allocate(0x100);
  scrollRAM.allocate(0x1000);
  characterRAM.allocate(0x2000);
  io = {};
}

}
