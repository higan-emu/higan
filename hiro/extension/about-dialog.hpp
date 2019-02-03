#if defined(Hiro_AboutDialog)

struct AboutDialog {
  using type = AboutDialog;

  auto setAuthor(const string& author = "") -> type&;
  auto setDescription(const string& description = "") -> type&;
  auto setLicense(const string& license = "") -> type&;
  auto setLogo(const image& logo = {}) -> type&;
  auto setName(const string& name = "") -> type&;
  auto setPlacement(Placement placement, sWindow relativeTo = {}) -> type&;
  auto setVersion(const string& version = "") -> type&;
  auto setWebsite(const string& website = "") -> type&;
  auto show() -> void;

private:
  struct State {
    string author;
    string description;
    string license;
    image logo;
    string name;
    Placement placement = Placement::Center;
    sWindow relativeTo;
    string version;
    string website;
  } state;
};

#endif
