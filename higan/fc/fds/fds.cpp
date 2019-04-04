#include <fc/fc.hpp>

namespace higan::Famicom {

FDS fds;
#include "io.cpp"
#include "serialization.cpp"

auto FDS::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create("Disk Slot", "Disk");
  port->hotSwappable = true;
  port->allocate = [&] { return Node::Peripheral::create("Famicom Disk"); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
  Thread::create(system.frequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
  cpu.peripherals.append(this);
  power();
}

auto FDS::unload() -> void {
  cpu.peripherals.removeWhere() == this;
  Thread::destroy();
  port = {};
  sideA.reset();
  sideB.reset();
}

auto FDS::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create("Famicom Disk");
  node->load(with);

  information = {};

  if(auto fp = platform->open(node, "metadata.bml", File::Read, File::Required)) {
    information.metadata = fp->reads();
  } else return;

  auto document = BML::unserialize(information.metadata);

  if(auto fp = platform->open(node, "program.disk", File::Read, File::Required)) {
    if(fp->size() >= 65500 * 1) {
      sideA.allocate(65500);
      fp->read(sideA.data(), sideA.size());
    }
    if(fp->size() >= 65500 * 2) {
      sideB.allocate(65500);
      fp->read(sideB.data(), sideB.size());
    }
  }

  port->prepend(node);
}

auto FDS::disconnect() -> void {
  if(!node) return;
  node = {};
}

auto FDS::main() -> void {
  if(irq.onTimer && !--irq.period) {
    if(irq.repeat) irq.period = irq.counter;
    irq.onTimer = irq.repeat;
    irq.triggered = 1;
    cpu.irqLine(1);
  }

  step(1);
}

auto FDS::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto FDS::power() -> void {
  irq = {};
  io = {};
}

}
