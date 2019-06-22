#if defined(Hiro_Viewport)
struct mViewport : mWidget {
  Declare(Viewport)

  auto doDrop(vector<string> names) const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto droppable() const -> bool;
  auto focusable() const -> bool;
  auto handle() const -> uintptr_t;
  auto onDrop(const function<void (vector<string>)>& callback = {}) -> type&;
  auto onMouseLeave(const function<void ()>& callback = {}) -> type&;
  auto onMouseMove(const function<void (Position position)>& callback = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto setDroppable(bool droppable = true) -> type&;
  auto setFocusable(bool focusable = true) -> type&;

//private:
  struct State {
    bool droppable = false;
    bool focusable = false;
    function<void (vector<string>)> onDrop;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
  } state;
};
#endif
