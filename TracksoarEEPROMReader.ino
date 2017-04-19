#include "extEEPROM.h"
#include "Wire.h"

extEEPROM g_EEPROM(kbits_2048, 1, 256);

bool g_write_data = false;
const char g_magic_character = '!';

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void readData()
{
  if (g_write_data)
  {
    char gps_time[7];       // HHMMSS
    g_EEPROM.readData(gps_time);

    uint32_t gps_seconds;   // seconds after midnight
    g_EEPROM.readData(gps_seconds);

    float gps_lat;
    g_EEPROM.readData(gps_lat);

    float gps_lon;
    g_EEPROM.readData(gps_lon);

    float gps_altitude;
    g_EEPROM.readData(gps_altitude);

    float temperature;
    g_EEPROM.readData(temperature);

    int32_t pressure;
    g_EEPROM.readData(pressure);

    float humidity;
    if (g_EEPROM.readData(humidity))
    {
      Serial.print(gps_time);
      Serial.print(',');
      Serial.print(gps_seconds);
      Serial.print(',');
      Serial.print(gps_lat);
      Serial.print(',');
      Serial.print(gps_lon);
      Serial.print(',');
      Serial.print(gps_altitude);
      Serial.print(',');
      Serial.print(temperature);
      Serial.print(',');
      Serial.print(pressure);
      Serial.print(',');
      Serial.print(humidity);
      Serial.println("");
    }
    else
    {
      Serial.println("EOF");
      g_write_data = false;
    }
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void loop()
{
  while (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input == g_magic_character)
    {
      g_EEPROM.read_from_begining();
      g_write_data = true;
    }
  }
  readData();
  delay(50);
}
