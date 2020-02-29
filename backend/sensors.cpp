#include "sensors.h"

#include <Arduino.h>

  /* prototype function until sensor is acquired */
uint8_t get_temperature()
{
  return random(15, 40);
}

  /* prototype function until sensor is acquired */
uint8_t get_humidity()
{
  return random(50, 100);
}