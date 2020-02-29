/*
    functions to read humidity & temperature
*/

#ifndef H_SENSORS_H
#define H_SENSORS_H

#include <esp_heap_caps.h>

/*
    reads temperature inside the box
*/
uint8_t get_temperature();

/*
    reads humidity inside the box
*/
uint8_t get_humidity();

#endif