//******************************************************************************
// IRremote
// Version 2.0.1 June, 2015
// Copyright 2009 Ken Shirriff
// For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
// Edited by Mitra to add new controller SANYO
//
// Interrupt code based on NECIRrcv by Joe Knapp
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
// Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
//
// JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
// LG added by Darryl Smith (based on the JVC protocol)
// Whynter A/C ARC-110WD added by Francesco Meschia
// Mitsubishi HVAC protocol added by Vincent Cruvellier
//
// Modified by Sufi-Al-Hussaini (AKA Shoaib Ahmed) to remove Arduino related dependencies
//
//******************************************************************************

#ifndef IRGEN_H
#define IRGEN_H

class IRGen {
public:
    IRGen() {}

    void mark(unsigned int usec);

    void space(unsigned int usec);

    //......................................................................
#	if GEN_RC5
    void  genRC5        		(unsigned long data,  int nbits) ;
#	endif
#	if GEN_RC6
    void  genRC6        		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_NEC
    void  genNEC        		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_SONY
    void  genSony       		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_PANASONIC
    void  genPanasonic  		(unsigned int address,  unsigned long data) ;
#	endif
    //......................................................................
#	if GEN_JVC
    // JVC does NOT repeat by gening a separate code (like NEC does).
    // The JVC protocol repeats by skipping the header.
    // To gen a JVC repeat signal, gen the original code value
    //   and set 'repeat' to true
    void  genJVC       			(unsigned long data,  int nbits,  bool repeat) ;
#	endif
    //......................................................................
#	if GEN_SAMSUNG
    void  genSAMSUNG    		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_WHYNTER
    void  genWhynter    		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_AIWA_RC_T501
    void  genAiwaRCT501 		(int code) ;
#	endif
    //......................................................................
#	if GEN_LG
    void genLG(unsigned long data, int nbits);
#	endif
    //......................................................................
#	if GEN_SANYO
    void  genSanyo      		( ) ; // NOT WRITTEN
#	endif
    //......................................................................
#	if GEN_MISUBISHI
    void  genMitsubishi 		( ) ; // NOT WRITTEN
#	endif
    //......................................................................
#	if GEN_DISH
    void  genDISH       		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_SHARP
    void  genSharpRaw   		(unsigned long data,  int nbits) ;
    void  genSharp      		(unsigned int address,  unsigned int command) ;
#	endif
    //......................................................................
#	if GEN_DENON
    void  genDenon      		(unsigned long data,  int nbits) ;
#	endif
    //......................................................................
#	if GEN_PRONTO
    void  genPronto     		(char* code,  bool repeat,  bool fallback) ;
#	endif
    //......................................................................
#	if GEN_LEGO_PF
    void  genLegoPowerFunctions	(uint16_t data, bool repeat = true) ;
#	endif
    //......................................................................
#	if GEN_HVAC_PANASONIC
    void genHvacPanasonic(uint8_t data[19]);
#	endif
};

#endif    /* IRGEN_H */
