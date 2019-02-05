auto Emulator::attach(higan::Node::Object node) -> void {
  if(auto location = node->property("location")) {
    node->load(file::read({location, "node.bml"}));
    if(node->find<higan::Node::Input>()) inputManager.bind();
  }
}

auto Emulator::detach(higan::Node::Object node) -> void {
  if(auto location = node->property("location")) {
    file::write({location, "node.bml"}, node->save());
  }
}

auto Emulator::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  auto location = node->property("location");

  if(name == "manifest.bml") {
    if(!file::exists({location, name}) && directory::exists(location)) {
      if(auto manifest = execute("icarus", "--manifest", location)) {
        return vfs::memory::file::open(manifest.output.data<uint8_t>(), manifest.output.size());
      }
    }
  }

  if(auto result = vfs::fs::file::open({location, name}, mode)) return result;

  if(required) {
    //attempt to pull required system firmware (boot ROMs, etc) from system template
    if(location == emulator.system.data) {
      if(file::exists({emulator.system.templates, name})) {
        file::copy({emulator.system.templates, name}, {emulator.system.data, name});
        if(auto result = vfs::fs::file::open({location, name}, mode)) return result;
      }
    }

    //todo: show file browser to select file here ... eg to select GBA BIOS
    MessageDialog()
    .setTitle("Error")
    .setText({"Missing required file:\n", location, name})
    .setPlacement(Placement::Center)
    .error();
  }

  return {};
}

auto Emulator::videoFrame(higan::Node::Video node, const uint32* data, uint pitch, uint width, uint height) -> void {
  auto viewportID = node->property("viewportID").natural();
  if(viewportID >= viewports.size()) return;
  auto viewport = viewports[viewportID];

  pitch >>= 2;
  if(auto [output, length] = viewport->video.acquire(width, height); output) {
    length >>= 2;
    for(auto y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
    }
    viewport->video.release();
    viewport->video.output();
  }
}

auto Emulator::audioFrame(higan::Node::Audio node, const double* samples, uint channels) -> void {
  if(channels == 1) {
    double stereo[] = {samples[0], samples[0]};
    audio.output(stereo);
  } else {
    audio.output(samples);
  }
}

auto Emulator::inputPoll(higan::Node::Input input) -> void {
  inputManager.poll();

  if(auto button = input->cast<higan::Node::Button>()) {
    button->value = 0;
    if(auto instance = button->property<shared_pointer<InputButton>>("instance")) {
      button->value = instance->value();
    }
  }
}
