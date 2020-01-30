auto Program::attach(higan::Node::Object node) -> void {
}

auto Program::detach(higan::Node::Object node) -> void {
}

auto Program::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  auto location = node->attribute("location");

  if(name == "manifest.bml") {
    if(auto manifest = execute("icarus", "--system", node->name(), "--manifest", location).output) {
      emulator->game.manifest = manifest;
      auto document = BML::unserialize(manifest);
      presentation.setTitle(Location::prefix(document["game/label"].string()));
      return vfs::memory::file::open(manifest.data<uint8_t>(), manifest.size());
    }
  }

  return emulator->load(node, name, mode, required);
}

auto Program::event(higan::Event event) -> void {
}

auto Program::log(string_view message) -> void {
}

auto Program::video(higan::Node::Screen node, const uint32_t* data, uint pitch, uint width, uint height) -> void {
  pitch >>= 2;
  if(auto [output, length] = ruby::video.acquire(width, height); output) {
    length >>= 2;
    for(auto y : range(height)) {
      memory::copy<uint32_t>(output + y * length, data + y * pitch, width);
    }
    ruby::video.release();
    ruby::video.output();
  }

  static uint64_t frameCounter = 0, previous, current;
  frameCounter++;

  current = chrono::timestamp();
  if(current != previous) {
    previous = current;
    presentation.statusBar.setText({"FPS: ", frameCounter});
    frameCounter = 0;
  }
}

auto Program::audio(higan::Node::Stream node) -> void {
}

auto Program::input(higan::Node::Input node) -> void {
}
