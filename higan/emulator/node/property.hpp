namespace higan::Object {

struct Property {
  Property(const string& name, const any& value = {}) : name(name), value(value) {}
  auto operator==(const Property& source) const -> bool { return name == source.name; }
  auto operator< (const Property& source) const -> bool { return name <  source.name; }

  string name;
  any value;
};

}
