/*
    freegarden firmware by Fedor
*/

#include <Arduino.h>
#include <esp_heap_caps.h>
#include <Esp.h>

#include <EEPROM.h>
#include <WiFi.h>
#include <WebServer.h>

#include "server.h"
#include "tables.h"
#include "sensors.h"

#include "defines.h"

#include "wifi_credentials.h"


WebServer server(SERVER_PORT);



  /* index.html variables */
  /* TODO: put in a struct! */
uint16_t plant_day = 0;
const char* plant_name = "tomato";
const char* plant_state = "asleep";
int optimal_temperature;  /* optimal temperature setting */
int optimal_humidity;     /* optimal humidity setting */


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// server handlers chunk
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    when user visits index.html 
*/
server_handler index_html()
{
    /* load EEPROM values */
  optimal_humidity    = EEPROM.readByte(EEPROM_HUMIDITY_SETTING_OFFSET);
  optimal_temperature = EEPROM.readByte(EEPROM_TEMPERATURE_SETTING_OFFSET);

    /* parse index.html */
  char* source_index_html = (char*)heap_caps_malloc(strlen(INDEX_HTML_TEMPLATE) + 1024, MALLOC_CAP_8BIT);
  sprintf(source_index_html, INDEX_HTML_TEMPLATE, 
                                                    temperatures_table, 
                                                    humidities_table, 
                                                    plant_day, 
                                                    plant_name, 
                                                    plant_state, 
                                                    get_temperature(), 
                                                    optimal_temperature, 
                                                    get_humidity(), 
                                                    optimal_humidity);

    /* display index.html */
  server.send(200, "text/html", source_index_html);
  
    /* free index.html memory block */
  heap_caps_free(source_index_html);

    /* retrieve input data */
  String s_optimal_temperature  = server.arg("optimal_temperature");
  String s_optimal_humidity     = server.arg("optimal_humidity");
  
    /* convert input data to correct datatypes */
  if(s_optimal_temperature.isEmpty() == false) optimal_temperature  = s_optimal_temperature.toInt();
  if(s_optimal_humidity.isEmpty()    == false) optimal_humidity     = s_optimal_humidity.toInt();


    /* display input data in the console */
  Serial.printf(  "\n------\n[+] New temperature setting: %d\n[+] New humidity setting: %d\n------\n",
                  optimal_temperature,
                  optimal_humidity
                );

    /* save the data into EEPROM */
  EEPROM.put        (EEPROM_TEMPERATURE_SETTING_OFFSET,   optimal_temperature);
  EEPROM.put        (EEPROM_HUMIDITY_SETTING_OFFSET,      optimal_humidity);
  EEPROM.commit();


}

/*
    when user visits about.html
*/
server_handler about_html()
{   
    // TODO!
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inits
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void init_time()
{
    configTime(3600, 3600, NTP_SERVER);
}

void init_eeprom()
{
    EEPROM.begin(EEPROM_ALLOC_SIZE); /* allocate 256 bytes */
}

void init_wifi()
{
    WiFi.begin( U_WIFI_SSID,
                U_WIFI_PASS);

    while(WiFi.status() != WL_CONNECTED) delay(2000);
#ifdef BO_VERBOSE
    Serial.printf("[+] Connected to %s\n", U_WIFI_SSID);
#endif
}

void init_serial()
{
    Serial.begin(9600);

#ifdef BO_VERBOSE
        /* greeting */
    Serial.printf("\n\n--[ freegarden v0.1 ]--\n\n[+] Available heap: %d bytes\n", esp_get_free_heap_size());
#endif
}

void init_server()
{
        /* setting up server handlers */
    server.on("/index.html", index_html);
    
    server.begin();

#ifdef BO_VERBOSE
    Serial.print("[+] Server running @ ");
    Serial.print(WiFi.localIP());
    Serial.printf(":%d\n", SERVER_PORT);   
#endif 
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setup & loop
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    init_serial();
    init_wifi();
    init_server();
    init_eeprom();
    init_time();
    init_tables();
}

void loop()
{
    while(true)
    {
        make_tables();
        server.handleClient();
    }

    uninit_tables();
}