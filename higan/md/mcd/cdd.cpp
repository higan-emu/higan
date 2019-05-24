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

  switch(io.status) {

  case Status::Stopped: {
    io.status = mcd.disc ? Status::ReadingTOC : Status::NoDisc;
    io.sector = 0;
    io.track  = 0;
  } break;

  case Status::ReadingTOC: {
    io.sector++;
    if(io.sector >= 150) io.status = Status::Paused;
    if(io.sector >= 150) io.track  = 1;
  } break;

  case Status::Playing: {
    mcd.cdc.decode(io.sector);
    io.sector++;
    io.track = 1;
  } break;

  }
}

auto MCD::CDD::process() -> void {
if(command[0]) print("CDD ", command[0], ":", command[3], "\n");

  if(!valid()) {
    //unverified
    print("CDD checksum error\n");
    io.status = Status::ChecksumError;
  }

  else
  switch(command[0]) {

  case Command::Idle: {
  } break;

  case Command::Stop: {
    io.status = Status::Stopped;
    status[1] = 0x0;
    status[2] = 0x0; status[3] = 0x0;
    status[4] = 0x0; status[5] = 0x0;
    status[6] = 0x0; status[7] = 0x0;
    status[8] = 0x0;
  } break;

  case Command::Request: {
    switch(command[3]) {

    case Request::AbsoluteTime: {
      uint sector = io.sector;
      uint minute = sector / 75 / 60 % 60;
      uint second = sector / 75 % 60;
      uint frame  = sector % 75;
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  % 10;
      status[8] = toc.tracks[io.track].type << 2;
    } break;

    case Request::RelativeTime: {
      uint sector = io.sector - toc.tracks[io.track].start;
      uint minute = sector / 75 / 60 % 60;
      uint second = sector / 75 % 60;
      uint frame  = sector % 75;
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  / 10;
      status[8] = toc.tracks[io.track].type << 2;
    } break;

    case Request::TrackInformation: {
      status[1] = command[3];
      status[2] = io.track / 10; status[3] = io.track % 10;
      status[4] = 0x0; status[5] = 0x0;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case Request::DiscCompletionTime: {  //time in mm:ss:ff
      uint minute = 4;
      uint second = 0;
      uint frame  = 0;
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  % 10;
      status[8] = 0x0;
    } break;

    case Request::DiscTracks: {  //first and last track numbers
      status[1] = command[3];
      status[2] = toc.first / 10; status[3] = toc.first % 10;
      status[4] = toc.last  / 10; status[5] = toc.last  % 10;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case Request::TrackStartTime: {
      uint track  = command[4] << 4 | command[5] << 0;
      uint sector = toc.tracks[track].start;
      uint minute = sector / 75 / 60 % 60;
      uint second = sector / 75 % 60;
      uint frame  = sector % 75;
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  % 10;
      status[6].bit(3) = toc.tracks[track].type;
      status[8] = track % 10;
    } break;

    case Request::ErrorInformation: {
      //always report no errors
      status[1] = command[3];
      status[2] = 0x0; status[3] = 0x0;
      status[4] = 0x0; status[5] = 0x0;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    }
  } break;

  case Command::SeekPlay: {
    uint minute = command[2] * 10 + command[3];
    uint second = command[4] * 10 + command[5];
    uint frame  = command[6] * 10 + command[7];

    counter   = 0;
    io.status = Status::Playing;
    io.sector = minute * 60 * 75 + second * 75 + frame - 3;
    io.track  = 1;

    status[1] = 0xf;
    status[2] = 0x0; status[3] = 0x0;
    status[4] = 0x0; status[5] = 0x0;
    status[6] = 0x0; status[7] = 0x0;
    status[8] = 0x0;
  } break;

  case Command::SeekPause: {
    uint minute = command[2] * 10 + command[3];
    uint second = command[4] * 10 + command[5];
    uint frame  = command[6] * 10 + command[7];

    counter   = 0;
    io.status = Status::Paused;
    io.sector = minute * 60 * 75 + second * 75 + frame;
    io.track  = 1;

    status[1] = 0xf;
    status[2] = 0x0; status[3] = 0x0;
    status[4] = 0x0; status[5] = 0x0;
    status[6] = 0x0; status[7] = 0x0;
    status[8] = 0x0;
  } break;

  case Command::Pause: {
    io.status = Status::Paused;
  } break;

  case Command::Play: {
    io.status = Status::Playing;
  } break;

  }

  status[0] = io.status;
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

auto MCD::CDD::insert() -> void {
  toc = {};
  for(auto& track : toc.tracks) track = {};

  if(!mcd.disc) {
    io.status = Status::NoDisc;
    return;
  }

  io.status = Status::ReadingTOC;
  io.sector = 0;
  io.track  = 0;

  toc.first = 1;
  toc.last  = 1;

  toc.tracks[1].valid = 1;
  toc.tracks[1].start = 150;
  toc.tracks[1].type  = 1;
}

auto MCD::CDD::eject() -> void {
  io.status = Status::NoDisc;
}

auto MCD::CDD::power(bool reset) -> void {
  fader = {};
  io = {};
  insert();
  for(uint index : range(10)) status [index] = 0x0;
  for(uint index : range(10)) command[index] = 0x0;
  checksum();
}
