namespace higan {

Logger logger;
#include "notification.cpp"
#include "tracer.cpp"

auto Logger::reset(Interface* interface) -> void {
  notifications.reset();
  tracers.reset();
}

auto Logger::attached(shared_pointer<Notification>& notification) const -> bool {
  return (bool)notifications.find(notification);
}

auto Logger::attach(shared_pointer<Notification>& notification) -> void {
  if(attached(notification)) return;
  notification = shared_pointer<Notification>{new Notification};
  notifications.append(notification);
}

auto Logger::detach(shared_pointer<Notification>& notification) -> void {
  if(!attached(notification)) return;
  notifications.removeByValue(notification);
  notification.reset();
}

//

auto Logger::attached(shared_pointer<Tracer>& tracer) const -> bool {
  return (bool)tracers.find(tracer);
}

auto Logger::attach(shared_pointer<Tracer>& tracer) -> void {
  if(attached(tracer)) return;
  tracer = shared_pointer<Tracer>{new Tracer};
  tracer->setDepth(4);
  tracer->setAddressBits(32);
  tracers.append(tracer);
}

auto Logger::detach(shared_pointer<Tracer>& tracer) -> void {
  if(!attached(tracer)) return;
  tracers.removeByValue(tracer);
  tracer.reset();
}

}
