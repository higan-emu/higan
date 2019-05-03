//NEC uPD75006 (G-631)
//4-bit MCU HLE

auto MCD::CDD::clock() -> void {
  if(++counter < 434) return;
  counter = 0;

  //75hz
  if(!hostClockEnable) return;

  if(statusPending) {
    statusPending = 0;
    irq.raise();
  }
}

auto MCD::CDD::process() -> void {
//print("CDD ", command[0], ":", command[3], "\n");

  if(!valid()) {
    //unverified
    print("CDD checksum error\n");
    status[0] = Status::ChecksumError;
    return;
  }

  switch(command[0]) {

  case Command::Idle: {  //simulates the status changing shortly after the Stop command
    status[0] = io.status;
  } break;

  case Command::Stop: {
    status[0] = Status::Stopped; status[1] = 0x0;
    status[2] = 0x0; status[3] = 0x0;
    status[4] = 0x0; status[5] = 0x0;
    status[6] = 0x0; status[7] = 0x0;
    status[8] = 0x0;
  } break;

  case Command::Request: {
    switch(command[3]) {

    case Request::DiscCompletionTime: {  //time in mm:ss:ff
      status[0] = io.status; status[1] = command[3];
      status[2] = 0x0; status[3] = 0x0;
      status[4] = 0x0; status[5] = 0x0;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case Request::DiscTracks: {  //first and last track numbers
      status[0] = io.status; status[1] = command[3];
      status[2] = 0x0; status[3] = 0x0;
      status[4] = 0x0; status[5] = 0x0;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case Request::ErrorInformation: {
      //always report no errors
      status[0] = io.status; status[1] = command[3];
      status[2] = 0x0; status[3] = 0x0;
      status[4] = 0x0; status[5] = 0x0;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    }
  } break;

  }

  checksum();
  statusPending = 1;
}

auto MCD::CDD::valid() -> bool {
  uint4 checksum;
  for(uint index : range(9)) checksum += command[index];
  checksum = ~checksum;
  return checksum == command[9];
}

auto MCD::CDD::checksum() -> void {
  uint4 checksum;
  for(uint index : range(9)) checksum += status[index];
  checksum = ~checksum;
  status[9] = checksum;
}

auto MCD::CDD::power(bool reset) -> void {
  fader = {};
  io = {};
  io.status = mcd.disc ? Status::Stopped : Status::NoDisc;
  for(uint index : range(10)) status [index] = 0x0;
  for(uint index : range(10)) command[index] = 0x0;
  checksum();
}
