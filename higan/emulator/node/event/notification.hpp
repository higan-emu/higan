struct Notification : Event {
  DeclareClass(Notification, "Notification")

  Notification(string name = {}, string component = {}) : Event(name, component) {
  }

  auto notify(string_view message = {}) -> void {
    if(message) {
      print(_component, " [", name, ": ", message, "]\n");
    } else {
      print(_component, " [", name, "]\n");
    }
  }

protected:
};
