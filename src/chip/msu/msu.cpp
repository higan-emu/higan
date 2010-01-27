#include <../base.hpp>

#define MSU_CPP
namespace SNES {

MSU msu;

#include "serialization.cpp"

void MSU::enter() {
  scheduler.clock.cop_freq = 44100;

  while(true) {
    if(scheduler.sync == Scheduler::SyncAll) {
      scheduler.exit(Scheduler::SynchronizeEvent);
    }

    int16 left = 0, right = 0;

    if(mmio.audio_play) {
      if(audiofile.open()) {
        if(audiofile.end()) {
          if(!mmio.audio_repeat) mmio.audio_play = false;
          audiofile.seek(mmio.audio_offset = 58);
        } else {
          mmio.audio_offset += 4;
          left  = audiofile.readl(2);
          right = audiofile.readl(2);
        }
      } else {
        mmio.audio_play = false;
      }
    }

    left  = sclamp<16>((double)left  * (double)mmio.audio_volume / 255.0);
    right = sclamp<16>((double)right * (double)mmio.audio_volume / 255.0);

    audio.coprocessor_sample(left, right);
    scheduler.addclocks_cop(1);
    scheduler.sync_copcpu();
  }
}

void MSU::init() {
}

void MSU::enable() {
  audio.coprocessor_enable(true);
  audio.coprocessor_frequency(44100.0);

  for(unsigned i = 0x2200; i <= 0x2207; i++) {
    memory::mmio.map(i, *this);
  }

  if(datafile.open()) datafile.close();
  datafile.open(string() << basename << ".msu", file::mode_read);
}

void MSU::power() {
  reset();
}

void MSU::reset() {
  mmio.data_offset  = 0;
  mmio.audio_offset = 0;
  mmio.audio_track  = 0;
  mmio.audio_volume = 255;
  mmio.data_busy    = true;
  mmio.audio_busy   = true;
  mmio.audio_repeat = false;
  mmio.audio_play   = false;
}

uint8 MSU::mmio_read(unsigned addr) {
  addr &= 0xffff;

  if(addr == 0x2200) {
    return (mmio.data_busy    << 7)
         | (mmio.audio_busy   << 6)
         | (mmio.audio_repeat << 5)
         | (mmio.audio_play   << 4)
         | (Revision          << 0);
  }

  if(addr == 0x2201) {
    if(mmio.data_busy) return 0x00;
    mmio.data_offset++;
    if(datafile.open()) return datafile.read();
    return 0x00;
  }

  if(addr == 0x2202) return 'S';
  if(addr == 0x2203) return '-';
  if(addr == 0x2204) return 'M';
  if(addr == 0x2205) return 'S';
  if(addr == 0x2206) return 'U';
  if(addr == 0x2207) return '0' + Revision;

  return 0x00;
}

void MSU::mmio_write(unsigned addr, uint8 data) {
  addr &= 0xffff;

  if(addr == 0x2200) {
    mmio.data_offset = (mmio.data_offset & 0xffffff00) | (data <<  0);
  }

  if(addr == 0x2201) {
    mmio.data_offset = (mmio.data_offset & 0xffff00ff) | (data <<  8);
  }

  if(addr == 0x2202) {
    mmio.data_offset = (mmio.data_offset & 0xff00ffff) | (data << 16);
  }

  if(addr == 0x2203) {
    mmio.data_offset = (mmio.data_offset & 0x00ffffff) | (data << 24);
    if(datafile.open()) datafile.seek(mmio.data_offset);
    mmio.data_busy = false;
  }

  if(addr == 0x2204) {
    mmio.audio_track = (mmio.audio_track & 0xff00) | (data << 0);
  }

  if(addr == 0x2205) {
    mmio.audio_track = (mmio.audio_track & 0x00ff) | (data << 8);
    if(audiofile.open()) audiofile.close();
    char track[16];
    sprintf(track, "-%u", mmio.audio_track);
    if(audiofile.open(string() << basename << track << ".wav", file::mode_read)) {
      audiofile.seek(mmio.audio_offset = 58);  //skip WAV header
    }
    mmio.audio_busy   = false;
    mmio.audio_repeat = false;
    mmio.audio_play   = false;
  }

  if(addr == 0x2206) {
    mmio.audio_volume = data;
  }

  if(addr == 0x2207) {
    mmio.audio_repeat = data & 2;
    mmio.audio_play   = data & 1;
  }
}

void MSU::base(const string& name) {
  basename = name;
}

bool MSU::exists() {
  return file::exists(string() << basename << ".msu");
}

}