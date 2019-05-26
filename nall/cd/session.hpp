#pragma once

//Table of Contents (TOC) data structures

namespace nall::CD {

inline auto toLBA(uint8_t minute, uint8_t second, uint8_t frame) -> int {
  if(minute > 99 || second > 59 || frame > 74) return -1;
  return minute * 60 * 75 + second * 75 + frame;
}

inline auto toMSF(int lba, uint8_t& minute, uint8_t& second, uint8_t& frame) -> bool {
  if(lba < 0 || lba >= 100 * 60 * 75) return false;
  minute = lba / 75 / 60 % 100;
  second = lba / 75 % 60;
  frame  = lba % 75;
  return true;
}

struct Index {
  int offset = -1;
  int length = -1;

  explicit operator bool() const {
    return offset >= 0 && length > 0;
  }

  auto lba() const -> int {
    return offset;
  }

  auto end() const -> int {
    return offset + length;
  }

  auto inRange(int lba) const -> bool {
    if(offset < 0 || length <= 0) return false;
    return lba >= offset && lba < offset + length;
  }
};

struct Track {
  Index indices[100];
  uint8_t control = 0b1111;
  uint8_t qMode = 0b1111;

  explicit operator bool() const {
    return (control & 0b1100) != 0b1100;
  }

  auto lba() const -> int {
    return indices[firstIndex()].lba();
  }

  auto end() const -> int {
    return indices[lastIndex()].end();
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

  auto firstIndex() const -> uint8_t {
    for(uint8_t index : range(100)) {
      if(indices[index]) return index;
    }
    return 0;
  }

  auto lastIndex() const -> uint8_t {
    for(uint8_t index : reverse(range(100))) {
      if(indices[index]) return index;
    }
    return 0;
  }

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

  auto end() const -> int {
    auto& track = tracks[lastTrack()];
    auto& index = track.indices[track.lastIndex()];
    return index.offset + index.length;
  }

  auto firstTrack() const -> uint8_t {
    for(uint8_t track : range(100)) {
      if(tracks[track]) return track;
    }
    return 0;
  }

  auto lastTrack() const -> uint8_t {
    for(uint8_t track : reverse(range(100))) {
      if(tracks[track]) return track;
    }
    return 0;
  }

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

  auto load(Markup::Node session) -> bool {
    *this = {};  //reset all member variables

    for(auto& track : session.find("track")) {
      uint trackID = track.natural();
      if(trackID > 99) return false;
      if(track["audio"]) tracks[trackID].control = 0b0000;
      if(track["data" ]) tracks[trackID].control = 0b0100;
      for(auto& index : track.find("index")) {
        uint indexID = index.natural();
        if(indexID > 99) return false;
        auto msf = index["msf"].text().split(":");
        if(msf.size() != 3) return false;
        auto minute = msf[0].natural();
        auto second = msf[1].natural();
        auto frame  = msf[2].natural();
        if(minute > 99 || second > 59 || frame > 74) return false;
        int lba = minute * 60 * 75 + second * 75 + frame;
        tracks[trackID].indices[indexID].offset = lba;
      }
    }

    Index* previous = nullptr;
    for(auto& track : tracks) {
      if(!track) continue;
      for(auto& index : track.indices) {
        if(index.offset < 0) continue;
        if(previous) {
          previous->length = index.offset - previous->offset;
        }
        previous = &index;
      }
    }

    if(!previous) return false;
    auto msf = session["msf"].text().split(":");
    if(msf.size() != 3) return false;
    auto minute = msf[0].natural();
    auto second = msf[1].natural();
    auto frame  = msf[2].natural();
    if(minute > 99 || second > 59 || frame > 74) return false;
    int lba = minute * 60 * 75 + second * 75 + frame;
    previous->length = lba - previous->offset;

    return true;
  }
};

}
