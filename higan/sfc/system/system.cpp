#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

System system;
Display display;
Speakers speakers;
Hacks hacks;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();

  auto reset = resetButton->value;
  platform->input(resetButton);
  if(!reset && resetButton->value) power(true);
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(smp);
  scheduler.synchronize(ppu);
  scheduler.synchronize(dsp);
  for(auto coprocessor : cpu.coprocessors) scheduler.synchronize(*coprocessor);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::load(Node::Object from) -> void {
  if(root) {
    save();
    unload();
    bus.reset();
  }

  root = Node::System::create("Super Famicom");
  root->load(from);

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  root->append(resetButton);

  hacks.load(root, from);
  cartridge.load(root, from);
  controllerPort1.load(root, from);
  controllerPort2.load(root, from);
  expansionPort.load(root, from);
  display.load(root, from);
  speakers.load(root, from);
  cpu.load(root, from);
  ppu.load(root, from);

  information = {};

  //these values cannot change at run-time
  hacks.ppu.fast->setLatch();
  hacks.ppu.noSpriteLimit->setLatch();
  hacks.ppu.hiresMode7->setLatch();
  hacks.dsp.fast->setLatch();
  hacks.coprocessors.fast->setLatch();

  if(cartridge.region() == "NTSC") {
    information.region = Region::NTSC;
    information.cpuFrequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.cpuFrequency = Constants::Colorburst::PAL * 4.8;
  }

  serializeInit();
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  controllerPort1.disconnect();
  controllerPort2.disconnect();
  expansionPort.disconnect();
  cartridge.disconnect();
}

auto System::power(bool reset) -> void {
  video.reset(interface);
  video.setPalette(display.node);
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  scheduler.reset();
  cpu.power(reset);
  smp.power(reset);
  dsp.power(reset);
  ppu.power(reset);
  cartridge.power(reset);

  scheduler.primary(cpu);
}

auto Display::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Video::create("Display"));
  node->type   = "CRT";
  node->width  = 512;
  node->height = 480;
  node->aspect = 8.0 / 7.0;
  node->colors = 1 << 19;
  node->color  = [&](auto index) -> uint64_t { return color(index); };
  node->append(colorEmulation = Node::Boolean::create("Color Emulation", true, [&](auto) {
    video.setPalette(display.node);
  }));
  node->append(colorBleed = Node::Boolean::create("Color Bleed", true, [&](auto value) {
    video.setEffect(higan::Video::Effect::ColorBleed, value);
  }));
  Node::load(node, from);
}

auto Display::color(natural color) -> uint64_t {
  uint r = color.bits( 0, 4);
  uint g = color.bits( 5, 9);
  uint b = color.bits(10,14);
  uint l = color.bits(15,18);

  //luma=0 is not 100% black; but it's much darker than normal linear scaling
  //exact effect seems to be analog; requires > 24-bit color depth to represent accurately
  double L = (1.0 + l) / 16.0 * (l ? 1.0 : 0.25);
  uint64 R = L * image::normalize(r, 5, 16);
  uint64 G = L * image::normalize(g, 5, 16);
  uint64 B = L * image::normalize(b, 5, 16);

  if(display.colorEmulation->value()) {
    static const uint8 gammaRamp[32] = {
      0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
      0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
      0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
      0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
    };
    R = L * gammaRamp[r] * 0x0101;
    G = L * gammaRamp[g] * 0x0101;
    B = L * gammaRamp[b] * 0x0101;
  }

  return R << 32 | G << 16 | B << 0;
}

auto Speakers::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Audio::create("Speakers"));
  Node::load(node, from);
}

auto Hacks::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Settings::create("Hacks"));
  from = Node::load(node, from);

  node->append(ppu.fast = Node::Boolean::create("Fast PPU", false));
  Node::load(ppu.fast, from);

  node->append(ppu.noSpriteLimit = Node::Boolean::create("No Sprite Limit", false));
  Node::load(ppu.noSpriteLimit, from);

  node->append(ppu.hiresMode7 = Node::Boolean::create("Hires Mode 7", false));
  Node::load(ppu.hiresMode7, from);

  node->append(dsp.fast = Node::Boolean::create("Fast DSP", false));
  Node::load(dsp.fast, from);

  node->append(coprocessors.fast = Node::Boolean::create("Fast Coprocessors", false));
  Node::load(coprocessors.fast, from);
}

}
