#include <spec/spec.hpp>

namespace higan::Spectrum {

#include "deck.cpp"
#include "tray.cpp"

auto Tape::allocate(Node::Port parent) -> Node::Peripheral {
  node = parent->append<Node::Peripheral>("ZX Spectrum (Tapes)");
  position = 0;
  length = 0;
  range = 0;

  return node;
}

auto Tape::connect() -> void {
  node->setManifest([&] { return information.manifest; });

  information = {};

  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  }

  auto document = BML::unserialize(information.manifest);
  information.name = document["game/label"].text();

  auto location = node->attribute("location");
  if (file::exists({location, "program.tape"})) {
    Decode::WAV wav;
    if (wav.open({location, "program.tape"})) {
      range = (1 << wav.bitrate) -1;
      data.allocate(wav.size());
      position = 0;
      frequency = wav.frequency;
      length = wav.size();
      for(int i = 0; i < length; i++) {
        data.write(i, wav.read());
      }
      wav.close();


      stream = node->append<Node::Stream>("Audio");
      stream->setChannels(1);
      stream->setFrequency(frequency);
      Thread::create(frequency, [&] { Tape::main(); });
    }
  }
}

auto Tape::main() -> void {
  platform->input(tapeDeck.playStop);
  if (tapeDeck.playStop->value()) {
    tapeDeck.state.playing ^= 1;
  }

  if (!tapeDeck.state.playing) {
    stream->sample(0.0f);
    step(1);
    return;
  }

  if (position > length) {
    tapeDeck.state.playing = 0;
    step(1);
    return;
  }

  uint64 sample = data.read(position);
  position++;

  stream->sample((float)sample / (float)range);
  tapeDeck.state.output = sample > (range / 2);
  step(1);
}

auto Tape::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize();
}

auto Tape::disconnect() -> void {
  if(!node) return;
  Thread::destroy();
  data.reset();
  node = {};
  position = 0;
  length = 0;
}

}
