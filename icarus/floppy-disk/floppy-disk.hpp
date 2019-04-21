struct FloppyDisk : Media {
  auto type() -> string override { return "Floppy Disk"; }
  auto construct() -> void override;
  auto metadata(string location) -> string override;
};
