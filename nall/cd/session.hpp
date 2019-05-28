#pragma once

//Table of Contents (TOC) data structures

namespace nall::CD {

//always stored in binary format; never in BCD format
struct MSF {
  uint8_t minute;      //00-99
  uint8_t second;      //00-59
  uint8_t frame = -1;  //00-74

  MSF() = default;
  MSF(uint8_t m, uint8_t s, uint8_t f) : minute(m), second(s), frame(f) {}
  MSF(int lba) { *this = fromLBA(lba); }
  MSF(const string& input) { *this = fromString(input); }

  explicit operator bool() const {
    return minute <= 99 && second <= 59 && frame <= 74;
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

struct Track {
  uint8_t control = 0b1111;
  uint8_t qMode = 0b1111;
  Index indices[100];

  explicit operator bool() const {
    return (control & 0b1100) != 0b1100;
  }

  auto lba() const -> int { return firstIndex().lba(); }
  auto len() const -> int { return lastIndex().end() - firstIndex().lba() + 1; }
  auto end() const -> int { return lastIndex().end(); }

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

  auto firstIndexID() const -> uint8_t {
    for(uint8_t index : range(100)) {
      if(indices[index]) return index;
    }
    return 0;
  }
  auto firstIndex() -> Index& { return indices[firstIndexID()]; }
  auto firstIndex() const -> const Index& { return indices[firstIndexID()]; }

  auto lastIndexID() const -> uint8_t {
    for(uint8_t index : reverse(range(100))) {
      if(indices[index]) return index;
    }
    return 0;
  }

  auto lastIndex() -> Index& { return indices[lastIndexID()]; }
  auto lastIndex() const -> const Index& { return indices[lastIndexID()]; }

  auto toIndex(int lba) const -> maybe<uint8_t> {
    for(uint8_t index : range(100)) {
      if(indices[index].inRange(lba)) return index;
    }
    return {};
  }
};

struct Session {
  Index leadIn;
  Track tracks[100];
  Index leadOut;

  auto firstTrackID() const -> uint8_t {
    for(uint8_t track : range(100)) {
      if(tracks[track]) return track;
    }
    return 0;
  }
  auto firstTrack() -> Track& { return tracks[firstTrackID()]; }
  auto firstTrack() const -> const Track& { return tracks[firstTrackID()]; }

  auto lastTrackID() const -> uint8_t {
    for(uint8_t track : reverse(range(100))) {
      if(tracks[track]) return track;
    }
    return 0;
  }
  auto lastTrack() -> Track& { return tracks[lastTrackID()]; }
  auto lastTrack() const -> const Track& { return tracks[lastTrackID()]; }

  auto isLeadIn(int lba) const -> bool {
    return leadIn.inRange(lba);
  }

  auto toTrack(int lba) const -> maybe<uint8_t> {
    for(uint8_t track : range(100)) {
      if(tracks[track].toIndex(lba)) return track;
    }
    return {};
  }

  auto inLeadOut(int lba) const -> bool {
    return leadOut.inRange(lba);
  }

  //recomputes lengths for track indices; requires valid lead-out index.
  //used to convert encodings of only MSF values to offset:length pairs.
  auto computeLengths() -> void {
    Index* previous = nullptr;
    for(auto& track : tracks) {
      if(!track) continue;
      for(auto& index : track.indices) {
        if(!MSF(index.offset)) continue;
        if(previous) previous->length = index.offset - previous->offset;
        previous = &index;
      }
    }
    if(previous) previous->length = leadOut.offset - previous->offset;
  }

  auto serialize() const -> string {
    string s;
    s.append("session\n");
    s.append("  leadin\n");
    s.append("    msf: ", MSF(leadIn.lba()).toString(), "\n");
    for(uint trackID : range(100)) {
      auto& track = tracks[trackID];
      if(!track) continue;
      s.append("  track: ", pad(trackID, 2, '0'), "\n");
      if(track.isAudio()) s.append("    audio\n");
      if(track.isData())  s.append("    data\n");
      for(uint indexID : range(100)) {
        auto& index = track.indices[indexID];
        if(!index) continue;
        s.append("    index: ", pad(indexID, 2, '0'), "\n");
        s.append("      msf: ", MSF(index.lba()).toString(), "\n");
      }
    }
    s.append("  leadout\n");
    s.append("    msf: ", MSF(leadOut.lba()).toString(), "\n");
    s.append("  end\n");
    s.append("    msf: ", MSF(leadIn.len() + leadOut.end() + 1).toString(), "\n");
    return s;
  }

  auto unserialize(string markup) -> bool {
    *this = {};  //reset all member variables

    auto document = BML::unserialize(markup);
    auto session = document["session"];
    if(!session["leadin"] || !session["track"] || !session["leadout"] || !session["end"]) return false;

    int leadin = MSF::fromString(session["leadin/msf"].text()).toLBA();
    leadIn = {leadin, -leadin};

    int leadout = MSF::fromString(session["leadout/msf"].text()).toLBA();
    int end = MSF::fromString(session["end/msf"].text()).toLBA();
    leadOut = {leadout, end - leadout + leadin};

    for(auto& track : session.find("track")) {
      uint trackID = track.natural();
      if(trackID > 99) return false;
      if(track["audio"]) tracks[trackID].control = 0b0000;
      if(track["data" ]) tracks[trackID].control = 0b0100;
      for(auto& index : track.find("index")) {
        uint indexID = index.natural();
        if(indexID > 99) return false;
        tracks[trackID].indices[indexID].offset = MSF(index["msf"].text()).toLBA();
      }
    }

    computeLengths();
    return true;
  }
};

}
