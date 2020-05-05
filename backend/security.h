#ifndef H_SECURITY_H
#define H_SECURITY_H

#include <esp_types.h>

extern char xor_key[32];

/**
 * @brief reads existing key / generates new key
 * 
 * @return true     a new key was generated
 * @return false    an existing key was found in EEPROM
 */
bool get_xor_key();

/**
 * @brief encrypts with xor
 * 
 * @param in        data to encrypt
 * @param key       encryption key
 * @param out       encrypted data
 * @param length    length of data/key/out in bytes
 */
void xor_encrypt(char* in, char* key, char* out, uint8_t length);

#endif