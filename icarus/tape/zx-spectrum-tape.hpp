struct ZXSpectrumTape : Tape {
  auto name() -> string override { return "ZX Spectrum (Tapes)"; }
  auto extensions() -> vector<string> override { return {"wav"}; }
  auto export(string location) -> vector<uint8_t> override;
  auto heuristics(vector<uint8_t>& data, string location) -> string override;
};
