#include "tables.h"

#include "defines.h"
#include "grow.h"

#include <esp_heap_caps.h>
#include <WiFi.h>

struct tm timeinfo;

char* temperatures_table;
char* humidities_table;

uint8_t temperatures_array  [TABLE_WIDTH];
uint8_t humidities_array    [TABLE_WIDTH];
uint8_t times_array         [TABLE_WIDTH];

/**
 * @brief initializes tables by allocating memory and first time count (in hours !!!!!)
 * 
 */
void init_tables()
{
    temperatures_table  = (char*)heap_caps_malloc(TABLE_ALLOC_SIZE, MALLOC_CAP_8BIT);
    humidities_table    = (char*)heap_caps_malloc(TABLE_ALLOC_SIZE, MALLOC_CAP_8BIT);

    times_array[TABLE_WIDTH-1] = eeprom_data.plant_time_now.tm_hour;
}

/**
 * @brief uninits tables by freeing the allocated memory
 * 
 */
void uninit_tables()
{
    heap_caps_free(temperatures_table);
    heap_caps_free(humidities_table);
}


/**
 * @brief parses table into readable by google charts form
 *        (https://www.wikihow.com/Make-a-Table)
 * 
 */
void make_tables()
{
  getLocalTime(&eeprom_data.plant_time_now);
  
  if((eeprom_data.plant_time_now.tm_hour > times_array[TABLE_WIDTH-1]) || ((times_array[TABLE_WIDTH-1] == 23) && (eeprom_data.plant_time_now.tm_hour == 0)))
  {
    for(uint8_t i=0; i < TABLE_WIDTH-1; i++)
    {
      times_array[i] = times_array[i+1];
      temperatures_array[i] = temperatures_array[i+1];
      humidities_array[i] = humidities_array[i+1];
    }

    times_array[TABLE_WIDTH-1]          = eeprom_data.plant_time_now.tm_hour;
    temperatures_array[TABLE_WIDTH-1]   = eeprom_data.plant_temperature_current;
    humidities_array[TABLE_WIDTH-1]     = eeprom_data.plant_humidity_current;
  }

  sprintf(temperatures_table, TABLE_TEMPLATE, times_array[0], temperatures_array[0],
                                              times_array[1], temperatures_array[1],
                                              times_array[2], temperatures_array[2],
                                              times_array[3], temperatures_array[3],
                                              times_array[4], temperatures_array[4],
                                              times_array[5], temperatures_array[5],
                                              times_array[6], temperatures_array[6],
                                              times_array[7], temperatures_array[7]);

  sprintf(humidities_table, TABLE_TEMPLATE,     times_array[0], humidities_array[0],
                                                times_array[1], humidities_array[1],  
                                                times_array[2], humidities_array[2],
                                                times_array[3], humidities_array[3],
                                                times_array[4], humidities_array[4],
                                                times_array[5], humidities_array[5],
                                                times_array[6], humidities_array[6],
                                                times_array[7], humidities_array[7]);
}