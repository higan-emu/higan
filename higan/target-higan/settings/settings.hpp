struct Settings {
  auto load() -> void;
  auto save() -> void;
  auto properties(bool mode, Markup::Node document) -> void;

  struct {
    string driver = Video::optimalDriver();
    bool exclusive = false;
    bool synchronize = false;
    bool flush = false;
    double luminance = 1.0;
    double saturation = 1.0;
    double gamma = 1.0;
  } video;

  struct {
    string driver = Audio::optimalDriver();
    string device;
    uint frequency = 48000;
    uint latency = 0;
    bool exclusive = false;
    bool synchronize = true;
    bool dynamicRate = false;
    int skew = 0;
    double volume = 1.0;
    double balance = 0.5;
  } audio;

  struct {
    string driver = Input::optimalDriver();
    string unfocused = "Block";
  } input;
};

extern Settings settings;
