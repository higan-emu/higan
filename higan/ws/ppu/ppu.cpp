#include <ws/ws.hpp>

namespace higan::WonderSwan {

PPU ppu;
#include "io.cpp"
#include "latch.cpp"
#include "render.cpp"
#include "serialization.cpp"

auto PPU::load(Node::Object parent, Node::Object from) -> void {
  screen.create(system.video.node);
}

auto PPU::unload() -> void {
  screen.destroy();
}

auto PPU::main() -> void {
  if(s.vtime == 142) {
    latchOAM();
  }

  if(s.vtime < 144) {
    uint y = s.vtime % (r.vtotal + 1);
    auto output = this->output + y * (224 + 13);
    latchRegisters();
    latchSprites(y);
    for(uint x : range(224)) {
      s.pixel = {Pixel::Source::Back, 0x000};
      if(r.lcdEnable) {
        renderBack();
        if(l.screenOneEnable) renderScreenOne(x, y);
        if(l.screenTwoEnable) renderScreenTwo(x, y);
        if(l.spriteEnable) renderSprite(x, y);
      }
      if(Model::WonderSwanColor() && r.lcdContrast == 1) {
        auto b = s.pixel.color.bit(0, 3);
        auto g = s.pixel.color.bit(4, 7);
        auto r = s.pixel.color.bit(8,11);
        //this is just a rough approximation, not accurate to the actual effect.
        //note that b,g,r are BitRange references, and modify the underlying s.pixel.color variable.
        b = min(15, b * 1.5);
        g = min(15, g * 1.5);
        r = min(15, r * 1.5);
      }
      *output++ = s.pixel.color;
      step(1);
    }
    step(32);
  } else {
    step(256);
  }
  scanline();
  if(r.htimerEnable && r.htimerCounter < r.htimerFrequency) {
    if(++r.htimerCounter == r.htimerFrequency) {
      if(r.htimerRepeat) {
        r.htimerCounter = 0;
      } else {
        r.htimerEnable = false;
      }
      cpu.raise(CPU::Interrupt::HblankTimer);
    }
  }
}

//vtotal+1 = scanlines per frame
//vtotal<143 inhibits vblank and repeats the screen image until vtime=144
//todo: unknown how votal<143 interferes with line compare interrupts
auto PPU::scanline() -> void {
  s.vtime++;
  if(s.vtime >= max(144, r.vtotal + 1)) return frame();
  if(s.vtime == r.lineCompare) cpu.raise(CPU::Interrupt::LineCompare);
  if(s.vtime == 144) {
    cpu.raise(CPU::Interrupt::Vblank);
    if(r.vtimerEnable && r.vtimerCounter < r.vtimerFrequency) {
      if(++r.vtimerCounter == r.vtimerFrequency) {
        if(r.vtimerRepeat) {
          r.vtimerCounter = 0;
        } else {
          r.vtimerEnable = false;
        }
        cpu.raise(CPU::Interrupt::VblankTimer);
      }
    }
  }
}

auto PPU::frame() -> void {
  s.field = !s.field;
  s.vtime = 0;
  scheduler.exit(Event::Frame);
}

auto PPU::refresh() -> void {
  uint width  = system.video.node->width;
  uint height = system.video.node->height;
  screen.refresh(output, (224 + 13) * sizeof(uint32), width, height);
}

auto PPU::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto PPU::power() -> void {
  Thread::create(3'072'000, {&PPU::main, this});

  screen.append(icon.auxiliary0.create  (13, 13).setPixels(Resource::Sprite::WonderSwan::Auxiliary0));
  screen.append(icon.auxiliary1.create  (13, 13).setPixels(Resource::Sprite::WonderSwan::Auxiliary1));
  screen.append(icon.auxiliary2.create  (13, 13).setPixels(Resource::Sprite::WonderSwan::Auxiliary2));
  screen.append(icon.headphones.create  (13, 13).setPixels(Resource::Sprite::WonderSwan::Headphones));
  screen.append(icon.initialized.create (13, 13).setPixels(Resource::Sprite::WonderSwan::Initialized));
  screen.append(icon.lowBattery.create  (13, 13).setPixels(Resource::Sprite::WonderSwan::LowBattery));
  screen.append(icon.orientation0.create(13, 13).setPixels(Resource::Sprite::WonderSwan::Orientation0));
  screen.append(icon.orientation1.create(13, 13).setPixels(Resource::Sprite::WonderSwan::Orientation1));
  screen.append(icon.poweredOn.create   (13, 13).setPixels(Resource::Sprite::WonderSwan::PoweredOn));
  screen.append(icon.sleeping.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::Sleeping));
  screen.append(icon.volumeA0.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeA0));
  screen.append(icon.volumeA1.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeA1));
  screen.append(icon.volumeA2.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeA2));
  screen.append(icon.volumeB0.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeB0));
  screen.append(icon.volumeB1.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeB1));
  screen.append(icon.volumeB2.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeB2));
  screen.append(icon.volumeB3.create    (13, 13).setPixels(Resource::Sprite::WonderSwan::VolumeB3));

  if(Model::WonderSwan()) {
    icon.poweredOn.setPosition   (  0, 144).invert();
    icon.initialized.setPosition ( 13, 144).invert();
    icon.sleeping.setPosition    ( 26, 144).invert();
    icon.lowBattery.setPosition  ( 39, 144).invert();
    icon.volumeA2.setPosition    ( 52, 144).invert();
    icon.volumeA1.setPosition    ( 52, 144).invert();
    icon.volumeA0.setPosition    ( 52, 144).invert();
    icon.headphones.setPosition  ( 65, 144).invert();
    icon.orientation1.setPosition( 78, 144).invert();
    icon.orientation0.setPosition( 91, 144).invert();
    icon.auxiliary2.setPosition  (104, 144).invert();
    icon.auxiliary1.setPosition  (117, 144).invert();
    icon.auxiliary0.setPosition  (130, 144).invert();
  }

  if(Model::WonderSwanColor() || Model::SwanCrystal() || Model::MamaMitte()) {
    icon.auxiliary0.setPosition  (224,   0);
    icon.auxiliary1.setPosition  (224,  13);
    icon.auxiliary2.setPosition  (224,  26);
    icon.orientation0.setPosition(224,  39);
    icon.orientation1.setPosition(224,  52);
    icon.headphones.setPosition  (224,  65);
    icon.volumeB0.setPosition    (224,  78);
    icon.volumeB1.setPosition    (224,  78);
    icon.volumeB2.setPosition    (224,  78);
    icon.volumeB3.setPosition    (224,  78);
    icon.lowBattery.setPosition  (224,  91);
    icon.sleeping.setPosition    (224, 104);
    icon.initialized.setPosition (224, 117);
    icon.poweredOn.setPosition   (224, 130);
  }

  bus.map(this, 0x0000, 0x0017);
  bus.map(this, 0x001c, 0x003f);
  bus.map(this, 0x00a2);
  bus.map(this, 0x00a4, 0x00ab);

  auto color = SoC::ASWAN() ? 0xfff : 0x000;
  for(auto& pixel : output) pixel = color;
  s = {};
  l = {};
  r = {};
  updateIcons();
}

auto PPU::updateIcons() -> void {
  if(Model::PocketChallengeV2()) return;

  bool visible = system.video.showIcons->value();

  icon.poweredOn.setVisible(visible);

  icon.sleeping.setVisible(r.icon.sleeping & visible);
  icon.orientation1.setVisible(r.icon.orientation1 & visible);
  icon.orientation0.setVisible(r.icon.orientation0 & visible);
  icon.auxiliary0.setVisible(r.icon.auxiliary0 & visible);
  icon.auxiliary1.setVisible(r.icon.auxiliary1 & visible);
  icon.auxiliary2.setVisible(r.icon.auxiliary2 & visible);

  auto volume = apu.r.masterVolume;

  if(Model::WonderSwan()) {
    icon.volumeA0.setVisible(volume == 0 & visible);
    icon.volumeA1.setVisible(volume == 1 & visible);
    icon.volumeA2.setVisible(volume == 2 & visible);
  }

  if(Model::WonderSwanColor() || Model::SwanCrystal() || Model::MamaMitte()) {
    icon.volumeB0.setVisible(volume == 0 & visible);
    icon.volumeB1.setVisible(volume == 1 & visible);
    icon.volumeB2.setVisible(volume == 2 & visible);
    icon.volumeB3.setVisible(volume == 3 & visible);
  }

  auto headphones = apu.r.headphonesConnected;

  icon.headphones.setVisible(headphones & visible);
}

}
