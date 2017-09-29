#include <cstdio>
#include <cstdlib>
#include "BWCLS.h"
#include "IRGen.h"

int AC_TYPE = 1;
int AC_HEAT = 0;

const int AC_FLOW_TOWER[3] = {0, 4, 6};
const int AC_FLOW_WALL[4] = {0, 2, 4, 5};

unsigned long AC_CODE_TO_SEND;

void ac_send_code(unsigned long code) {
    IRGen irgen;
    irgen.genLG(code, 28);
}

void ac_activate(int temperature, int air_flow) {
    int AC_MSBITS1 = 8;
    int AC_MSBITS2 = 8;
    int AC_MSBITS3 = 0;
    int AC_MSBITS4;
    if (AC_HEAT == 1) {
        // heating
        AC_MSBITS4 = 4;
    } else {
        // cooling
        AC_MSBITS4 = 0;
    }
    int AC_MSBITS5 = temperature - 15;
    int AC_MSBITS6;

    if (AC_TYPE == 0) {
        AC_MSBITS6 = AC_FLOW_TOWER[air_flow];
    } else {
        AC_MSBITS6 = AC_FLOW_WALL[air_flow];
    }

    int AC_MSBITS7 = (AC_MSBITS3 + AC_MSBITS4 + AC_MSBITS5 + AC_MSBITS6) & 0x0F;

    AC_CODE_TO_SEND = AC_MSBITS1 << 4;
    AC_CODE_TO_SEND = (AC_CODE_TO_SEND + AC_MSBITS2) << 4;
    AC_CODE_TO_SEND = (AC_CODE_TO_SEND + AC_MSBITS3) << 4;
    AC_CODE_TO_SEND = (AC_CODE_TO_SEND + AC_MSBITS4) << 4;
    AC_CODE_TO_SEND = (AC_CODE_TO_SEND + AC_MSBITS5) << 4;
    AC_CODE_TO_SEND = (AC_CODE_TO_SEND + AC_MSBITS6) << 4;
    AC_CODE_TO_SEND = (AC_CODE_TO_SEND + AC_MSBITS7);

    ac_send_code(AC_CODE_TO_SEND);
}

void ac_change_air_swing(int air_swing) {
    if (AC_TYPE == 0) {
        if (air_swing == 1) {
            AC_CODE_TO_SEND = 0x881316B;
        } else {
            AC_CODE_TO_SEND = 0x881317C;
        }
    } else {
        if (air_swing == 1) {
            AC_CODE_TO_SEND = 0x8813149;
        } else {
            AC_CODE_TO_SEND = 0x881315A;
        }
    }

    ac_send_code(AC_CODE_TO_SEND);
}

void ac_power_down() {
    AC_CODE_TO_SEND = 0x88C0051;
    ac_send_code(AC_CODE_TO_SEND);
}

void ac_air_clean(int air_clean) {
    if (air_clean == 1) {
        AC_CODE_TO_SEND = 0x88C000C;
    } else {
        AC_CODE_TO_SEND = 0x88C0084;
    }

    ac_send_code(AC_CODE_TO_SEND);
}

void usage(const char *pgm) {
    fprintf(stderr, "Usage: \n%s [switch]\n", pgm);
    fprintf(stderr, "%s", "--help\n"
            "--op=<0 ~ 2>\n"
            "      0 : off\n"
            "      1 : temp/flow --temp=<18 ~ 30> --flow=<0 ~ 2>\n"
            "      2 : air_swing --swing=<0 or 1>\n"
            "--type=<0 or 1> (optional)\n"
            "      0 : tower\n"
            "      1 : wall (default)\n"
            "--heat=<0 or 1> (optional)\n"
            "      0 : cool (default)\n"
            "      1 : heat\n"
            "      3 : air_clean --clean=<0 or 1>\n");
}

void exit_error(const char *pgm, const char *err) {
    if (err) fprintf(stderr, "Error: %s\n", err);
    usage(pgm);
    exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
    static const char *sw_type = "type";
    static const char *sw_heat = "heat";
    static const char *sw_op = "op";
    static const char *sw_temp = "temp";
    static const char *sw_flow = "flow";
    static const char *sw_swing = "swing";
    static const char *sw_clean = "clean";
//    static const char *sw_power = "power";

    BWCLS cls(argc, argv);
    const char *pgm_name = cls.get_values()[0].c_str();
    bool has_f_help = !cls["help"].empty();
    bool has_f_type = !cls[sw_type].empty();
    bool has_f_heat = !cls[sw_heat].empty();
    bool has_f_op = !cls[sw_op].empty();
    bool has_f_temp = !cls[sw_temp].empty();
    bool has_f_flow = !cls[sw_flow].empty();
    bool has_f_swing = !cls[sw_swing].empty();
    bool has_f_clean = !cls[sw_clean].empty();

    if (has_f_help) exit_error(pgm_name, nullptr);
    else if (!has_f_op) exit_error(pgm_name, "Missing op!");

    int op = std::stoi(cls[sw_op].c_str());

    if (has_f_type) {
        int t = std::stoi(cls[sw_type].c_str());
        if ((t != 0) && (t != 1)) exit_error(pgm_name, "Invalid type");

        AC_TYPE = t;
    }

    if (has_f_heat) {
        int h = std::stoi(cls[sw_heat].c_str());
        if ((h != 0) && (h != 1)) exit_error(pgm_name, "Invalid heat");

        AC_HEAT = h;
    }

    switch (op) {
        case 0:
            ac_power_down();
            break;
        case 1: {
            if (!has_f_temp || !has_f_flow) exit_error(pgm_name, "Missing temp/flow!");
            int t = std::stoi(cls[sw_temp].c_str());
            int f = std::stoi(cls[sw_flow].c_str());
            if (!(18 <= t && t <= 30) || !(0 <= f && f <= 2)) exit_error(pgm_name, "Invalid temp/flow!");
            ac_activate(t, f);
            break;
        }
        case 2: {
            if (!has_f_swing) exit_error(pgm_name, "Missing swing!");
            int s = std::stoi(cls[sw_swing].c_str());
            if (!(s == 0 || s == 1)) exit_error(pgm_name, "Invalid swing!");
            ac_change_air_swing(s);
            break;
        }
        case 3: {
            if (!has_f_clean) exit_error(pgm_name, "Missing clean!");
            int c = std::stoi(cls[sw_clean].c_str());
            if (!(c == 0 || c == 1)) exit_error(pgm_name, "Invalid clean!");
            ac_air_clean(c);
            break;
        }
        default:
            exit_error(pgm_name, "Unknown op!");
            break;
    }

    printf("\n");
    exit(EXIT_SUCCESS);
}

