#include "security.h"

#include <EEPROM.h>
#include <esp_types.h>

char xor_key[32];

bool get_xor_key()
{
        /* read probably already existing key */
    for (uint8_t i = 0; i < 32; i++) xor_key[i] = EEPROM.read(i);
    
        /* if key is bs, generate & save a new one */
    if(xor_key[0] != 0xfe)
    {
            /* generate key */
        xor_key[0] = 0xfe;
        for (uint8_t i = 1; i < 32; i++)xor_key[i] = esp_random();

            /* save the key */
        for (uint8_t i = 0; i < 32; i++) EEPROM.write(i, xor_key[i]);

        return 1;
    }

    return 0;
}

void xor_encrypt(char* in, char* key, char* out, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++) out[i] = in[i] ^ key[i];
}