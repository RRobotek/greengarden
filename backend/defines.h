#ifndef H_DEFINES_H
#define H_DEFINES_H

        /* hardware settings */
    #define LEDS_PIN                            23
    #define DHT11_PIN                           21
    #define HEATER_PIN                          19
    #define FAN_PIN                             5
        /* server settings */
    #define SERVER_PORT                         80                          // server port

        /* time settings */
    #define NTP_SERVER                          "0.be.pool.ntp.org"        // ntp server used for time

        /* build options (comment to disable) */
    #define BO_VERBOSE                                                      // outputs stuff to the serial monitor
    #define BO_MANUAL_CONTROL
#endif