#if defined(Hiro_Viewport)

namespace hiro {

struct pViewport : pWidget {
  Declare(Viewport, Widget)

  auto handle() const -> uintptr;

  auto _onDraw(cairo_t* context) -> void;
  auto _onExpose(GdkEventExpose* expose) -> void;
};

}

#endif
