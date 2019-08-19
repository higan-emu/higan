auto System::portRead(uint16 address) -> uint8 {
  uint8 data;

  //DISP_MODE
  if(address == 0x0060) {
    data.bit(0) = r.unknown;
    data.bit(5) = r.format;
    data.bit(6) = r.depth;
    data.bit(7) = r.color;
    return data;
  }

  //IEEP_DATA
  if(address == 0x00ba) return eeprom.read(EEPROM::DataLo);
  if(address == 0x00bb) return eeprom.read(EEPROM::DataHi);

  //IEEP_ADDR
  if(address == 0x00bc) return eeprom.read(EEPROM::AddressLo);
  if(address == 0x00bd) return eeprom.read(EEPROM::AddressHi);

  //IEEP_STATUS
  if(address == 0x00be) return eeprom.read(EEPROM::Status);

  return data;
}

auto System::portWrite(uint16 address, uint8 data) -> void {
  //DISP_MODE
  if(address == 0x0060) {
    r.unknown = data.bit(0,4) & 0b01011;
    r.format  = data.bit(5);
    r.depth   = data.bit(6);
    r.color   = data.bit(7);
    return;
  }

  //IEEP_DATA
  if(address == 0x00ba) return eeprom.write(EEPROM::DataLo, data);
  if(address == 0x00bb) return eeprom.write(EEPROM::DataHi, data);

  //IEEP_ADDR
  if(address == 0x00bc) return eeprom.write(EEPROM::AddressLo, data);
  if(address == 0x00bd) return eeprom.write(EEPROM::AddressHi, data);

  //IEEP_CMD
  if(address == 0x00be) return eeprom.write(EEPROM::Command, data);
}
