#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

enum eeprom_addresses
  {
    EEPROM_START_ADDRESS = 0,
    EEPROM_ADDRESS_ESPNOW_IS_PAIRED = 1,
    EEPROM_ADDRESS_ESPNOW_MAC_ADDRESS,
    EEPROM_END_ADDRESS = 8};

void eeprom_init ();
void eeprom_add_mac_address();
void eeprom_clear_mac_address();
void eeprom_reset();



#endif