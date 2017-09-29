//
// Created by Shoaib Ahmed on 9/12/17.
//
#include <cstdio>
#include <cstdlib>
#include "BWCLS.h"
#include "IRGen.h"


static uint8_t data[19] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x48, 0x3C, 0x80, 0xAF, 0x00, 0x00, 0x0E, 0xE0, 0x10, 0x00,
                           0x01, 0x00, 0x06, 0xBE};
// data array is a valid frame, only byte to be changed will be updated.

typedef enum HvacMode {
    HVAC_HOT,
    HVAC_COLD,
    HVAC_DRY,
    HVAC_FAN, // used for Panasonic only
    HVAC_AUTO
} HvacMode_t; // HVAC  MODE

typedef enum HvacFanMode {
    FAN_SPEED_NONE,
    FAN_SPEED_1,
    FAN_SPEED_2,
    FAN_SPEED_3,
    FAN_SPEED_4,
    FAN_SPEED_5,
    FAN_SPEED_AUTO,
    FAN_SPEED_SILENT
} HvacFanMode_;  // HVAC  FAN MODE

typedef enum HvacVanneMode {
    VANNE_AUTO,
    VANNE_H1,
    VANNE_H2,
    VANNE_H3,
    VANNE_H4,
    VANNE_H5,
    VANNE_AUTO_MOVE
} HvacVanneMode_;  // HVAC  VANNE MODE

typedef enum HvacWideVanneMode {
    WIDE_LEFT_END,
    WIDE_LEFT,
    WIDE_MIDDLE,
    WIDE_RIGHT,
    WIDE_RIGHT_END
} HvacWideVanneMode_t;  // HVAC  WIDE VANNE MODE

typedef enum HvacAreaMode {
    AREA_SWING,
    AREA_LEFT,
    AREA_AUTO,
    AREA_RIGHT
} HvacAreaMode_t;  // HVAC  WIDE VANNE MODE

typedef enum HvacProfileMode {
    NORMAL,
    QUIET,
    BOOST
} HvacProfileMode_t;  // HVAC PANASONIC OPTION MODE


void usage(const char *pgm);

void exit_error(const char *pgm, const char *err);

void setData(
        HvacMode HVAC_Mode,                 // Example HVAC_HOT  HvacPanasonicMode
        int HVAC_Temp,                      // Example 21  (°c)
        HvacFanMode HVAC_FanMode,           // Example FAN_SPEED_AUTO  HvacPanasonicFanMode
        HvacVanneMode HVAC_VanneMode,       // Example VANNE_AUTO_MOVE  HvacPanasonicVanneMode
        HvacProfileMode HVAC_ProfileMode,   // Example QUIET HvacPanasonicProfileMode
        int HVAC_SWITCH                     // Example false
);


int main(int argc, const char *argv[]) {
    static const char *sw_mode = "mode";
    static const char *sw_temp = "temp";
    static const char *sw_fan = "fan";
    static const char *sw_vanne = "vanne";
    static const char *sw_profile = "profile";
    static const char *sw_switch = "switch";

    BWCLS cls(argc, argv);
    const char *pgm_name = cls.get_values()[0].c_str();
    bool has_f_help = !cls["help"].empty();
    bool has_f_mode = !cls[sw_mode].empty();
    bool has_f_temp = !cls[sw_temp].empty();
    bool has_f_fan = !cls[sw_fan].empty();
    bool has_f_vanne = !cls[sw_vanne].empty();
    bool has_f_profile = !cls[sw_profile].empty();
    bool has_f_switch = !cls[sw_switch].empty();

    if (has_f_help || !has_f_mode || !has_f_temp || !has_f_fan
        || !has_f_vanne || !has_f_profile || !has_f_switch)
        exit_error(pgm_name, NULL);

    int md = std::stoi(cls[sw_mode].c_str());
    int tmp = std::stoi(cls[sw_temp].c_str());
    int fn = std::stoi(cls[sw_fan].c_str());
    int vn = std::stoi(cls[sw_vanne].c_str());
    int pf = std::stoi(cls[sw_profile].c_str());
    bool sw = std::stoi(cls[sw_switch].c_str()) != 0;

    if ((md < HVAC_HOT || md > HVAC_AUTO)
        || (tmp < 16 || tmp > 30)
        || (fn < FAN_SPEED_1 || fn > FAN_SPEED_SILENT)
        || (vn < 0 || vn > 4)
        || (pf < NORMAL || pf > BOOST))
        exit_error(pgm_name, NULL);


    IRGen irgen;
    setData((HvacMode_t) md, tmp, (HvacFanMode_) (fn), (HvacVanneMode_) vn, (HvacProfileMode_t) pf, sw);
    irgen.genHvacPanasonic(data);
    printf("\n");
    exit(EXIT_SUCCESS);
}

void usage(const char *pgm) {
    fprintf(stderr, "Usage: \n%s [switch]\n", pgm);
    fprintf(stderr, "%s", "--help\n"
            "--mode=<0 : hot, 1 : cold, 2: dry, 3: fan, 4: auto>\n"
            "--temp=<16 ~ 30 : temperature (°c)>\n"
            "--fan=<1 ~ 5 : Speed, 6: Auto, 7: Silent>\n"
            "--vanne=<0 : Wide left end, 1 : Wide left, 2: Wide middle, \n"
            "          3 : Wide right, 4: Wide right end>\n"
            "--profile=<0 : normal, 1 : quiet, 2: boost>\n"
            "--switch=<0 : off, 1 : on>\n");
}

void exit_error(const char *pgm, const char *err) {
    if (err) fprintf(stderr, "%s", err);
    usage(pgm);
    exit(EXIT_FAILURE);
}

void setData(
        HvacMode HVAC_Mode,                 // Example HVAC_HOT  HvacPanasonicMode
        int HVAC_Temp,                      // Example 21  (°c)
        HvacFanMode HVAC_FanMode,           // Example FAN_SPEED_AUTO  HvacPanasonicFanMode
        HvacVanneMode HVAC_VanneMode,       // Example VANNE_AUTO_MOVE  HvacPanasonicVanneMode
        HvacProfileMode HVAC_ProfileMode,   // Example QUIET HvacPanasonicProfileMode
        int HVAC_SWITCH                     // Example false
) {
    // Byte 6 - On / Off
    if (HVAC_SWITCH) {
        data[5] = (uint8_t) 0x08; // Switch HVAC Power
    } else {
        data[5] = (uint8_t) 0x09; // Do not switch HVAC Power
    }

    // Byte 6 - Mode
    switch (HVAC_Mode) {
        case HVAC_HOT:
            data[5] = (uint8_t) (data[5] | 0x40);
            break;
        case HVAC_FAN:
            data[5] = (uint8_t) (data[5] | 0x60);
            break;
        case HVAC_COLD:
            data[5] = (uint8_t) (data[5] | 0x18);
            break;
        case HVAC_DRY:
            data[5] = (uint8_t) (data[5] | 0x20);
            break;
        case HVAC_AUTO:
            data[5] = (uint8_t) (data[5] | 0x00);
            break;
        default:
            break;
    }

    // Byte 7 - Temperature
    // Check Min Max For Hot Mode
    uint8_t Temp;
    if (HVAC_Temp > 30) { Temp = 30; }
    else if (HVAC_Temp < 16) { Temp = 16; }
    else { Temp = HVAC_Temp; };
    data[6] = (uint8_t) (Temp - 16) << 1;
    data[6] = (uint8_t) (data[6] | 0x20);
    // bits used form the temp are [4:1]
    // data|6] = data[6] << 1;


    // Byte 9 - FAN / VANNE
    switch (HVAC_FanMode) {
        case FAN_SPEED_1:
            data[8] = (uint8_t) 0x30;
            break;
        case FAN_SPEED_2:
            data[8] = (uint8_t) 0x40;
            break;
        case FAN_SPEED_3:
            data[8] = (uint8_t) 0x50;
            break;
        case FAN_SPEED_4:
            data[8] = (uint8_t) 0x60;
            break;
        case FAN_SPEED_5:
            data[8] = (uint8_t) 0x50;
            break;
        case FAN_SPEED_AUTO:
            data[8] = (uint8_t) 0xA0;
            break;
        default:
            break;
    }

    switch (HVAC_VanneMode) {
        case VANNE_AUTO:
            data[8] = (uint8_t) (data[8] | 0x0F);
            break;
        case VANNE_AUTO_MOVE:
            data[8] = (uint8_t) (data[8] | 0x0F);
            break; // same as AUTO in the PANASONIC CASE
        case VANNE_H1:
            data[8] = (uint8_t) (data[8] | 0x01);
            break;
        case VANNE_H2:
            data[8] = (uint8_t) (data[8] | 0x02);
            break;
        case VANNE_H3:
            data[8] = (uint8_t) (data[8] | 0x03);
            break;
        case VANNE_H4:
            data[8] = (uint8_t) (data[8] | 0x04);
            break;
        case VANNE_H5:
            data[8] = (uint8_t) (data[8] | 0x05);
            break;
        default:
            break;
    }

    // Byte 14 - Profile
    switch (HVAC_ProfileMode) {
        case NORMAL:
            data[13] = (uint8_t) 0x10;
            break;
        case QUIET:
            data[13] = (uint8_t) 0x60;
            break;
        case BOOST:
            data[13] = (uint8_t) 0x11;
            break;
        default:
            break;
    }
}
