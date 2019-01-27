auto Program::path(uint id) -> string {
  if(auto index = slots.find([&](auto& slot) { return slot.id == id; })) return slots[*index].location;
  return {};
}

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  if(name == "manifest.bml") {
    if(!file::exists({path(id), name})) {
      if(auto manifest = execute("icarus", "--manifest", path(id))) {
        return vfs::memory::file::open(manifest.output.data<uint8_t>(), manifest.output.size());
      }
    }
  }

  if(auto result = vfs::fs::file::open({path(id), name}, mode)) return result;

  if(required) {
    string location = BrowserDialog()
    .setTitle({"Load ", emulator->information().name, " - ", name})
    .setPath(Path::user())
    .setFilters({"All|*"})
    .openFile();
    if(file::exists(location); auto memory = vfs::memory::file::open(location, true)) {
      if(memory->size()) file::write({path(id), name}, memory->reads());
    }
    if(auto result = vfs::fs::file::open({path(id), name}, mode)) return result;

    MessageDialog()
    .setTitle({"Error"})
    .setText({"Missing required file:\n\n", path(id), name})
    .setParent(presentation).error();
  }

  return {};
}

auto Program::load(uint id, string name, string type, vector<string> options) -> higan::Platform::Load {
  BrowserDialog dialog;
  string location = dialog
  .setTitle({"Load ", name})
  .setPath({Path::user(), "Emulation/", name})
  .setFilters({string{name, "|*.", type}, "All|*.*"})
  .setOptions(options)
  .openFolder();
  string option = dialog.option();

  if(directory::exists(location)) {
    uint pathID = gamePaths.size();
    gamePaths.append(location);
    return {pathID, option};
  }

  return {};
}

auto Program::videoFrame(const uint32* data, uint pitch, uint width, uint height) -> void {
  pitch >>= 2;

  if(auto [output, length] = video.acquire(width, height); output) {
    length >>= 2;

    for(uint y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
    }

    video.release();
    video.output();
  }

  static natural frameCounter, previous, current;
  frameCounter++;
  current = chrono::timestamp();
  if(current != previous) {
    previous = current;
    presentation.statusMonitor.setText({frameCounter, " FPS"});
    frameCounter = 0;
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

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
}

auto Program::dipSettings(Markup::Node node) -> uint {
  return 0;
}

auto Program::notify(string text) -> void {
}
