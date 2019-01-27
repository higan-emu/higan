LoadWindow::LoadWindow() {
  layout.setPadding(5);
  loadTree.setBackgroundColor({ 56,  56,  56});
  loadTree.setForegroundColor({255, 255, 255});
  loadTree.onActivate([&] {
    eventActivate();
  });
  regionOption.append(ComboButtonItem().setText("NTSC"));
  bootButton.setText("Boot").onActivate([&] {
    presentation.powerStatus.setChecked().doToggle();
    setVisible(false);
  });
}

auto LoadWindow::show() -> void {
  refresh();
  setTitle(emulator->information().name);
  setSize({480, 240});
  setCentered(presentation);
  setVisible();
  setFocused();
}

auto LoadWindow::refresh() -> void {
  loadTree.reset();
  for(auto& slot : emulator->slots()) load(loadTree, slot);
  for(auto item : loadTree.items()) expand(item);
}

template<typename T> auto LoadWindow::load(T parent, higan::Interface::Slot& slot) -> void {
  TreeViewItem item{&parent};
  item.setProperty("slotID", slot.id);
  if(auto index = program.slots.find([&](auto& item) { return slot.id == item.id; })) {
    item.setIcon(Icon::Action::Add);
    item.setText({slot.form, ": ", Location::base(program.slots[*index].location).trimRight("/", 1L)});
  } else {
    item.setIcon(Icon::Action::Remove);
    item.setText(slot.form);
  }
  for(auto& child : slot.slots) load(item, child);
}

template<typename T> auto LoadWindow::expand(T item) -> void {
  item.setExpanded();
  for(auto child : item.items()) expand(child);
}

auto LoadWindow::eventActivate() -> void {
  if(auto item = loadTree.selected()) {
    auto slots = emulator->slots();
    uint slotID = item.property("slotID").natural();
    if(auto index = slots.find([&](auto& slot) { return slot.id == slotID; })) {
      auto& slot = slots[*index];
      if(auto location = BrowserDialog()
        .setPath({Path::user(), "Emulation/", slot.name})
        .setFilters({"All|*.*"})
        .setParent(*this).openFolder()
      ) {
        if(auto index = program.slots.find([&](auto& slot) { return slot.id == slotID; })) {
          program.slots[*index].id = slot.id;
          program.slots[*index].location = location;
        } else {
          program.slots.append(Program::Slot());
          program.slots.last().id = slot.id;
          program.slots.last().location = location;
        }
        emulator->load(slot.id);
        refresh();
      }
    }
  }
}
