auto Emulator::open(higan::Node node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  auto location = node->property("location");

  if(name == "manifest.bml") {
    if(!file::exists({location, name})) {
      if(auto manifest = execute("icarus", "--manifest", location)) {
        return vfs::memory::file::open(manifest.output.data<uint8_t>(), manifest.output.size());
      }
    }
  }

  if(auto result = vfs::fs::file::open({location, name}, mode)) return result;

  if(required) {
    MessageDialog()
    .setTitle("Error")
    .setText({"Missing required file:\n", location, name})
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

auto Emulator::audioFrame(const double* samples, uint channels) -> void {
  if(channels == 1) {
    double stereo[] = {samples[0], samples[0]};
    audio.output(stereo);
  } else {
    audio.output(samples);
  }
}

auto Emulator::inputPoll(higan::Node::Input input) -> void {
  inputManager.poll();

  if(auto button = input->cast<higan::Node::Input::Button>()) {
    button->value = 0;
    if(auto device = button->property("device")) {
      auto groupID = button->property("group").natural();
      auto inputID = button->property("input").natural();
      button->value = device.to<shared_pointer<HID::Device>>()->group(groupID).input(inputID).value();
    }
  }
}
