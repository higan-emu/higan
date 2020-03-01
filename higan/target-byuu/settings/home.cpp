auto HomePanel::construct() -> void {
  setCollapsible();
  setVisible(false);

  image icon{Resource::Logo};
  auto data = icon.data();
  for(uint y : range(icon.height())) {
    for(uint x : range(icon.width())) {
      auto pixel = icon.read(data);
      auto a = pixel >> 24 & 255;
      auto r = pixel >> 16 & 255;
      auto g = pixel >>  8 & 255;
      auto b = pixel >>  0 & 255;
      a = a * 0.25;
      icon.write(data, a << 24 | r << 16 | g << 8 | b << 0);
      data += icon.stride();
    }
  }
  canvas.setIcon(icon);
}
