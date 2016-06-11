#include <fstream>
#include <array>
#include <iostream>
#include "stdint.h"
class extEEPROM
{
    public:

        extEEPROM();

        template<class T>
        bool readData(T& Value);

        template<class T>
        void writeData(T& Value);

    private:

        void read(unsigned long addr, unsigned char *values, unsigned int nBytes);
        unsigned char read(unsigned long addr);

        void write(unsigned long addr, unsigned char *values, unsigned int nBytes);
        void write(unsigned long addr, unsigned char value);

        bool readMagicPacket(unsigned offset);

        void writeMagicPacket();

    private:

        uint8_t _eepromAddr;            //eeprom i2c address
        uint16_t _dvcCapacity;          //capacity of one EEPROM device, in kbits
        uint8_t _nDevice;               //number of devices on the bus
        uint16_t _pageSize;             //page size in unsigned chars
        uint8_t _csShift;               //number of bits to shift address for chip select bits in control unsigned char
        uint16_t _nAddrBytes;           //number of address unsigned chars (1 or 2)
        unsigned long _totalCapacity;   //capacity of all EEPROM devices on the bus, in unsigned chars
        unsigned long _currentWriteLocation;
        unsigned long _currentReadLocation;
        static const char _magicCharacter;
        static const unsigned _magicPacketLength;
        std::array<unsigned char, 50000> _data;

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <class T>
void extEEPROM::writeData(T& Value)
{
  if (_currentWriteLocation + sizeof(T) < _totalCapacity)
  {
    write(
      _currentWriteLocation,
      reinterpret_cast<unsigned char*> (&Value),
      sizeof(T));

    _currentWriteLocation += sizeof(T);

    writeMagicPacket();
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <class T>
bool extEEPROM::readData(T& Value)
{
  if (
    (_currentReadLocation + sizeof(T) < _totalCapacity) &&
    (!readMagicPacket(sizeof(T))))
  {
    read(
      _currentReadLocation,
      reinterpret_cast<unsigned char*> (&Value),
      sizeof(T));

    _currentReadLocation += sizeof(T);

    return true;
  }
  return false;
}
