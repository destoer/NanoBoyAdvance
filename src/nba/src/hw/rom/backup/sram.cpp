/*
 * Copyright (C) 2021 fleroviux
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#include <nba/rom/backup/sram.hpp>

namespace nba {

SRAM::SRAM(std::string const& save_path)
    : save_path(save_path) {
  Reset();
}

void SRAM::Reset() {
  int bytes = 32768;
  file = BackupFile::OpenOrCreate(save_path, { 32768 }, bytes);
}

auto SRAM::Read(u32 address) -> u8 {
  return file->Read(address & 0x7FFF);
}

void SRAM::Write(u32 address, u8 value) {
  file->Write(address & 0x7FFF, value);
}

} // namespace nba
