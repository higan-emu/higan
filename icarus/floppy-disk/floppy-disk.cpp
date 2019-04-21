#include "famicom-disk.cpp"

auto FloppyDisk::construct() -> void {
  Media::construct();
}

auto FloppyDisk::metadata(string location) -> string {
  string s;
  s += "game\n";
  return s;
}
