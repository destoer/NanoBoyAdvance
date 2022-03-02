/*
 * Copyright (C) 2021 fleroviux
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once

#include <nba/integer.hpp>

#include "hw/apu/apu.hpp"
#include "hw/irq/irq.hpp"
#include "scheduler.hpp"

namespace nba::core {

struct Timer {
  Timer(Scheduler& scheduler, IRQ& irq, APU& apu)
      : scheduler(scheduler)
      , irq(irq)
      , apu(apu) {
    Reset();
  }

  void Reset();
  auto Read (int chan_id, int offset) -> u8;
  void Write(int chan_id, int offset, u8 value);
  void Step(int cycles);

private:
  enum Registers {
    REG_TMXCNT_L = 0,
    REG_TMXCNT_H = 2
  };

  struct Channel {
    int id;
    u16 reload = 0;
    u32 counter = 0;
    u16 latch_reload;

    struct Control {
      int frequency = 0;
      bool cascade = false;
      bool interrupt = false;
      bool enable = false;
    } control = {};

    bool running = false;
    int shift;
    int mask;
    int samplerate;
    u64 timestamp_started;
    Scheduler::Event* event = nullptr;
    std::function<void(int)> event_cb;
    int delay;
  } channels[4];

  Scheduler& scheduler;
  IRQ& irq;
  APU& apu;
  int prescalers[4];

  auto GetCounterDeltaSinceLastUpdate(Channel const& channel) -> u32;
  void StartChannel(Channel& channel, int cycles_late);
  void StopChannel(Channel& channel);
  void OnOverflow(Channel& channel);
};

} // namespace nba::core
