#if defined(Hiro_Frame)

namespace hiro {

auto pFrame::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | BS_GROUPBOX,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setText(state().text);
}

auto pFrame::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pFrame::append(sSizable sizable) -> void {
}

auto pFrame::remove(sSizable sizable) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto& sizable = state().sizable) sizable->setEnabled(sizable->enabled());
  pWidget::setEnabled(enabled);
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  bool empty = !state().text;
  auto size = pFont::size(hfont, state().text);
  //offsets are based on the default Windows 10 theme
  pWidget::setGeometry({
    geometry.x(),
    geometry.y() - (empty ? 9 : 3),
    geometry.width(),
    geometry.height() + (empty ? 10 : 4)
  });
  if(auto& sizable = state().sizable) {
    sizable->setGeometry({
      geometry.x() + 5,
      geometry.y() + (empty ? 5 : size.height()),
      geometry.width() - 10,
      geometry.height() - (empty ? 10 : size.height() + 5)
    });
  }
}

auto pFrame::setText(const string& text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto& sizable = state().sizable) sizable->setVisible(sizable->visible());
  pWidget::setVisible(visible);
}

}

#endif
