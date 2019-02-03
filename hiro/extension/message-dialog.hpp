#if defined(Hiro_MessageDialog)

struct MessageDialog {
  using type = MessageDialog;

  MessageDialog(const string& text = "");
  auto checked() const -> bool;
  auto dismissed() const -> bool;
  auto error(const vector<string>& buttons = {"Ok"}) -> string;
  auto information(const vector<string>& buttons = {"Ok"}) -> string;
  auto question(const vector<string>& buttons = {"Yes", "No"}) -> string;
  auto setChecked(bool checked = true) -> type&;
  auto setOption(const string& option = "") -> type&;
  auto setPlacement(Placement placement, sWindow relativeTo = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto warning(const vector<string>& buttons = {"Ok"}) -> string;

private:
  struct State {
    vector<string> buttons;
    bool checked = false;
    bool dismissed = false;
    image icon;
    string option;
    Placement placement = Placement::Center;
    sWindow relativeTo;
    string response;
    string text;
    string title;
  } state;

  auto _run() -> string;
};

#endif
