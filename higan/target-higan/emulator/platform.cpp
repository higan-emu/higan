auto Emulator::attach(higan::Node::Object node) -> void {
  if(auto location = node->property("location")) {
    if(node->find<higan::Node::Input>()) inputManager.bind();
  }
}

auto Emulator::detach(higan::Node::Object node) -> void {
  if(auto location = node->property("location")) {
    file::write({location, "settings.bml"}, node->save());
  }
}

auto Emulator::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  auto location = node->property("location");

  if(name == "metadata.bml") {
    if(!file::exists({location, name}) && directory::exists(location)) {
      if(auto metadata = execute("icarus", "--system", node->name, "--metadata", location).output) {
        return vfs::memory::file::open(metadata.data<uint8_t>(), metadata.size());
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

    if(MessageDialog()
    .setTitle("Warning")
    .setText({"Missing required file:\n",
              location, name, "\n\n",
              "Would you like to browse for this file now?"})
    .setAlignment(Alignment::Center)
    .question() == "No") return {};

    if(auto source = BrowserDialog()
    .setTitle({"Load ", name})
    .setPath(location)
    .setAlignment(Alignment::Center)
    .openFile()
    ) {
      if(auto input = vfs::memory::file::open(source, true)) {
        if(auto output = file::open({location, name}, file::mode::write)) {
          output.write({input->data(), (uint)input->size()});
        }
      }
      if(auto result = vfs::fs::file::open({location, name}, mode)) return result;
    }
  }

  return {};
}

auto Emulator::video(higan::Node::Video node, const uint32_t* data, uint pitch, uint width, uint height) -> void {
  auto viewport = node->property<shared_pointer<ViewportWindow>>("viewport");
  if(!viewport) return;

  pitch >>= 2;
  if(auto [output, length] = viewport->video.acquire(width, height); output) {
    length >>= 2;
    for(auto y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
    }
    viewport->video.release();
    viewport->video.output();
  }

  static uint frameCounter = 0;
  static uint64_t previous, current;
  frameCounter++;

  current = chrono::timestamp();
  if(current != previous) {
    previous = current;
    viewport->setTitle({"Display: ", frameCounter, " FPS"});
    frameCounter = 0;
  }
}

auto Emulator::audio(higan::Node::Audio node, const double* samples, uint channels) -> void {
  if(channels == 1) {
    double stereo[] = {samples[0], samples[0]};
    sound.output(stereo);
  } else {
    sound.output(samples);
  }
}

auto Emulator::input(higan::Node::Input input) -> void {
  inputManager.poll();

  if(auto button = input->cast<higan::Node::Button>()) {
    button->value = 0;
    if(auto instance = button->property<shared_pointer<InputButton>>("instance")) {
      button->value = instance->value();
    }
  }
}
