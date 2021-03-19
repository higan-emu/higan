#include <md/md.hpp>

namespace higan::MegaDrive {

APU apu;
#include "bus.cpp"
#include "debugger.cpp"
#include "serialization.cpp"

auto APU::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("APU");

  debugger.load(node);
}

auto APU::unload() -> void {
  debugger = {};
  node = {};
}

auto APU::main() -> void {
  updateBus();
  if(!running()) {
    return step(1);
  }

  if(state.nmiLine) {
    state.nmiLine = 0;  //edge-sensitive
    debugger.interrupt("NMI");
    irq(0, 0x0066, 0xff);
  }

  if(state.intLine) {
    //level-sensitive
    debugger.interrupt("IRQ");
    irq(1, 0x0038, 0xff);
  }

  debugger.instruction();
  instruction();
}

auto APU::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu, vdp, psg, ym2612);
}

auto APU::setNMI(uint1 value) -> void {
  state.nmiLine = value;
}

auto APU::setINT(uint1 value) -> void {
  state.intLine = value;
}

auto APU::setRES(uint1 value) -> void {
  if(!value && arbstate.resetLine) {
    power(true);
  }
  arbstate.resetLine = value;
}

auto APU::setBREQ(uint1 value) -> void {
  arbstate.busreqLine = value;
}

auto APU::updateBus() -> void {
  if(!arbstate.resetLine) return; // Z80 bus switch may be blocked by reset
  if(arbstate.busreqLine && !arbstate.busreqLatch) {
    step(9); // estimated minimum wait time to allow 68K to read back unavailable bus status (Fatal Rewind)
  }
  arbstate.busreqLatch = arbstate.busreqLine;
}

auto APU::power(bool reset) -> void {
  Z80::bus = this;
  Z80::power();
  ym2612.power(reset);
  //bus->grant(false);
  Thread::create(system.frequency() / 15.0, {&APU::main, this});
  if(!reset) {
    ram.allocate(8_KiB);
    arbstate = {};
  }
  state = {};
  io = {};
}

}

