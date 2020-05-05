#include <Arduino.h>
#include <time.h>

/* todo: implement method to read/write this struct on eeprom */
struct EEPROM_DATA
{
  byte      xor_key                     [32];  // 32
  byte      wifi_ssid_encrypted         [32];  // 32
  byte      wifi_pass_encrypted         [32];  // 32

  char      plant_name                  [16];  // 16
  uint8_t   plant_temperature_optimal;         // 1
  uint8_t   plant_humidity_optimal;            // 1

  uint8_t   plant_temperature_current;
  uint8_t   plant_humidity_current;

  uint8_t plant_hour_light_on;
  uint8_t plant_hour_light_off;

  bool      plant_is_awake;

  uint16_t  plant_day;                         // 2

  bool      is_heater_on;   
  bool      is_light_on;   
  bool      is_fan_on;   
  
  bool      plant_light_always_on;

  struct tm plant_time_now;                    // 36
};

extern struct EEPROM_DATA eeprom_data;