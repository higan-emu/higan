auto CPU::Debugger::load(Node::Object parent) -> void {
  tracer.instruction = parent->append<Node::Instruction>("Instruction", "CPU");
  tracer.instruction->setAddressBits(32);

  tracer.exception = parent->append<Node::Notification>("Exception", "CPU");
  tracer.interrupt = parent->append<Node::Notification>("Interrupt", "CPU");
  tracer.message = parent->append<Node::Notification>("Message", "CPU");
  tracer.function = parent->append<Node::Notification>("Function", "CPU");

  memory.ram = parent->append<Node::Memory>("CPU RAM");
  memory.ram->setSize(cpu.ram.size);
  memory.ram->setRead([&](uint32 address) -> uint8 {
    return cpu.ram.readByte(address);
  });
  memory.ram->setWrite([&](uint32 address, uint8 data) -> void {
    return cpu.ram.writeByte(address, data);
  });

  memory.dcache = parent->append<Node::Memory>("CPU Data Cache");
  memory.dcache->setSize(cpu.dcache.size);
  memory.dcache->setRead([&](uint32 address) -> uint8 {
    return cpu.dcache.readByte(address);
  });
  memory.dcache->setWrite([&](uint32 address, uint8 data) -> void {
    return cpu.dcache.writeByte(address, data);
  });

  memory.icache = parent->append<Node::Memory>("CPU Instruction Cache");
  memory.icache->setSize(cpu.icache.size);
  memory.icache->setRead([&](uint32 address) -> uint8 {
    return cpu.icache.readByte(address);
  });
  memory.icache->setWrite([&](uint32 address, uint8 data) -> void {
    return cpu.icache.writeByte(address, data);
  });
}

auto CPU::Debugger::instruction() -> void {
  if(tracer.instruction->enabled()) {
    u32 address = cpu.pipeline.address;
    u32 instruction = cpu.pipeline.instruction;
    if(tracer.instruction->address(address)) {
      cpu.disassembler.showColors = 0;
      tracer.instruction->notify(cpu.disassembler.disassemble(address, instruction), {});
      cpu.disassembler.showColors = 1;
    }
  }
}

auto CPU::Debugger::exception(string_view type) -> void {
  if(tracer.exception->enabled()) {
    tracer.exception->notify(type);
  }
}

auto CPU::Debugger::interrupt(string_view type) -> void {
  if(tracer.interrupt->enabled()) {
    tracer.interrupt->notify(type);
  }
}

auto CPU::Debugger::message(char c) -> void {
  static string message;
  if(tracer.message->enabled()) {
    if(c == '\n') {
      if(message) {
        tracer.message->notify(message);
        message = {};
      }
    } else if(message.size() < 200) {
      message.append(c);
    }
  }
}

auto CPU::Debugger::function() -> void {
  u32 pc = cpu.core.pc;
  u32 fn = cpu.core.r[9];
  u32 p0 = cpu.core.r[4];
  u32 p1 = cpu.core.r[5];
  u32 p2 = cpu.core.r[6];
  u32 p3 = cpu.core.r[7];
  string call;

  if(pc == 0xa0) {
    if(fn == 0x00) call = {"FileOpen(", hex(p0, 8L), ", ", hex(p1, 8L), ")"};
  }

  if(pc == 0xb0) {
    if(fn == 0x32) call = {"FileOpen(", hex(p0, 8L), ", ", hex(p1, 8L), ")"};
  }

  if(pc == 0xc0) {
  }

  if(call) {
    tracer.function->notify(call);
  }
}
