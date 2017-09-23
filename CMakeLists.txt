cmake_minimum_required(VERSION 3.7)
project(IRGen)

set(LG_AC_SOURCE_FILES
        BWCLS.cpp
        BWCLS.h
        IR-lg-ac-demo.cpp
        IRGen.cpp
        IRGen.h)

add_executable(IR-lg-ac-demo ${LG_AC_SOURCE_FILES})
target_compile_definitions(IR-lg-ac-demo PRIVATE GEN_LG)

set(PANASONIC_AC_SOURCE_FILES
        BWCLS.cpp
        BWCLS.h
        IR-panasonic-ac-demo.cpp
        IRGen.cpp
        IRGen.h)

add_executable(IR-panasonic-ac-demo ${LG_AC_SOURCE_FILES})
target_compile_definitions(IR-panasonic-ac-demo PRIVATE GEN_LG)
