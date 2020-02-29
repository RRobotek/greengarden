/*
    functions to parse data & times into table usable by Google charts
*/

#ifndef H_TABLES_H
#define H_TABLES_H

#include <time.h>
#include <esp_heap_caps.h>





/* specific defines */


#define TABLE_WIDTH         8
#define TABLE_ALLOC_SIZE    400

#define TABLE_TEMPLATE      "  ['%d',  %d],\n"\
                            "  ['%d',  %d],\n"\
                            "  ['%d',  %d],\n"\
                            "  ['%d',  %d],\n"\
                            "  ['%d',  %d],\n"\
                            "  ['%d',  %d],\n"\
                            "  ['%d',  %d],\n"\
                            "  ['%d',  %d] \n"



/* global variables */


extern struct tm timeinfo;

extern char* temperatures_table;
extern char* humidities_table;

extern uint8_t temperatures_array  [TABLE_WIDTH];
extern uint8_t humidities_array    [TABLE_WIDTH];
extern uint8_t times_array         [TABLE_WIDTH];





/* function declarations */


/*
  initializes tables by allocating memory and first time count (in minutes !!!!!)
*/
void init_tables();


/*
  uninits tables by freeing the allocated memory
*/
void uninit_tables();


/*
  parses table into readable by google charts form
  (https://www.wikihow.com/Make-a-Table)
*/
void make_tables();



#endif