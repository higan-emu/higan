#if defined(Hiro_PopupMenu)

@implementation CocoaPopupMenu : NSMenuItem

-(id) initWith:(hiro::mPopupMenu&)menuReference {
  if(self = [super initWithTitle:@"" action:nil keyEquivalent:@""]) {
    popupMenu = &menuReference;

    cocoaPopupMenu = [[NSMenu alloc] initWithTitle:@""];
    [self setSubmenu:cocoaPopupMenu];
  }
  return self;
}

-(NSMenu*) cocoaPopupMenu {
  return cocoaPopupMenu;
}

@end

namespace hiro {

auto pPopupMenu::construct() -> void {
  @autoreleasepool {
    cocoaPopupMenu = [[CocoaPopupMenu alloc] initWith:self()];
  }
}

auto pPopupMenu::destruct() -> void {
  @autoreleasepool {
    [[cocoaPopupMenu cocoaPopupMenu] release];
  }
}

auto pPopupMenu::append(sAction action) -> void {
  @autoreleasepool {
    if(auto pAction = action->self()) {
      [[cocoaPopupMenu cocoaPopupMenu] addItem:pAction->cocoaAction];
    }
  }
}

auto pPopupMenu::remove(sAction action) -> void {
  @autoreleasepool {
    if(auto pAction = action->self()) {
      [[cocoaPopupMenu cocoaPopupMenu] removeItem:pAction->cocoaAction];
    }
  }
}

auto pPopupMenu::setVisible(bool visible) -> void {
  if(!visible) return;

  NSEvent* event = [[NSApplication sharedApplication] currentEvent];
  [NSMenu popUpContextMenu:[cocoaPopupMenu cocoaPopupMenu] withEvent:event forView:nil];
}

}

#endif
