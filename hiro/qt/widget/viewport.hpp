#if defined(Hiro_Viewport)

namespace hiro {

struct pViewport : pWidget {
  Declare(Viewport, Widget)

  auto handle() const -> uintptr_t;
  auto setDroppable(bool droppable) -> void;
  auto setFocusable(bool focusable) -> void;

  auto _setState() -> void;

  QtViewport* qtViewport = nullptr;
};

}

#endif
