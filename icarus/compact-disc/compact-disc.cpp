#include "mega-cd.cpp"

auto CompactDisc::construct() -> void {
  Media::construct();
}

auto CompactDisc::metadata(string location) -> string {
  string s;
  s += "game\n";
  return s;
}

auto CompactDisc::import(string filename) -> string {
  string location = {pathname, Location::prefix(filename), "/"};
  if(!directory::create(location)) return "output directory not writable";
  if(auto error = importCUE(filename)) return error;

  auto input = Media::read(filename, ".bin");
  if(!input) input = Media::read(filename, ".img");
  if(!input) return "image file missing";
  if(auto fp = file::open({location, "program.rom"}, file::mode::write)) {
    for(uint index : range(150 * 2352)) fp.write(0x00);
    for(auto& byte : input) fp.write(byte);

    int lba = input.size() / 2352 + 150;
    int minute = lba / 75 / 60;
    int second = lba / 75 % 60;
    int frame  = lba % 75;

    auto markup = string::read({location, "disc.bml"});
    markup +={"  msf: ", pad(minute, 2, '0'), ":", pad(second, 2, '0'), ":", pad(frame, 2, '0'), "\n"};
    file::write({location, "disc.bml"}, markup);
  }

  if(auto fp = file::open({location, "program.sub"}, file::mode::write)) {
    //todo: attempt to generate valid P/Q data here
    for(uint index : range(150 * 96)) fp.write(0x00);
    uint sectors = input.size() / 2352;
    input = Media::read(filename, ".sub");
    if(input) {
      for(auto& byte : input) fp.write(byte);
    } else {
      //todo: attempt to generate valid P/Q data here
      for(uint index : range(sectors * 96)) fp.write(0x00);
    }
  }

  return {};
}

auto CompactDisc::importCUE(string filename) -> string {
  auto input = Media::read(filename, ".cue");
  if(!input) return "CUE: missing";

  string location = {pathname, Location::prefix(filename), "/"};

  string s;
  int offset = 0;
  int track = -1;
  bool type = 0;
  s += "session\n";
  auto lines = string{input}.split("\n");
  for(auto& line : lines) {
    line.strip();

    if(line.beginsWith("TRACK ")) {
      line.trimLeft("TRACK ", 1L).strip();
      if(line.endsWith(" AUDIO")) {
        type = 0;
        line.trimRight(" AUDIO", 1L).strip();
      } else if(line.endsWith(" MODE1/2352")) {
        type = 1;
        line.trimRight(" MODE1/2352", 1L).strip();
      } else return "CUE: unrecognized track type";

      track = line.natural();
      if(track > 99) return "CUE: invalid track number";
      s +={"  track: ", pad(track, 2, '0'), "\n"};
      if(type == 0) s += "    audio\n";
      if(type == 1) s += "    data\n";
      continue;
    }

    if(line.beginsWith("INDEX ")) {
      if(track == -1) return "CUE: missing track number";
      line.trimLeft("INDEX ", 1L).strip();
      auto part = line.split(" ", 1L);
      if(part.size() != 2) return "CUE: misformatted index";
      int index = part[0].natural();
      if(index > 99) return "CUE: invalid index";
      part = part[1].split(":");
      if(part.size() != 3) return "CUE: misformatted index time";
      int minute = part[0].natural();
      int second = part[1].natural();
      int frame  = part[2].natural();
      if(minute > 99 || second > 59 || frame > 74) return "CUE: invalid index time";
      int lba = minute * 60 * 75 + second * 75 + frame + 150;
      minute = lba / 75 / 60;
      second = lba / 75 % 60;
      frame  = lba % 75;
      if(minute > 99) return "CUE: invalid index time";
      if(track == 1 && index == 1) {
        s += "    index: 00\n";
        s += "      msf: 00:00:00\n";
      }
      s +={"    index: ", pad(index, 2, '0'), "\n"};
      s +={"      msf: ", pad(minute, 2, '0'), ":", pad(second, 2, '0'), ":", pad(frame, 2, '0'), "\n"};
      continue;
    }
  }
  file::write({location, "disc.bml"}, s);
  return {};
}
