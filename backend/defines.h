#ifndef H_DEFINES_H
#define H_DEFINES_H

        /* eeprom settings */
    #define EEPROM_TEMPERATURE_SETTING_OFFSET   0x00                        // memory location where persistent temperature setting is stored
    #define EEPROM_HUMIDITY_SETTING_OFFSET      0x01                        // memory location where persistent humidity setting is stored
    #define EEPROM_ALLOC_SIZE                   0x80                        // amount of allocated persistent memory

        /* server settings */
    #define SERVER_PORT                         80                          // server port

        /* time settings */
    #define NTP_SERVER                           "0.be.pool.ntp.org"        // ntp server used for time

        /* build options (comment to disable) */
    #define BO_VERBOSE                                                      // outputs stuff to the serial monitor

#endif