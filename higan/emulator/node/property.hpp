namespace higan::Core {

struct Property {
  Property(string name, string value = {}) : name(name), value(value) {}
  auto operator==(const Property& source) const -> bool { return name == source.name; }
  auto operator< (const Property& source) const -> bool { return name <  source.name; }

  string name;
  string value;
};

}
