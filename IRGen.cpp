#include <cstdio>
#include <cinttypes>
#include "IRGen.h"

void IRGen::mark(unsigned int time) {
    printf("+%u ", time);
}

void IRGen::space(unsigned int time) {
    printf("-%u ", time);
}


//+=============================================================================
#if GEN_LG
#define LG_BITS         28
#define LG_HDR_MARK     8350
#define LG_HDR_SPACE    4250
#define LG_BIT_MARK     550
#define LG_ONE_SPACE    1500
#define LG_ZERO_SPACE   500
#define LG_RPT_LENGTH   60000


void IRGen::genLG(unsigned long data, int nbits) {
    // Header
    mark(LG_HDR_MARK);
    space(LG_HDR_SPACE);
    mark(LG_BIT_MARK);

    // Data
    for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
        if (data & mask) {
            space(LG_ONE_SPACE);
            mark(LG_BIT_MARK);
        } else {
            space(LG_ZERO_SPACE);
            mark(LG_BIT_MARK);
        }
    }
    space(0);  // Always end with the LED off
}

#endif /* GEN_LG */

//+=============================================================================
#if GEN_HVAC_PANASONIC
// PANASONIC HVAC
#define HVAC_PANASONIC_HDR_MARK     3500
#define HVAC_PANASONIC_HDR_SPACE    1750
#define HVAC_PANASONIC_BIT_MARK     435
#define HVAC_PANASONIC_ONE_SPACE    1300
#define HVAC_PANASONIC_ZERO_SPACE   435
#define HVAC_PANASONIC_RPT_SPACE    10000
#define HVAC_PANASONIC_RPT_MARK     435

void IRGen::genHvacPanasonic(uint8_t data[19]) {
    uint8_t mask = 1;
    uint8_t dataconst[8] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06};

    // data array is a valid frame, only byte to be changed will be updated.
    uint8_t i;

    // Byte 18 - CRC
    data[18] = 0;
    for (i = 0; i < 18; i++) {
        data[18] = (uint8_t) data[i] + data[18];  // CRC is a simple bits addition
    }

    space(0);

    // Generate constant frame #1
    mark(HVAC_PANASONIC_HDR_MARK);
    space(HVAC_PANASONIC_HDR_SPACE);
    for (i = 0; i < 8; i++) {
        // Generate all Bits from Byte dataconst in Reverse Order
        for (mask = 00000001; mask > 0; mask <<= 1) { // iterate through bit mask
            if (dataconst[i] & mask) { // Bit ONE
                mark(HVAC_PANASONIC_BIT_MARK);
                space(HVAC_PANASONIC_ONE_SPACE);
            } else { // Bit ZERO
                mark(HVAC_PANASONIC_BIT_MARK);
                space(HVAC_PANASONIC_ZERO_SPACE);
            }
            //Next bits
        }
    }
    mark(HVAC_PANASONIC_RPT_MARK);
    space(HVAC_PANASONIC_RPT_SPACE);

    // Generate frame #2
    mark(HVAC_PANASONIC_HDR_MARK);
    space(HVAC_PANASONIC_HDR_SPACE);
    for (i = 0; i < 19; i++) {
        // Generate all Bits from Byte Data in Reverse Order
        for (mask = 00000001; mask > 0; mask <<= 1) { // iterate through bit mask
            if (data[i] & mask) { // Bit ONE
                mark(HVAC_PANASONIC_BIT_MARK);
                space(HVAC_PANASONIC_ONE_SPACE);
            } else { // Bit ZERO
                mark(HVAC_PANASONIC_BIT_MARK);
                space(HVAC_PANASONIC_ZERO_SPACE);
            }
            // Next bits
        }
    }
    // End of Packet and retransmission of the Packet
    mark(HVAC_PANASONIC_RPT_MARK);
    space(HVAC_PANASONIC_RPT_SPACE);
    space(0);
}

#endif  /* GEN_HVAC_PANASONIC */