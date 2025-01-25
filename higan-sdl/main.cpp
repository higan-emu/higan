#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <nall/nall.hpp>
#include <nall/main.hpp>
#include <higan/higan.hpp>
#include <cv/interface/interface.hpp>
#include <fc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <ms/interface/interface.hpp>
#include <msx/interface/interface.hpp>
#include <n64/interface/interface.hpp>
#include <ngp/interface/interface.hpp>
#include <pce/interface/interface.hpp>
#include <ps1/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <sg/interface/interface.hpp>
#include <ws/interface/interface.hpp>

shared_pointer<higan::Interface> interface;

struct Emulator : higan::Platform {
  bool create(string location) {
    higan::platform = this;
    tick = SDL_GetTicks();

    // load system
    string configuration = file::read({location, "settings.bml"});
    interface->load(root);
    root->copy(higan::Node::unserialize(configuration));
    interface->power();

    // hack to switch button mapping order for some systems
    auto system = interface->name();
    button.order = Buttons::Order::BA;
    if (system.beginsWith("MSX") || system.beginsWith("Neo Geo")) {
      button.order = Buttons::Order::AB;
    } else if (system.beginsWith("Nintendo 64")) {
      button.order = Buttons::Order::Nintendo64;
    } else if (system.beginsWith("Mega Drive")) {
      button.order = Buttons::Order::MegaDrive;
    }

    // create window
    window = SDL_CreateWindow(
      string{interface->game(), " - higan-sdl"},
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      100, 100, SDL_WINDOW_HIDDEN
    );

    // open audio device
    SDL_AudioSpec want{}, have{};
    want.freq = 48000;
    want.format = AUDIO_F32SYS;
    want.channels = 2;
    want.samples = 512;
    if (audioDev = SDL_OpenAudioDevice(NULL,0,&want,&have,0); audioDev == 0) {
      print("failed to open audio device: ", SDL_GetError());
    } else {
      SDL_PauseAudioDevice(audioDev, 0);
    }

    return true;
  }

  bool destroy() {
    unload();
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audioDev);
    return true;
  }

  bool run() {
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
      switch(ev.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        bool value = ev.type == SDL_KEYDOWN;
        switch (ev.key.keysym.sym) {
        case SDLK_w:         button.up    = value; break;
        case SDLK_s:         button.down  = value; break;
        case SDLK_a:         button.left  = value; break;
        case SDLK_d:         button.right = value; break;
        case SDLK_j:         button.a     = value; break;
        case SDLK_k:         button.b     = value; break;
        case SDLK_u:         button.x     = value; break;
        case SDLK_i:         button.y     = value; break;
        case SDLK_BACKSPACE: button.back  = value; break;
        case SDLK_RETURN:    button.start = value; break;
        case SDLK_q:         button.lb    = value; break;
        case SDLK_z:         button.lt    = value; break;
        case SDLK_e:         button.rb    = value; break;
        case SDLK_x:         button.rt    = value; break;
        }
        break;
      }

      case SDL_QUIT:
        return false;
      }
    }

    if(interface) interface->run();
    return !!interface;
  }

  void sync() {
    constexpr auto audioBufLowWatermark = 1024 * sizeof(float) * 2;
    constexpr auto audioBufHighWatermark = 2048 * sizeof(float) * 2;
    constexpr auto audioBufMaxWatermark = 4096 * sizeof(float) * 2;
    int32_t tickdelta = sampleCount / 48;
    sampleCount %= 48;
    if (audioDev) {
      auto qlen = SDL_GetQueuedAudioSize(audioDev);
      if (qlen < audioBufLowWatermark) tickdelta -= 1;
      if (qlen > audioBufHighWatermark) tickdelta += 1;
      if (qlen > audioBufMaxWatermark) SDL_ClearQueuedAudio(audioDev);
      tickdelta = max(tickdelta, 0);
    }
    tick += tickdelta;
    if (auto currentTick = SDL_GetTicks(); tick > currentTick) {
      SDL_Delay(tick - currentTick);
    } else if (currentTick - tick > 125) {
      tick = SDL_GetTicks();
    }
  }

  void attach(higan::Node::Object node) {
    if(interface && node->is<higan::Node::Screen>()) {
      screens = root->find<higan::Node::Screen>();
    }
    if(interface && node->is<higan::Node::Stream>()) {
      streams = root->find<higan::Node::Stream>();
    }
  }

  void detach(higan::Node::Object node) {
    if(interface && node->is<higan::Node::Screen>()) {
      screens = root->find<higan::Node::Screen>();
    }
    if(interface && node->is<higan::Node::Stream>()) {
      streams = root->find<higan::Node::Stream>();
    }
  }

  shared_pointer<vfs::file> open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) override {
    using disk = vfs::disk;
    if(auto r = disk::open({node->attribute("location"),name}, mode)) return r;
    if(auto r = disk::open({root->attribute("location"),name}, mode)) return r;
    print("missing ", required? "required":"optional", " file: ", name, "\n");
    return {};
  }

  void event(higan::Event) override {}

  void log(string_view message) override {}

  void video(higan::Node::Screen, const u32* data, uint pitch, uint width, uint height) override {
    if (w != width || h != height) {
      w = width, h = height;
      SDL_SetWindowSize(window, w * scale, h * scale);
      SDL_ShowWindow(window);
    }
    auto b = SDL_CreateRGBSurfaceFrom(const_cast<u32*>(data), w, h, 32, pitch,
                                      0xff<<16, 0xff<<8, 0xff, 0);
    if (!b) return (void)print("error creating surface: ",SDL_GetError(),"\n");
    SDL_BlitScaled(b, nullptr, SDL_GetWindowSurface(window), nullptr);
    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(b);
    sync();
  }

  void audio(higan::Node::Stream) override {
    if(!streams) return;

    vector<float> data;
    while(true) {
      for(auto& stream : streams) {
        if(!stream->pending()) {
          if (audioDev && data.size() != 0) {
            SDL_QueueAudio(audioDev, data.data(), data.size()*sizeof(float));
          }
          return;
        }
      }
      double samples[2] = {0.0, 0.0};
      for(auto& stream : streams) {
        double buffer[2];
        auto channels = stream->read(buffer);
        for(uint c : range(2)) {
          samples[c] = buffer[c % channels];
        }
      }
      for(uint c : range(2)) {
        data.append(max(-1.0, min(+1.0, samples[c])));
      }
      sampleCount++;
    }
  }

  void input(higan::Node::Input node) override {
    if(auto b = node->cast<higan::Node::Button>()) {
      auto name = node->name();
      if(name == "Up"        ) b->setValue(button.up);
      if(name == "Down"      ) b->setValue(button.down);
      if(name == "Left"      ) b->setValue(button.left);
      if(name == "Right"     ) b->setValue(button.right);
      if(name == "1"         ) b->setValue(button.a);
      if(name == "2"         ) b->setValue(button.b);
      if(name == "II"        ) b->setValue(button.a);
      if(name == "I"         ) b->setValue(button.b);
      if(name == "C"         ) b->setValue(button.b);
      if(name == "Y1"        ) b->setValue(button.x);
      if(name == "Y2"        ) b->setValue(button.y);
      if(name == "Y3"        ) b->setValue(button.lb);
      if(name == "Y4"        ) b->setValue(button.rb);
      if(name == "X"         ) b->setValue(button.y);
      if(name == "X1"        ) b->setValue(button.up);
      if(name == "X2"        ) b->setValue(button.right);
      if(name == "X3"        ) b->setValue(button.down);
      if(name == "X4"        ) b->setValue(button.left);
      if(name == "Pass"      ) b->setValue(button.a);
      if(name == "Clear"     ) b->setValue(button.y);
      if(name == "View"      ) b->setValue(button.start);
      if(name == "Escape"    ) b->setValue(button.back);
      if(name == "Cross"     ) b->setValue(button.a);
      if(name == "Circle"    ) b->setValue(button.b);
      if(name == "Square"    ) b->setValue(button.x);
      if(name == "Triangle"  ) b->setValue(button.y);
      if(name == "Microphone") b->setValue(button.x);
      if(name == "L"         ) b->setValue(button.lb);
      if(name == "R"         ) b->setValue(button.rb);
      if(name == "L1"        ) b->setValue(button.lb);
      if(name == "L2"        ) b->setValue(button.lt);
      if(name == "R1"        ) b->setValue(button.rb);
      if(name == "R2"        ) b->setValue(button.rt);
      if(name == "Mode"      ) b->setValue(button.back);
      if(name == "Select"    ) b->setValue(button.back);
      if(name == "Start"     ) b->setValue(button.start);
      if(name == "Run"       ) b->setValue(button.start);
      if(name == "Pause"     ) b->setValue(button.start);
      if(name == "Option"    ) b->setValue(button.start);
      switch (button.order) {
      case Buttons::Order::AB:
        if(name == "A") b->setValue(button.a);
        if(name == "B") b->setValue(button.b);
        break;
      case Buttons::Order::BA:
        if(name == "B") b->setValue(button.a);
        if(name == "A") b->setValue(button.b);
        if(name == "Y") b->setValue(button.x);
        break;
      case Buttons::Order::MegaDrive:
        if(name == "A") b->setValue(button.x);
        if(name == "B") b->setValue(button.a);
        if(name == "Y") b->setValue(button.lb);
        if(name == "Z") b->setValue(button.rb);
        break;
      case Buttons::Order::Nintendo64:
        if(name == "B") b->setValue(button.x);
        if(name == "A") b->setValue(button.a);
        if(name == "Z") b->setValue(button.lt);
        break;
      }
    }
  }

  void unload() {
    if (interface) interface->unload();
  }

  higan::Node::Object root;

  vector<higan::Node::Screen> screens;
  vector<higan::Node::Stream> streams;

  SDL_Window *window = nullptr;
  SDL_AudioDeviceID audioDev = 0;
  u32 w = 0, h = 0, scale = 2, sampleCount = 0, tick;

  struct Buttons {
    bool up{}, down{}, left{}, right{};
    bool a{}, b{}, x{}, y{};
    bool lb{}, lt{}, rb{}, rt{};
    bool back{}, start{};
    enum class Order { AB, BA, MegaDrive, Nintendo64 } order;
  } button;
} emulator;

void nall::main(Arguments arguments) {
  auto location = arguments.take();
  if (!location) return (void)print("usage: higan-sdl <system folder>\n");
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
    return (void)print("error initializing SDL2: ", SDL_GetError(), "\n");
  }
  string system;
  if (auto f = file::read({location, "manifest.bml"}); f) {
    if (auto manifest = BML::unserialize(f); manifest) {
      system = manifest["system"].value().strip();
    } else {
      return (void)print("could not parse system manifest\n");
    }
  } else {
    return (void)print("could not open system manifest\n");
  }
  #ifdef CORE_CV
  if (interface = new higan::ColecoVision::ColecoVisionInterface; interface->name() == system); else
  #endif
  #ifdef CORE_FC
  if (interface = new higan::Famicom::FamicomInterface; interface->name() == system); else
  #endif
  #ifdef CORE_GB
  if (interface = new higan::GameBoy::GameBoyInterface; interface->name() == system); else
  #endif
  #ifdef CORE_GB
  if (interface = new higan::GameBoy::GameBoyColorInterface; interface->name() == system); else
  #endif
  #ifdef CORE_GBA
  if (interface = new higan::GameBoyAdvance::GameBoyAdvanceInterface; interface->name() == system); else
  #endif
  #ifdef CORE_GBA
  if (interface = new higan::GameBoyAdvance::GameBoyPlayerInterface; interface->name() == system); else
  #endif
  #ifdef CORE_MD
  if (interface = new higan::MegaDrive::MegaDriveInterface; interface->name() == system); else
  #endif
  #ifdef CORE_MS
  if (interface = new higan::MasterSystem::GameGearInterface; interface->name() == system); else
  #endif
  #ifdef CORE_MS
  if (interface = new higan::MasterSystem::MasterSystemInterface; interface->name() == system); else
  #endif
  #ifdef CORE_MSX
  if (interface = new higan::MSX::MSXInterface; interface->name() == system); else
  #endif
  #ifdef CORE_MSX
  if (interface = new higan::MSX::MSX2Interface; interface->name() == system); else
  #endif
  #ifdef CORE_N64
  if (interface = new higan::Nintendo64::Nintendo64Interface; interface->name() == system); else
  #endif
  #ifdef CORE_NGP
  if (interface = new higan::NeoGeoPocket::NeoGeoPocketInterface; interface->name() == system); else
  #endif
  #ifdef CORE_NGP
  if (interface = new higan::NeoGeoPocket::NeoGeoPocketColorInterface; interface->name() == system); else
  #endif
  #ifdef CORE_PCE
  if (interface = new higan::PCEngine::PCEngineInterface; interface->name() == system); else
  #endif
  #ifdef CORE_PCE
  if (interface = new higan::PCEngine::PCEngineDuoInterface; interface->name() == system); else
  #endif
  #ifdef CORE_PS1
  if (interface = new higan::PlayStation::PlayStationInterface; interface->name() == system); else
  #endif
  #ifdef CORE_WS
  if (interface = new higan::WonderSwan::PocketChallengeV2Interface; interface->name() == system); else
  #endif
  #ifdef CORE_SG
  if (interface = new higan::SG1000::SC3000Interface; interface->name() == system); else
  #endif
  #ifdef CORE_SG
  if (interface = new higan::SG1000::SG1000Interface; interface->name() == system); else
  #endif
  #ifdef CORE_SFC
  if (interface = new higan::SuperFamicom::SuperFamicomInterface; interface->name() == system); else
  #endif
  #ifdef CORE_PCE
  if (interface = new higan::PCEngine::SuperGrafxInterface; interface->name() == system); else
  #endif
  #ifdef CORE_WS
  if (interface = new higan::WonderSwan::SwanCrystalInterface; interface->name() == system); else
  #endif
  #ifdef CORE_WS
  if (interface = new higan::WonderSwan::WonderSwanInterface; interface->name() == system); else
  #endif
  #ifdef CORE_WS
  if (interface = new higan::WonderSwan::WonderSwanColorInterface; interface->name() == system); else
  #endif
  return (void)print("unknown or unsupported system: ", system, "\n");
  emulator.create(location);
  while (emulator.run());
  SDL_Quit();
}
