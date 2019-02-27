struct Settings {
  auto load() -> void;
  auto save() -> void;

  auto serialize() -> string;
  auto unserialize(const string&) -> void;

  struct {
    string driver = Video::optimalDriver();
  } video;

  struct {
    string driver = Audio::optimalDriver();
  } audio;

  struct {
    string driver = Input::optimalDriver();
  } input;
};

extern Settings settings;
