#include "fakeEEPROM.hpp"
#include <cstring>
#include <iostream>

using namespace std;
const char extEEPROM::_magicCharacter = '!';
const unsigned extEEPROM::_magicPacketLength = 4;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// Constructor.
// - deviceCapacity is the capacity of a single EEPROM device in
//   kilobits (kb) and should be one of the values defined in the
//   eeprom_size_t enumeration in the extEEPROM.h file. (Most
//   EEPROM manufacturers use kbits in their part numbers.)
// - nDevice is the number of EEPROM devices on the I2C bus (all must
//   be identical).
// - pageSize is the EEPROM's page size in unsigned chars.
// - eepromAddr is the EEPROM's I2C address and defaults to 0x50 which is common.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
extEEPROM::extEEPROM()
  : _currentWriteLocation(0ul),
    _currentReadLocation(0ul),
    _data()
{
    _totalCapacity = 50000;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void extEEPROM::write(unsigned long addr, unsigned char *values, unsigned int nBytes)
{
  for (auto i = 0u; i < nBytes; ++i)
  {
    _data[i+ addr] = *(values + i);
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void extEEPROM::read(unsigned long addr, unsigned char *values, unsigned int nBytes)
{
  for (auto i = 0u; i < nBytes; ++i)
  {
    *(values + i) = _data[i+ addr];
  }
}

//Write a single unsigned char to external EEPROM.
//If the I/O would extend past the top of the EEPROM address space,
//a status of EEPROM_ADDR_ERR is returned. For I2C errors, the status
//from the Arduino Wire library is passed back through to the caller.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void extEEPROM::write(unsigned long addr, unsigned char value)
{
    write(addr, &value, 1);
}

//Read a single unsigned char from external EEPROM.
//If the I/O would extend past the top of the EEPROM address space,
//a status of EEPROM_ADDR_ERR is returned. For I2C errors, the status
//from the Arduino Wire library is passed back through to the caller.
//To distinguish error values from valid data, error values are returned as negative numbers.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
unsigned char extEEPROM::read(unsigned long addr)
{
    uint8_t data;
    read(addr, &data, 1);
    return data;
}

//Write  magic packet ahead to stop the reading of garbage data on data read.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void extEEPROM::writeMagicPacket()
{
  if (_currentWriteLocation + _magicPacketLength + 1 < _totalCapacity)
  {
    for (unsigned i = 0; i < _magicPacketLength; ++i)
    {
      write(_currentWriteLocation + i, static_cast<unsigned char>('!'));
    }
  }
}

//Reads ahead and returns true if the magic packet is the next 4 unsigned chars in the
//eeprom. This is done to stop the reading of garbage data on data read.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool extEEPROM::readMagicPacket(unsigned offset)
{
  if (_currentReadLocation + offset + _magicPacketLength < _totalCapacity)
  {
    bool magicPacketFound = true;
    for (unsigned i = 0u; i < _magicPacketLength; ++i)
    {
      magicPacketFound &=
        static_cast<char>(read(_currentReadLocation + i)) == _magicCharacter;
    }
    return magicPacketFound;
  }
return false;
}
