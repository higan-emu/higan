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
    io.sample = 0;
    io.track  = 1;
  } break;

  case Status::ReadingTOC: {
    io.sector++;
    if(io.sector >= 150) io.status = Status::Paused;
    if(io.sector >= 150) io.track  = 1;
  } break;

  case Status::Playing: {
    if(io.latency && --io.latency) break;
    if(session.tracks[io.track].isAudio()) break;
    mcd.cdc.decode(io.sector);
    io.sector++;
    if(io.sector >= session.tracks[io.track].end()) io.status = Status::Paused;
  } break;

  case Status::Seeking: {
    if(io.latency && --io.latency) break;
    io.status = Status::Paused;
  } break;

  }
}

auto MCD::CDD::sample() -> void {
  int16 left  = 0;
  int16 right = 0;
  if(io.status == Status::Playing && !io.latency) {
    if(session.tracks[io.track].isAudio()) {
      mcd.fd->seek((session.leadIn.len() + io.sector) * 2448 + io.sample);
      left  = mcd.fd->readl(2);
      right = mcd.fd->readl(2);
      io.sample += 4;
      if(io.sample >= 2352) {
        io.sample = 0;
        io.sector++;
        if(io.sector >= session.tracks[io.track].end()) io.status = Status::Paused;
      }
    }
  }
  stream->sample(left / 32768.0, right / 32768.0);
}

auto MCD::CDD::process() -> void {
//if(command[0]) print("CDD ", command[0], ":", command[3], "\n");

  if(!valid()) {
    //unverified
    print("CDD checksum error\n");
    io.status = Status::ChecksumError;
  }

  else
  switch(command[0]) {

  case Command::Idle: {
    //fixes Lunar: The Silver Star
    if(!io.latency && status[1] == 0xf) {
      status[1] = 0x2;
      status[2] = io.track / 10; status[3] = io.track % 10;
    }
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
      auto [minute, second, frame] = CD::MSF(io.sector);
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  % 10;
      status[8] = session.tracks[io.track].isData() << 2;
    } break;

    case Request::RelativeTime: {
      auto [minute, second, frame] = CD::MSF(io.sector - session.tracks[io.track].indices[1].lba());
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  / 10;
      status[8] = session.tracks[io.track].isData() << 2;
    } break;

    case Request::TrackInformation: {
      status[1] = command[3];
      status[2] = io.track / 10; status[3] = io.track % 10;
      status[4] = 0x0; status[5] = 0x0;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case Request::DiscCompletionTime: {  //time in mm:ss:ff
      auto [minute, second, frame] = CD::MSF(session.leadOut.lba());
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  % 10;
      status[8] = 0x0;
    } break;

    case Request::DiscTracks: {  //first and last track numbers
      auto firstTrack = session.firstTrackID();
      auto lastTrack  = session.lastTrackID();
      status[1] = command[3];
      status[2] = firstTrack / 10; status[3] = firstTrack % 10;
      status[4] = lastTrack  / 10; status[5] = lastTrack  % 10;
      status[6] = 0x0; status[7] = 0x0;
      status[8] = 0x0;
    } break;

    case Request::TrackStartTime: {
      uint track  = command[4] * 10 + command[5];
      auto [minute, second, frame] = CD::MSF(session.tracks[track].indices[1].lba());
      status[1] = command[3];
      status[2] = minute / 10; status[3] = minute % 10;
      status[4] = second / 10; status[5] = second % 10;
      status[6] = frame  / 10; status[7] = frame  % 10;
      status[6].bit(3) = session.tracks[track].isData();
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

    counter    = 0;
    io.status  = Status::Playing;
    io.latency = 11;
    io.sector  = minute * 60 * 75 + second * 75 + frame - 3;
    io.sample  = 0;
    if(auto track = session.toTrack(io.sector)) io.track = track();

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
     int lba    = minute * 60 * 75 + second * 75 + frame - 3;

    counter    = 0;
    io.status  = Status::Seeking;
    io.latency = abs(io.sector - lba) * 120 / 270000;
    io.sector  = lba;
    io.sample  = 0;
    if(auto track = session.toTrack(io.sector)) io.track = track();

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
  if(!mcd.disc) {
    io.status = Status::NoDisc;
    return;
  }

  io.status = Status::ReadingTOC;
  io.sector = 0;
  io.sample = 0;
  io.track  = 1;
}

auto MCD::CDD::eject() -> void {
  session = {};

  io.status = Status::NoDisc;
  io.sector = 0;
  io.sample = 0;
  io.track  = 1;
}

auto MCD::CDD::power(bool reset) -> void {
  stream = audio.createStream(2, 44100);
  fader = {};
  io = {};
  insert();
  for(uint index : range(10)) status [index] = 0x0;
  for(uint index : range(10)) command[index] = 0x0;
  checksum();
}
