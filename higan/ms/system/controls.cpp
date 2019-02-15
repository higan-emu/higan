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

auto Controls::poll() -> void {
  if(Model::SG1000() || Model::SC3000() || Model::MasterSystem()) {
    platform->input(pause);
  }

  if(Model::MasterSystem()) {
    platform->input(reset);
  }

  if(Model::GameGear()) {
    platform->input(up);
    platform->input(down);
    platform->input(left);
    platform->input(right);
    platform->input(one);
    platform->input(two);
    platform->input(start);

    if(!(up->value & down->value)) {
      yHold = 0, upLatch = up->value, downLatch = down->value;
    } else if(!yHold) {
      yHold = 1, swap(upLatch, downLatch);
    }

    if(!(left->value & right->value)) {
      xHold = 0, leftLatch = left->value, rightLatch = right->value;
    } else if(!xHold) {
      xHold = 1, swap(leftLatch, rightLatch);
    }
  }
}
