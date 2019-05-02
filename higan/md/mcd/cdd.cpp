//NEC uPD75006 (G-631)
//4-bit MCU HLE

auto MCD::CDD::clock() -> void {
  if(!hostClockEnable || ++counter < 434) return;
  counter = 0;

  //75hz
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
    status[0] = DriveStatus::ChecksumError;
    return;
  }

  //invalidate command checksum
  command[9] = 0x0;

  switch(command[0]) {

  case 0x0: {  //report drive status
    status[0] = io.status;
  } break;

  case 0x1: {  //stop drive
    io.status = DriveStatus::NoDisc;
    status[0] = DriveStatus::Stop;
    status[1] = 0x0;
    status[2] = 0x0;
    status[3] = 0x0;
    status[4] = 0x0;
    status[5] = 0x0;
    status[6] = 0x0;
    status[7] = 0x0;
    status[8] = 0x0;
  } break;

  case 0x2: {  //report table of contents
    switch(command[3]) {

    case 0x3: {  //total length (mm:ss:ff)
      status[0] = io.status;
      status[1] = 0x3;
      status[2] = 0x0;
      status[3] = 0x0;
      status[4] = 0x0;
      status[5] = 0x0;
      status[6] = 0x0;
      status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case 0x4: {  //first and last track numbers
      status[0] = io.status;
      status[1] = 0x4;
      status[2] = 0x0;
      status[3] = 0x0;
      status[4] = 0x0;
      status[5] = 0x0;
      status[6] = 0x0;
      status[7] = 0x0;
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
  io.status = DriveStatus::NoDisc;
  for(uint index : range(10)) status [index] = 0x0;
  for(uint index : range(10)) command[index] = 0x0;
  checksum();
}
