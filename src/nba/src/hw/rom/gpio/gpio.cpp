/*
 * Copyright (C) 2021 fleroviux
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#include <nba/rom/gpio/gpio.hpp>

namespace nba {

void GPIO::Reset() {
  allow_reads = false;
  port_data = 0;
  for (int i = 0; i < 4; i++) {
    direction[i] = PortDirection::Out;
  }
  UpdateReadWriteMasks();
}

void GPIO::UpdateReadWriteMasks() {
  rd_mask = 0;
  for (int i = 0; i < 4; i++) {
    if (GetPortDirection(i) == PortDirection::In) rd_mask |= 1 << i;
  }
  wr_mask = ~rd_mask & 15;
}

auto GPIO::Read(u32 address) -> u8 {
  if (!allow_reads) {
    return 0;
  }

  switch (static_cast<Register>(address)) {
    case Register::Data: {
      auto value = ReadPort() & rd_mask;

      port_data &= wr_mask;
      port_data |= value;
      return value;
    }
    case Register::Direction: {
      return rd_mask;
    }
    case Register::Control: {
      return allow_reads ? 1 : 0;
    }
  }

  return 0;
}

void GPIO::Write(u32 address, u8 value) {
  switch (static_cast<Register>(address)) {
    case Register::Data: {
      port_data &= rd_mask;
      port_data |= wr_mask & value;
      WritePort(port_data);
      break;
    }
    case Register::Direction: {
      for (int i = 0; i < 4; i++) {
        direction[i] = (value & (1 << i)) ? PortDirection::Out : PortDirection::In;
      }
      UpdateReadWriteMasks();
      break;
    }
    case Register::Control: {
      allow_reads = value & 1;
      break;
    }
  }
}

} // namespace nba
