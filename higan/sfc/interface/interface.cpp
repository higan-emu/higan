#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

Interface* interface = nullptr;

auto SuperFamicomInterface::name() -> string {
  return "Super Famicom";
}

auto SuperFamicomInterface::colors() -> uint32 {
  return 1 << 19;
}

auto SuperFamicomInterface::color(uint32 color) -> uint64 {
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

  if(system.display.colorEmulation->value()) {
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

auto SuperFamicomInterface::hashes() -> vector<string> {
  return cartridge.hashes();
}

auto SuperFamicomInterface::manifests() -> vector<string> {
  return cartridge.manifests();
}

auto SuperFamicomInterface::titles() -> vector<string> {
  return cartridge.titles();
}

auto SuperFamicomInterface::load() -> bool {
  return system.load();
}

auto SuperFamicomInterface::load(uint slot) -> bool {
  return true;
}

auto SuperFamicomInterface::save() -> void {
  system.save();
}

auto SuperFamicomInterface::unload() -> void {
  save();
  system.unload();
}

auto SuperFamicomInterface::initialize(string configuration) -> void {
  interface = this;
  return system.initialize(configuration);
}

auto SuperFamicomInterface::terminate() -> void {
  return system.terminate();
}

auto SuperFamicomInterface::root() -> Node::Object {
  return system.root;
}

auto SuperFamicomInterface::power() -> void {
  system.power(/* reset = */ false);
}

auto SuperFamicomInterface::reset() -> void {
  system.power(/* reset = */ true);
}

auto SuperFamicomInterface::run() -> void {
  system.run();
}

auto SuperFamicomInterface::rtc() -> bool {
  if(cartridge.has.EpsonRTC) return true;
  if(cartridge.has.SharpRTC) return true;
  return false;
}

auto SuperFamicomInterface::synchronize(uint64 timestamp) -> void {
  if(!timestamp) timestamp = chrono::timestamp();
  if(cartridge.has.EpsonRTC) epsonrtc.synchronize(timestamp);
  if(cartridge.has.SharpRTC) sharprtc.synchronize(timestamp);
}

auto SuperFamicomInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto SuperFamicomInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto SuperFamicomInterface::cheats(const vector<string>& list) -> void {
  cheat.reset();
  #if defined(CORE_GB)
  if(cartridge.has.ICD) return GameBoy::cheat.assign(list);
  #endif
  cheat.assign(list);
}

}
