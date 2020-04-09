/******************************************************************
* FILENAME :    
*       max7219.h
*
* DESCRIPTION :
*       This header file supports the max7219.c file!
*
* AUTHOR :    Daniel Carvalho        START DATE :    06 Apr 2020
*
*
* VERSION   DATE
* 1.0       09Apr20
*******************************************************************/

#include <xc.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/attribs.h>

// State macros - ON or OFF the panel
#define STATE_ADDR          0x0C
#define STATE_NORMAL        0x01
#define STATE_SHUTDOWN      0x00

// Intensity macros - Bright of the panel
#define INTENSITY_ADDR      0x0A
#define INTENSITY_LOW       0x00
#define INTENSITY_MEDIUM    0x07
#define INTENSITY_HIGH      0x0F

// Decode type macros - Led matrix or 7 segment displays
#define DECODE_ADDR         0x09
#define DECODE_LEDMATRIX    0x00
#define DECODE_7SEG         0xFF

// Scan macros - Which rows are ON
#define SCAN_ADDR           0x0B
#define SCAN_ALL            0x07
#define SCAN_HALF           0x03

// Functions prototypes
void maxWrite(uint8_t address, uint8_t data);
void maxPrintDigit(int side, int number);
void maxPrintNumber(int number);
void maxPrintLetter(char letter);
void grabDigitPattern(int side, int digit);
void grabLetterPattern(char letter);
void maxTurnOff();
void maxTurnOn();
void maxClearPanel();
void maxInit(uint8_t outputDevice, uint8_t activeRows);