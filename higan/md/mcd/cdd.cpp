auto MCD::CDD::clock() -> void {
  if(++counter < 434) return;
  counter = 0;

  //75hz
  hock = 0;
  irq.raise();
}

auto MCD::CDD::write() -> void {
//print("CDD ", command[0], "\n");
  for(auto& byte : status) byte = 0;

  switch(command[0]) {

  case 0x0: {  //report drive status
    status[0] = 0xb;
  } break;

  case 0x1: {  //stop drive
    status[0] = 0xb;
  } break;

  case 0x2: {  //report table of contents
    status[1] = command[3];
    switch(command[3]) {

    case 0x3: {  //total length (mm:ss:ff)
      status[0] = 0xb;
    } break;

    case 0x4: {  //first and last track numbers
      status[0] = 0xb;
    } break;

    case 0xc: {  //close tray
      status[0] = 0xb;
    } break;

    }
  } break;

  default:
    status[0] = 0xb;
    break;

  }

  checksum();
}

auto MCD::CDD::checksum() -> void {
  status[9] = 0;
  for(uint index : range(9)) status[9] += status[index];
  status[9] = ~status[9];
}

auto MCD::CDD::power(bool reset) -> void {
  fader = {};
  for(auto& byte : status ) byte = 0;
  for(auto& byte : command) byte = 0;
  checksum();
}
