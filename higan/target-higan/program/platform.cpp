auto Program::open(higan::Node node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
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

auto Program::videoFrame(const uint32* data, uint pitch, uint width, uint height) -> void {
  pitch >>= 2;
  if(auto [output, length] = video.acquire(width, height); output) {
    length >>= 2;
    for(auto y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
    }
    video.release();
    video.output();
  }
}

auto Program::audioFrame(const double* samples, uint channels) -> void {
  if(channels == 1) {
    double stereo[] = {samples[0], samples[0]};
    audio.output(stereo);
  } else {
    audio.output(samples);
  }
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  return 0;
}
