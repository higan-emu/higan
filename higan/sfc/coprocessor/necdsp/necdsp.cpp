NECDSP necdsp;
#include "memory.cpp"
#include "serialization.cpp"

auto NECDSP::load(Node::Object parent, Node::Object from) -> void {
  logger.attach(tracer);
  tracer->setSource("nec");
  tracer->setAddressBits(24);  //14-bits; use 24-bits to match CPU bus width
}

auto NECDSP::unload() -> void {
  logger.detach(tracer);

  cpu.coprocessors.removeByValue(this);
  Thread::destroy();
}

auto NECDSP::main() -> void {
  if(tracer->enabled() && tracer->address(regs.pc)) {
    tracer->instruction(disassembleInstruction(), disassembleContext());
  }
  exec();

  Thread::step(1);
  Thread::synchronize(cpu);
}

auto NECDSP::power() -> void {
  uPD96050::power();
  Thread::create(Frequency, {&NECDSP::main, this});
  cpu.coprocessors.append(this);
}
