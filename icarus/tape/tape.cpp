#include "zx-spectrum-tape.cpp"

auto Tape::construct() -> void {
  Media::construct();
}

auto Tape::append(vector<uint8_t>& output, string filename) -> bool {
  if(!file::exists(filename)) return false;
  auto input = file::read(filename);
  auto size = output.size();
  output.resize(size + input.size());
  memory::copy(output.data() + size, input.data(), input.size());
  return true;
}

auto Tape::manifest(string location) -> string {
  vector<uint8_t> data;
  if(directory::exists(location)) {
    data = export(location);
  } else if(file::exists(location)) {
    data = file::read(location);
  }
  return manifest(data, location);
}

auto Tape::manifest(vector<uint8_t>& data, string location) -> string {
  string digest = Hash::SHA256(data).digest();
  for(auto game : database.find("game")) {
    if(game["sha256"].text() == digest) return BML::serialize(game);
  }
  return heuristics(data, location);
}

auto Tape::import(string location) -> string {
  auto data = Media::read(location);
  auto manifest = this->manifest(data, location);
  if(!manifest) return "failed to parse tape";

  auto document = BML::unserialize(manifest);
  location = {pathname, Location::prefix(location), "/"};
  if(!directory::create(location)) return "output directory not writable";

  if(settings.createManifests) {
    file::write({location, "manifest.bml"}, manifest);
  }

  location = {pathname, Location::prefix(location), "/"};
  if(!directory::create(location)) return "output directory not writable";

  file::write({location, "program.tape"}, data);
  return {};
}
