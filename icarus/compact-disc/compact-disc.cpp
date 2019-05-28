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

  CD::Session session;
  auto cue = Media::read(filename, ".cue");
  if(!cue) return "cue file missing";
  if(auto error = importCUE(session, string{cue})) return error;

  auto img = Media::read(filename, ".img");
  if(!img) img = Media::read(filename, ".bin");
  if(!img) return "image file missing";
  if(img.size() % 2352) return "image file size not a multiple of 2352";
  uint sectors = img.size() / 2352;

  auto sub = Media::read(filename, ".sub");
  if(sub.size() != sectors * 96) sub = {};  //ignore invalid .sub files

  session.leadOut.offset = sectors + session.tracks[1].indices[0].len();
  session.computeLengths();
  file::write({location, "cd.bml"}, session.serialize());

  if(auto fp = file::open({location, "cd.rom"}, file::mode::write)) {
    array_view<uint8_t> imgData{img};
    array_view<uint8_t> subData{sub};

    //generate lead-in
    for(uint index : range(session.leadIn.len() * 2448)) fp.write(0x00);

    //generate track 1 pre-gap
    for(uint index : range(session.tracks[1].indices[0].len() * 2448)) fp.write(0x00);

    for(uint sector : range(sectors)) {
      for(uint index : range(2352)) fp.write(*imgData++);
      if(sub) {
        for(uint index : range(96)) fp.write(*subData++);
      } else {
        for(uint index : range(96)) fp.write(0x00);
      }
    }

    //generate lead-out
    for(uint index : range(session.leadOut.len() * 2448)) fp.write(0x00);
  }

  return {};
}

auto CompactDisc::importCUE(CD::Session& session, string document) -> string {
  session = {};
  session.leadIn = {-7500, 7500};
  session.tracks[1].indices[0] = {0, 150};
  session.leadOut = {150, 6750};  //session.leadOut.offset computed later

  int offset = 0;
  int track = -1;
  auto lines = document.split("\n").strip();

  for(auto& line : lines) {
    if(line.beginsWith("TRACK ")) {
      line.trimLeft("TRACK ", 1L).strip();
      int control = 0b1111;
      if(line.endsWith(" AUDIO")) {
        line.trimRight(" AUDIO", 1L).strip();
        control = 0b0000;
      } else if(line.endsWith(" MODE1/2352")) {
        line.trimRight(" MODE1/2352", 1L).strip();
        control = 0b0100;
      } else return "CUE: unrecognized track type";

      track = line.natural();
      if(track > 99) return "CUE: invalid track number";
      session.tracks[track].control = control;
      continue;
    }

    if(line.beginsWith("INDEX ")) {
      if(track == -1) return "CUE: missing track number";
      line.trimLeft("INDEX ", 1L).strip();
      auto part = line.split(" ", 1L);
      if(part.size() != 2) return "CUE: misformatted index";
      int index = part[0].natural();
      if(index > 99) return "CUE: invalid index";
      auto msf = CD::MSF(part[1]);
      if(!msf) return "CUE: invalid index time";
      msf = CD::MSF::fromLBA(msf.toLBA() + 150);
      if(!msf) return "CUE: invalid index time";
      session.tracks[track].indices[index].offset = msf.toLBA();
      continue;
    }
  }

  return {};
}
