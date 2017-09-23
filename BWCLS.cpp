//  BWCLS
//  Simple Command Line Switch Processor
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWCLS.h"
#include <sstream>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


BWCLS::BWCLS(const unsigned int argc, const char **argv) : _argc(argc), _argv(argv) {
    process_args();
}

void BWCLS::process_args() {
    const char *arg = nullptr;
    for (int i = 0; i < _argc; ++i) {
        arg = _argv[i];
        if (*arg == '-') {
            add_switch(arg);
        } else {
            add_arg(arg);
        }
    }

}

void BWCLS::add_switch(const char *c_arg) {
    static const char equal = '=';
    static const char dash = '-';

    // make sure the string isn't out of range
    if (strnlen(c_arg, __BWCLS_MAXSTRLEN) >= __BWCLS_MAXSTRLEN) return;

    // skip leading dashes
    while (*c_arg == dash) {
        if (*++c_arg == 0) return;
    }

    std::string switch_str(c_arg);
    if (switch_str.find(equal) != std::string::npos) {
        std::vector<std::string> parts = split(switch_str, equal);
        _switches.insert(std::make_pair(parts[0], parts[1]));
    } else {
        _switches.insert(std::make_pair(switch_str, switch_str));
    }
}

void BWCLS::add_arg(const char *c_arg) {
    std::string arg_str(c_arg);
    _values.push_back(arg_str);
}

std::string BWCLS::switch_value(const std::string &key) const {
    std::string v;
    if (_switches.find(key) != _switches.end()) v = _switches.at(key);
    return v;
}

std::string BWCLS::operator[](const std::string &key) const {
    return switch_value(key);
}
