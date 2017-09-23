//  BWCLS.h
//  Command Line Switch
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//
// Modified by Sufi-Al-Hussaini (AKA Shoaib Ahmed) to use std::string instead of BWString
//

#ifndef __BWLib__BWCLS__
#define __BWLib__BWCLS__

#include <map>
#include <vector>
#include <string>

#define __BWCLS_VERSION     "1.0.7.1"
#define __BWCLS_MAXSTRLEN    1024

class BWCLS {
    const int _argc;
    const char **_argv;
    std::map<std::string, std::string> _switches;
    std::vector<std::string> _values;

    BWCLS();    // no default constructor
    void process_args();

    void add_switch(const char *arg);

    void add_arg(const char *arg);

public:
    typedef std::map<std::string, std::string> switch_map;
    typedef std::vector<std::string> value_vector;

    BWCLS(const unsigned int argc, const char **argv);

    static const char *version() { return __BWCLS_VERSION; }

    switch_map &get_switches() { return _switches; }

    value_vector &get_values() { return _values; }

    std::string switch_value(const std::string &key) const;

    std::string operator[](const std::string &key) const;
};

#endif /* defined(__BWLib__BWCLS__) */
