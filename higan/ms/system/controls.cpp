Controls controls;

auto Controls::load(Node::Object parent, Node::Object from) -> void {
  if(Model::SG1000() || Model::SC3000() || Model::MasterSystem()) {
    pause = Node::append<Node::Button>(parent, from, "Pause");
  }

  if(Model::MasterSystem()) {
    reset = Node::append<Node::Button>(parent, from, "Reset");
  }

  if(Model::GameGear()) {
    up    = Node::append<Node::Button>(parent, from, "Up");
    down  = Node::append<Node::Button>(parent, from, "Down");
    left  = Node::append<Node::Button>(parent, from, "Left");
    right = Node::append<Node::Button>(parent, from, "Right");
    one   = Node::append<Node::Button>(parent, from, "1");
    two   = Node::append<Node::Button>(parent, from, "2");
    start = Node::append<Node::Button>(parent, from, "Start");
  }
}
