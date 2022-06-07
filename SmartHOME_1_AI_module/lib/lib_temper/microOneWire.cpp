#include "microOneWire.h"
#ifdef __AVR__
#define MOW_CLI() uint8_t oldSreg = SREG; cli();
#define MOW_SEI() SREG = oldSreg
#else
#define MOW_CLI()
#define MOW_SEI()
#endif

bool oneWire_reset(uint8_t pin) { 
    pinMode(pin, OUTPUT);
    delayMicroseconds(600);
    pinMode(pin, INPUT);
    MOW_CLI();
    delayMicroseconds(60);
    bool pulse = digitalRead(pin);
    MOW_SEI();
    delayMicroseconds(600);
    return !pulse;
}

void oneWire_write(uint8_t data, uint8_t pin) {
    for (uint8_t i = 8; i; i--) {
        pinMode(pin, OUTPUT);
        MOW_CLI();
        if (data & 1) {
            delayMicroseconds(5);
            pinMode(pin, INPUT);
            delayMicroseconds(60);
        } else {
            delayMicroseconds(60);
            pinMode(pin, INPUT);
            delayMicroseconds(5);
        }
        MOW_SEI();
        data >>= 1;
    }
}

uint8_t oneWire_read(uint8_t pin) {
    uint8_t data = 0;
    for (uint8_t i = 8; i; i--) {
        data >>= 1;
        MOW_CLI();
        pinMode(pin, OUTPUT);
        delayMicroseconds(2);
        pinMode(pin, INPUT);
        delayMicroseconds(8);
        if (digitalRead(pin)) data |= (1 << 7);
        delayMicroseconds(60);
        MOW_SEI();
    }
    return data;
}

uint8_t read_bit(void){
	IO_REG_TYPE mask IO_REG_MASK_ATTR = bitmask;
	volatile IO_REG_TYPE *reg IO_REG_BASE_ATTR = baseReg;
	uint8_t r;

	noInterrupts();
	DIRECT_MODE_OUTPUT(reg, mask);
	DIRECT_WRITE_LOW(reg, mask);
	delayMicroseconds(3);
	DIRECT_MODE_INPUT(reg, mask);	// let pin float, pull up will raise
	delayMicroseconds(10);
	r = DIRECT_READ(reg, mask);
	interrupts();
	delayMicroseconds(53);
	return r;
}

bool oneWire_search(uint8_t *newAddr, bool search_mode /* = true */, uint8_t pin){
   uint8_t id_bit_number;
   uint8_t last_zero, rom_byte_number;
   bool    search_result;
   uint8_t id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = false;

   // if the last call was not the last one
   if (!LastDeviceFlag) {
      // 1-Wire reset
      if (!oneWire_reset()) {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = false;
         LastFamilyDiscrepancy = 0;
         return false;
      }

      // issue the search command
      if (search_mode == true) {
        oneWire_write(0xF0, pin);   // NORMAL SEARCH
      } else {
        oneWire_write(0xEC, pin);   // CONDITIONAL SEARCH
      }

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = read_bit();
         cmp_id_bit = read_bit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1)) {
            break;
         } else {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit) {
               search_direction = id_bit;  // bit write value for search
            } else {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy) {
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               } else {
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);
               }
               // if 0 was picked then record its position in LastZero
               if (search_direction == 0) {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            write_bit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0) {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65)) {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0) {
            LastDeviceFlag = true;
         }
         search_result = true;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0]) {
      LastDiscrepancy = 0;
      LastDeviceFlag = false;
      LastFamilyDiscrepancy = 0;
      search_result = false;
   } else {
      for (int i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
   }
   return search_result;
}

