#include <Arduino.h>
#include <esp_heap_caps.h>
#include <Esp.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>

#include "DHT.h"           // dht lib

#include "server.h"
#include "tables.h"
#include "grow.h"
#include "defines.h"
#include "security.h"

#include "wifi_credentials.h"



WebServer   server  (SERVER_PORT);
DHT         dht     (DHT11_PIN, DHT11);




server_handler about_html()
{   
    server.send(200, "text/html", ABOUT_HTML_TEMPLATE);
}

server_handler graphs_html()
{
  char* to_send = (char*)heap_caps_malloc( strlen(GRAPHS_HTML_TEMPLATE) + 1024, MALLOC_CAP_8BIT );

  sprintf(to_send, GRAPHS_HTML_TEMPLATE,  temperatures_table,
                                          humidities_table);

  server.send(200, "text/html", to_send);

  heap_caps_free(to_send);
}

server_handler settings_html()
{
  char* to_send = (char*)heap_caps_malloc( strlen(SETTINGS_HTML_TEMPLATE) + 1024, MALLOC_CAP_8BIT );

  sprintf(to_send, SETTINGS_HTML_TEMPLATE, (eeprom_data.plant_light_always_on ? "checked" : ""));
  server.send(200, "text/html", to_send);

  heap_caps_free(to_send);

    /* retrieve input data */
  String s_optimal_temperature  = server.arg("optimal_temperature");
  String s_optimal_humidity     = server.arg("optimal_humidity");
  String s_time_light_on        = server.arg("time_light_on");
  String s_time_light_off       = server.arg("time_light_off");
  String s_plant_name           = server.arg("plant_name");
  String s_light_is_always_on   = server.arg("light_is_always_on");
  String s_light_is_always_off   = server.arg("light_is_always_off");
  
    /* convert input data to correct datatypes */
  if(s_optimal_temperature.isEmpty() == false)
  {
     eeprom_data.plant_temperature_optimal = s_optimal_temperature.toInt();
  }
  if(s_optimal_humidity.isEmpty() == false)
  {
     eeprom_data.plant_temperature_optimal = s_optimal_humidity.toInt();
  }
  if(s_time_light_on.isEmpty() == false)
  {
     eeprom_data.plant_hour_light_on  = s_time_light_on.toInt();
  } 
  if(s_time_light_off.isEmpty()  == false)
  {
     eeprom_data.plant_hour_light_off  = s_time_light_off.toInt();
  }
  if(s_plant_name.isEmpty() == false)
  {
    const char* buffer = s_plant_name.c_str();
    memcpy(eeprom_data.plant_name, buffer, 15);
  }   

  if(strcmp(s_light_is_always_off.c_str(), "True") == 0)
  {
     eeprom_data.plant_light_always_on  = false;
  }
  if(strcmp(s_light_is_always_on.c_str(), "True") == 0)
  {
     eeprom_data.plant_light_always_on  = true;
  }

  Serial.printf("\nlight: %s\n", s_light_is_always_on);
}

server_handler report_html()
{
  char* to_send = (char*)heap_caps_malloc( strlen(REPORTS_HTML_TEMPLATE) + 1024, MALLOC_CAP_8BIT );

  sprintf(to_send, REPORTS_HTML_TEMPLATE,   eeprom_data.plant_name,
                                            eeprom_data.plant_day,
                                            eeprom_data.plant_hour_light_on,
                                            eeprom_data.plant_hour_light_off,
                                            eeprom_data.plant_temperature_current,
                                            eeprom_data.plant_temperature_optimal,
                                            eeprom_data.plant_humidity_current,
                                            eeprom_data.plant_humidity_optimal
                                          );

  server.send(200, "text/html", to_send);

  heap_caps_free(to_send);
}

server_handler command_html()
{
  char* to_send = (char*)heap_caps_malloc( strlen(SETTINGS_HTML_TEMPLATE) + 1024, MALLOC_CAP_8BIT );

  sprintf(to_send, COMMAND_HTML_TEMPLATE, (eeprom_data.is_light_on  ? "checked" : ""),
                                          (eeprom_data.is_fan_on    ? "checked" : ""),
                                          (eeprom_data.is_heater_on ? "checked" : ""));

  server.send(200, "text/html", to_send);

  heap_caps_free(to_send);

  String s_is_light_on      = server.arg("switch_light_on");
  String s_is_light_off     = server.arg("switch_light_off");
  String s_is_fan_on        = server.arg("switch_fans_on");
  String s_is_fan_off       = server.arg("switch_fans_off");
  String s_is_heater_on     = server.arg("switch_heater_on");
  String s_is_heater_off    = server.arg("switch_heater_off");

  if(strcmp(s_is_light_off.c_str(), "True") == 0)
  {
     eeprom_data.is_light_on  = false;
  }
  if(strcmp(s_is_light_on.c_str(), "True") == 0)
  {
     eeprom_data.is_light_on  = true;
  }

  if(strcmp(s_is_fan_off.c_str(), "True") == 0)
  {
     eeprom_data.is_fan_on  = false;
  }
  if(strcmp(s_is_fan_on.c_str(), "True") == 0)
  {
     eeprom_data.is_fan_on  = true;
  }

  if(strcmp(s_is_heater_off.c_str(), "True") == 0)
  {
     eeprom_data.is_heater_on  = false;
  }
  if(strcmp(s_is_heater_on.c_str(), "True") == 0)
  {
     eeprom_data.is_heater_on  = true;
  }
}



void monitor_grow()
{
  getLocalTime(&eeprom_data.plant_time_now);

  uint8_t t = (uint8_t)dht.readTemperature();
  if(t != 255) eeprom_data.plant_temperature_current = t; // if sensor outputs bullshit

  uint8_t h = (uint8_t)dht.readHumidity();
  if(h != 255) eeprom_data.plant_humidity_current = h;    // if sensor outputs bullshit

  if(eeprom_data.plant_time_now.tm_hour == eeprom_data.plant_hour_light_on && !eeprom_data.plant_light_always_on)
  {
    eeprom_data.is_light_on = true;
  }
  if(eeprom_data.plant_time_now.tm_hour == eeprom_data.plant_hour_light_off && !eeprom_data.plant_light_always_on)
  {
    eeprom_data.is_light_on = false;
  }

  if(eeprom_data.plant_light_always_on)
  {
    eeprom_data.is_light_on = true;
  }

#ifdef BO_AUTOMATIC_CONTROLL
    /* control temperature */
  if(eeprom_data.plant_temperature_current > eeprom_data.plant_temperature_optimal) digitalWrite(HEATER_PIN, LOW);
  if(eeprom_data.plant_temperature_current < eeprom_data.plant_temperature_optimal) digitalWrite(HEATER_PIN, HIGH);

    /* control humidty */
  if(eeprom_data.plant_humidity_current < eeprom_data.plant_humidity_optimal) digitalWrite(FAN_PIN, HIGH);
  if(eeprom_data.plant_humidity_current > eeprom_data.plant_humidity_optimal) digitalWrite(FAN_PIN, LOW);
#endif

#ifdef BO_MANUAL_CONTROL
  eeprom_data.is_fan_on     ? digitalWrite(FAN_PIN, HIGH)     : digitalWrite(FAN_PIN, LOW);
  eeprom_data.is_light_on   ? digitalWrite(LEDS_PIN, HIGH)    : digitalWrite(LEDS_PIN, LOW);
  eeprom_data.is_heater_on  ? digitalWrite(HEATER_PIN, HIGH)  : digitalWrite(HEATER_PIN, LOW);
#endif

  

#ifdef BO_VERY_VERBOSE
  if(grow_info.current_time.tm_min < 10) // to display minutes like [17:03] instead of [17:3] a e s t h e t h i c a f
  {
    Serial.printf("[CORE %d - %d:0%d] growing @ %d C (%d%%)\n",  xPortGetCoreID(),
                                                              grow_info.current_time.tm_hour, 
                                                              grow_info.current_time.tm_min, 
                                                              grow_info.current_temperature, 
                                                              grow_info.current_humidity);
  }
  else
  {
    Serial.printf("[CORE %d - %d:%d] growing @ %d C (%d%%)\n",  xPortGetCoreID(),
                                                              grow_info.current_time.tm_hour, 
                                                              grow_info.current_time.tm_min, 
                                                              grow_info.current_temperature, 
                                                              grow_info.current_humidity);
  }
#endif
}



void init_pins()
{
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(LEDS_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void init_grow()
{
    /* initialize default settings */
  eeprom_data.plant_day = 0;
}

void init_time()
{
    configTime(3600, 3600, NTP_SERVER);
}

void init_eeprom()
{
    EEPROM.begin(0xff); /* allocate 256 bytes */
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
    server.on("/about.html", about_html);
    server.on("/graphs.html", graphs_html);
    server.on("/report.html", report_html);
    server.on("/settings.html", settings_html);
    server.on("/command.html", command_html);
    server.begin();

#ifdef BO_VERBOSE
    Serial.print("[+] Server running @ ");
    Serial.print(WiFi.localIP());
    Serial.printf(":%d\n", SERVER_PORT);   
#endif 
}



TaskHandle_t h_day_night_monitor;
void day_night_monitor( void* param)
{
  //Serial.printf("[.] day_night_monitor running on core: %d\n", xPortGetCoreID());

  while(1)
  {
    monitor_grow();
    delay(5000);
  }
}



void setup()
{
    /* initialize the dht11 temperature sensor */
  dht.begin();
  
    /* initalizke */
  init_pins();
  init_grow();
  init_eeprom();
  init_serial();
  init_wifi();
  init_server();
  init_time();
  init_tables();

    /* create day night monitoring thread */
  xTaskCreatePinnedToCore(  day_night_monitor,
                            "dn_mon",
                            4096,
                            NULL,
                            1,
                            &h_day_night_monitor,
                            0 );
} 

void loop()
{
  make_tables();
  server.handleClient();
} 