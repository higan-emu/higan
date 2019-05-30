#pragma once

//Table of Contents (TOC) data structures

namespace nall::CD {

struct MSF {
  uint8_t minute;      //00-99
  uint8_t second;      //00-59
  uint8_t frame = -1;  //00-74

  MSF() = default;
  MSF(uint8_t m, uint8_t s, uint8_t f) : minute(m), second(s), frame(f) {}
  MSF(int lba) { *this = fromLBA(lba); }
  MSF(const string& input) { *this = fromString(input); }

  auto minuteBCD() const -> uint8_t { return (minute / 10) << 4 | (minute % 10); }
  auto secondBCD() const -> uint8_t { return (second / 10) << 4 | (second % 10); }
  auto frameBCD()  const -> uint8_t { return (frame  / 10) << 4 | (frame  % 10); }

  explicit operator bool() const {
    return minute <= 99 && second <= 59 && frame <= 74;
  }

  static auto fromBCD(uint8_t minute, uint8_t second, uint8_t frame) -> MSF {
    minute = (minute >> 4) * 10 + (minute & 15);
    second = (second >> 4) * 10 + (second & 15);
    frame  = (frame  >> 4) * 10 + (frame  & 15);
    return {minute, second, frame};
  }

  static auto fromLBA(int lba) -> MSF {
    if(lba < 0) lba = 100 * 60 * 75 + lba;
    if(lba >= 100 * 60 * 75) return {};
    uint8_t minute = lba / 75 / 60 % 100;
    uint8_t second = lba / 75 % 60;
    uint8_t frame  = lba % 75;
    return {minute, second, frame};
  }

  static auto fromString(const string& input) -> MSF {
    auto msf = input.split(":");
    if(msf.size() != 3) return {};
    uint8_t minute = msf[0].natural();
    uint8_t second = msf[1].natural();
    uint8_t frame  = msf[2].natural();
    return {minute, second, frame};
  }

  auto toLBA() const -> int {
    int lba = minute * 60 * 75 + second * 75 + frame;
    if(minute < 90) return lba;
    return -(100 * 60 * 75 - lba);
  }

  auto toString() const -> string {
    return {pad(minute, 2, '0'), ":", pad(second, 2, '0'), ":", pad(frame, 2, '0')};
  }
};

struct Index {
  int offset = 100 * 60 * 75;
  int length = -1;

  explicit operator bool() const {
    return length >= 0;
  }

  auto lba() const -> int { return offset; }
  auto len() const -> int { return length; }
  auto end() const -> int { return offset + length - 1; }

  auto inRange(int lba) const -> bool {
    if(length < 0) return false;
    return lba >= offset && lba <= offset + length - 1;
  }
};

struct Track : Index {
  uint8_t control = 0b1111;  //4-bit
  uint8_t address = 0b1111;  //4-bit

  explicit operator bool() const {
    return (control & 0b1100) != 0b1100;
  }

  auto emphasis() const -> bool {
    return control & 1;
  }

  auto copyable() const -> bool {
    return control & 2;
  }

  auto channels() const -> uint {
    if((control & 0b1100) == 0b0000) return 2;
    if((control & 0b1100) == 0b1000) return 4;
    return 0;  //data track or reserved
  }

  auto isAudio() const -> bool {
    return channels() != 0;
  }

  auto isData() const -> bool {
    return (control & 0b1100) == 0b0100;
  }
};

struct Session {
  Index leadIn;       //00
  Track tracks[100];  //01-99
  Index leadOut;      //aa
  uint8_t firstTrack = 0x00;
  uint8_t lastTrack  = 0x00;

  auto isLeadIn(int lba) const -> bool {
    return leadIn.inRange(lba);
  }

  auto isTrack(int lba) const -> maybe<uint8_t> {
    for(uint8_t track : range(100)) {
      if(tracks[track].inRange(lba)) return track;
    }
    return {};
  }

  auto isLeadOut(int lba) const -> bool {
    return leadOut.inRange(lba);
  }

  //recomputes lengths for track indices; requires valid lead-out index.
  //used to convert encodings of only MSF values to offset:length pairs.
  auto computeLengths() -> void {
    Index* previous = nullptr;
    for(auto& track : tracks) {
      if(!track) continue;
      if(!MSF(track.offset)) continue;
      if(previous) previous->length = track.offset - previous->offset;
      previous = &track;
    }
    if(previous) previous->length = leadOut.offset - previous->offset;
  }

  static auto decodeTOC(array_view<uint8_t> data, uint leadInSectors, uint totalSectors) -> Session {
    Session session;
    if(data.size() != leadInSectors * 96) return session;
    session.leadIn = {-int(leadInSectors), int(leadInSectors)};
    session.leadOut = {0, int(totalSectors - leadInSectors)};
    for(uint lba : range(leadInSectors)) {
      uint index = lba * 96 + 12;
      auto crc16 = CRC16({&data[index], 10});
      if(data[index + 10] != uint8_t(crc16 >> 8)) continue;
      if(data[index + 11] != uint8_t(crc16 >> 0)) continue;

      uint8_t control = data[index + 0] >> 4;
      uint8_t address = data[index + 0] & 15;
      uint8_t trackID = (data[index + 2] >> 4) * 10 + (data[index + 2] & 15);

      if(trackID <= 99 && address == 0b0001) {  //0x00-0x99
        auto& track = session.tracks[trackID];
        track.control = control;
        track.address = address;
        track.offset = MSF::fromBCD(data[index + 7], data[index + 8], data[index + 9]).toLBA();
      }

      if(trackID == 100) {  //0xa0
        auto track = data[index + 7];
        session.firstTrack = (track >> 4) * 10 + (track & 15);
      }

      if(trackID == 101) {  //0xa1
        auto track = data[index + 7];
        session.lastTrack = (track >> 4) * 10 + (track & 15);
      }

      if(trackID == 102) {  //0xa2
        auto msf = MSF::fromBCD(data[index + 7], data[index + 8], data[index + 9]);
        session.leadOut.offset = msf.toLBA();
      }
    }
    session.computeLengths();
    return session;
  }

  auto encodeTOC() const -> vector<uint8_t> {
    vector<uint8_t> data;
    data.resize(7500 * 96);
    uint8_t trackID = 0;
    int lba = 0;
    while(lba < leadIn.len()) {

    //tracks
    for(uint trackID : range(100)) {
    for(uint repeat : range(3)) {
      auto& track = tracks[trackID];
      if(!track) continue;
      uint index = lba * 96 + 12;
      data[index + 0] = track.control << 4 | track.address << 0;
      data[index + 1] = 0x00;
      data[index + 2] = trackID / 10 << 4 | trackID % 10;
      auto msf = MSF(leadIn.lba() + lba);
      data[index + 3] = msf.minuteBCD();
      data[index + 4] = msf.secondBCD();
      data[index + 5] = msf.frameBCD();
      data[index + 6] = 0x00;
      msf = MSF(track.lba());
      data[index + 7] = msf.minuteBCD();
      data[index + 8] = msf.secondBCD();
      data[index + 9] = msf.frameBCD();
      auto crc16 = CRC16({&data[index], 10});
      data[index + 10] = crc16 >> 8;
      data[index + 11] = crc16 >> 0;
      if(++lba >= leadIn.len()) break;
    }}
    if(lba >= leadIn.len()) break;

    //first track
    for(uint repeat : range(3)) {
      uint index = lba * 96 + 12;
      data[index + 0] = 0x01;  //control value unverified; address = 1
      data[index + 1] = 0x00;  //track# = 00 (TOC)
      data[index + 2] = 0xa0;  //first track
      auto msf = MSF(leadIn.lba() + lba);
      data[index + 3] = msf.minuteBCD();
      data[index + 4] = msf.secondBCD();
      data[index + 5] = msf.frameBCD();
      data[index + 6] = 0x00;
      data[index + 7] = firstTrack / 10 << 4 | firstTrack % 10;
      data[index + 8] = 0x00;
      data[index + 9] = 0x00;
      auto crc16 = CRC16({&data[index], 10});
      data[index + 10] = crc16 >> 8;
      data[index + 11] = crc16 >> 0;
      if(++lba >= leadIn.len()) break;
    }
    if(lba >= leadIn.len()) break;

    //last track
    for(uint repeat : range(3)) {
      uint index = lba * 96 + 12;
      data[index + 0] = 0x01;
      data[index + 1] = 0x00;
      data[index + 2] = 0xa1;  //last track
      auto msf = MSF(leadIn.lba() + lba);
      data[index + 3] = msf.minuteBCD();
      data[index + 4] = msf.secondBCD();
      data[index + 5] = msf.frameBCD();
      data[index + 6] = 0x00;
      data[index + 7] = lastTrack / 10 << 4 | lastTrack % 10;
      data[index + 8] = 0x00;
      data[index + 9] = 0x00;
      auto crc16 = CRC16({&data[index], 10});
      data[index + 10] = crc16 >> 8;
      data[index + 11] = crc16 >> 0;
      if(++lba >= leadIn.len()) break;
    }
    if(lba >= leadIn.len()) break;

    //lead-out point
    for(uint repeat : range(3)) {
      uint index = lba * 96 + 12;
      data[index + 0] = 0x01;
      data[index + 1] = 0x00;
      data[index + 2] = 0xa2;  //lead-out point
      auto msf = MSF(leadIn.lba() + lba);
      data[index + 3] = msf.minuteBCD();
      data[index + 4] = msf.secondBCD();
      data[index + 5] = msf.frameBCD();
      data[index + 6] = 0x00;
      msf = MSF(leadOut.lba());
      data[index + 7] = msf.minuteBCD();
      data[index + 8] = msf.secondBCD();
      data[index + 9] = msf.frameBCD();
      auto crc16 = CRC16({&data[index], 10});
      data[index + 10] = crc16 >> 8;
      data[index + 11] = crc16 >> 0;
      if(++lba >= leadIn.len()) break;
    }
    if(lba >= leadIn.len()) break;

    }
    return data;
  }
};

}
