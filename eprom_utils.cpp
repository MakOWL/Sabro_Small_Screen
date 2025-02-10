#include "eprom_utils.h"
#include <EEPROM.h>
#include "com_structs.h"

 void eeprom_init()
  {
    EEPROM.begin (EEPROM_END_ADDRESS - EEPROM_START_ADDRESS);
    uint8_t eeprom_start = EEPROM.read (EEPROM_START_ADDRESS);

    if (eeprom_start == 0xff)
      {
        EEPROM.write (EEPROM_START_ADDRESS, 0);
        is_paired = false;
        EEPROM.write (EEPROM_ADDRESS_ESPNOW_IS_PAIRED, (uint8_t) is_paired);
        EEPROM.commit();
        return;
      }

    is_paired = (bool) EEPROM.read (EEPROM_ADDRESS_ESPNOW_IS_PAIRED);
    if (is_paired)
      EEPROM.readBytes(EEPROM_ADDRESS_ESPNOW_MAC_ADDRESS, paired_mac, MAC_ADDRESS_ARRAY_SIZE);
  }

  void eeprom_add_mac_address()
  {
    if (!is_paired) return;
    EEPROM.write (EEPROM_ADDRESS_ESPNOW_IS_PAIRED, (uint8_t) is_paired);
    EEPROM.writeBytes (EEPROM_ADDRESS_ESPNOW_MAC_ADDRESS, paired_mac, MAC_ADDRESS_ARRAY_SIZE);
    EEPROM.commit();
  }

    void eeprom_clear_mac_address()
  {
    if (is_paired) return;
    EEPROM.write (EEPROM_ADDRESS_ESPNOW_IS_PAIRED, (uint8_t) is_paired);
    EEPROM.commit();
  }
  void eeprom_reset()
{
    for (int i = EEPROM_START_ADDRESS; i < EEPROM_END_ADDRESS; i++)
    {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
}
