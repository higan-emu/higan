auto Media::construct() -> void {
  database = BML::unserialize(file::read(locate({"Database/", name(), ".bml"})));
  pathname = {Path::user(), "Emulation/", name(), "/"};
}

auto Media::read(string location) -> vector<uint8_t> {
  if(location.iendsWith(".zip")) {
    Decode::ZIP archive;
    if(archive.open(location) && archive.file.size() >= 1) {
      return archive.extract(archive.file.first());
    }
  }
  return file::read(location);
}
