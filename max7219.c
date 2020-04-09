/******************************************************************
* FILENAME :    
*       max7219.c
*
* DESCRIPTION :
*       This library provides some functions that allow you to 
*       control in a easy way the MAX7219 module.
* 
* OBS :
*       Make sure you have SPI2 module of your PIC32 successfully
*       activated when you use this library.
*
* AUTHOR :    Daniel Carvalho        START DATE :    06 Apr 2020
*
* VERSION   DATE
* 1.0       09Apr20
*******************************************************************/
#include "max7219.h"

// Numbers footprint
uint8_t zeroL[4]    =   {0x3E, 0x22, 0x3E, 0x00};
uint8_t zeroR[4]    =   {0x00, 0x3E, 0x22, 0x3E};
uint8_t oneL[4]     =   {0x08, 0x04, 0x3E, 0x00};
uint8_t oneR[4]     =   {0x00, 0x08, 0x04, 0x3E};
uint8_t twoL[4]     =   {0x3A, 0x2A, 0x2E, 0x00};
uint8_t twoR[4]     =   {0x00, 0x3A, 0x2A, 0x2E};
uint8_t threeL[4]   =   {0x2A, 0x2A, 0x3E, 0x00};
uint8_t threeR[4]   =   {0x00, 0x2A, 0x2A, 0x3E};
uint8_t fourL[4]    =   {0x0E, 0x08, 0x3E, 0x00};
uint8_t fourR[4]    =   {0x00, 0x0E, 0x08, 0x3E};
uint8_t fiveL[4]    =   {0x2E, 0x2A, 0x3A, 0x00};
uint8_t fiveR[4]    =   {0x00, 0x2E, 0x2A, 0x3A};
uint8_t sixL[4]     =   {0x3E, 0x2A, 0x3A, 0x00};
uint8_t sixR[4]     =   {0x00, 0x3E, 0x2A, 0x3A};
uint8_t sevenL[4]   =   {0x02, 0x3A, 0x06, 0x00};
uint8_t sevenR[4]   =   {0x00, 0x02, 0x3A, 0x06};
uint8_t eightL[4]   =   {0x3E, 0x2A, 0x3E, 0x00};
uint8_t eightR[4]   =   {0x00, 0x3E, 0x2A, 0x3E};
uint8_t nineL[4]    =   {0x2E, 0x2A, 0x3E, 0x00};
uint8_t nineR[4]    =   {0x00, 0x2E, 0x2A, 0x3E};

// Letters footprint
uint8_t letterA[8]  =   {0x00, 0xFF, 0xFF, 0x33, 0x33, 0xFF, 0xFF, 0x00};
uint8_t letterE[8]  =   {0x00, 0xFF, 0xFF, 0xDB, 0xDB, 0xC3, 0xC3, 0x00};

// Emojis footprint
uint8_t emojiHappy[8]   =   {0x3C, 0x42, 0x95, 0xA1, 0xA1, 0x95, 0x42, 0x3C};
uint8_t emojiSad[8]     =   {0x3C, 0x42, 0xA5, 0x91, 0x91, 0xA5, 0x42, 0x3C};
// Signs footprint
uint8_t signStop[8] =   {0x3C, 0x7E, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x3C};

// Global variables to pick patterns
uint8_t numberPattern[4];
uint8_t letterPattern[8];

/*
 * Function:    maxWrite(uint8_t address, uint8_t data)
 * 
 * Params:      address - Register to acesss
 *              data    - Data to write into register  
 * 
 * Overview:    Write data into a desired register
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxWrite(uint8_t address, uint8_t data){
    char cleanBuffer;
    
    cleanBuffer = SPI2BUF;
    
    PORTCbits.RC2 = 0;
    
    SPI2BUF = address;
    SPI2BUF = data;
    while(SPI2STATbits.SPIBUSY);
    
    PORTCbits.RC2 = 1;
}

/*
 * Function:    maxPrintDigit(int side, int digit)
 * 
 * Params:      side    - Half left: 0 | Half right: 1
 *              digit   - Digit to display [0...9]  
 * 
 * Overview:    Display a digit on panel.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxPrintDigit(int side, int digit){
    int start, end, i;
    
    grabDigitPattern(side, digit);
    
    if(side == 1){
        start = 4;
        end = 8;
    }
    else{
        start = 0;
        end = 4;
    }
    
    for(i=start; i<end; i++){
        PORTCbits.RC2 = 0;
        SPI2BUF = 0x0 << 4 | i+1;
        SPI2BUF = numberPattern[i-start];
        while(SPI2STATbits.SPIBUSY);
        PORTCbits.RC2 = 1;
    }
    
}

/*
 * Function:    maxPrintNumber(int number)
 * 
 * Params:      number  - Number to display 
 * 
 * Overview:    Display a 2 digit number on panel.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxPrintNumber(int number){
    int leftDigit, rightDigit;
    
    if(number < 10){
        rightDigit = number%10;
        maxPrintDigit(1, rightDigit);
    
        leftDigit = number/10;
        maxPrintDigit(0, 0);
    }
    else{
        rightDigit = number%10;
        maxPrintDigit(1, rightDigit);
    
        leftDigit = number/10;
        maxPrintDigit(0, leftDigit);
    }

    
}

/*
 * Function:    maxPrintLetter(char letter)
 * 
 * Params:      letter  - Letter to display
 * 
 * Overview:    Display a letter centered on panel.
 * 
 * Obs.:        Please check full list with letters, emojis and signs supported.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxPrintLetter(char letter){
    int i;
    
    grabLetterPattern(letter);
    
    for(i=0; i<8; i++){
        PORTCbits.RC2 = 0;
        SPI2BUF = 0x0 << 4 | i+1;
        SPI2BUF = letterPattern[i];
        while(SPI2STATbits.SPIBUSY);
        PORTCbits.RC2 = 1;
    }
}


/*
 * Function:    grabDigitPattern(int side, int digit)
 * 
 * Params:      side    - Side of digit to display
 *              digit   - Digit to display 
 * 
 * Overview:    This function have a internal role, it grabs a desired pattern
 *              and write to numberPattern global variable. This way some
 *              functions can use the pattern easily.
 * 
 * Obs.:        This is a internal function, please dont use it!
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void grabDigitPattern(int side, int digit){
    int i;
    
    switch(digit){
        case 0:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = zeroR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = zeroL[i];
                }
            }
            break;
            
        case 1:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = oneR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = oneL[i];
                }
            }
            break;
            
        case 2:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = twoR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = twoL[i];
                }
            }
            break;
            
        case 3:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = threeR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = threeL[i];
                }
            }
            break;
            
        case 4:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = fourR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = fourL[i];
                }
            }
            break;
        
        case 5:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = fiveR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = fiveL[i];
                }
            }
            break;
        
        case 6:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = sixR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = sixL[i];
                }
            }
            break;
            
        case 7:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = sevenR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = sevenL[i];
                }
            }
            break;
            
        case 8:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = eightR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = eightL[i];
                }
            }
            break;
            
        case 9:
            if(side == 1){
                for(i=0; i<4; i++){
                    numberPattern[i] = nineR[i];
                }
            }
            else{
                for(i=0; i<4; i++){
                    numberPattern[i] = nineL[i];
                }
            }
            break;

        
        default:
            break;

    }
}

/*
 * Function:    grabLetterPattern(char letter)
 * 
 * Params:      letter  - Letter to display.
 * 
 * Overview:    This function have a internal role, it grabs a desired pattern
 *              and write to letterPattern global variable. This way some
 *              functions can use the pattern easily.
 * 
 * Obs.:        This is a internal function, please dont use it!
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void grabLetterPattern(char letter){
    int i;
    
    letter = tolower(letter);
    
    switch(letter){
        case 'e':
            for(i=0; i<8; i++){
                letterPattern[i] = letterE[i];
            }
            break;
            
        case 'a':
            for(i=0; i<8; i++){
                letterPattern[i] = letterA[i];
            }
            break;
        
        case ')':
            for(i=0; i<8; i++){
                letterPattern[i] = emojiHappy[i];
            }
            break;
        
        case '(':
            for(i=0; i<8; i++){
                letterPattern[i] = emojiSad[i];
            }
            break;
            
            
        default:
            break;     
    }
}

/*
 * Function:    maxClearPanel()
 * 
 * Params:      
 * 
 * Overview:    Use this function to turn off all leds in panel.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxClearPanel(){
    int i;
    char cleanBuffer;
    
    cleanBuffer = SPI2BUF;
    
    for(i=0; i<8; i++){
        PORTCbits.RC2 = 0;
        SPI2BUF = 0x0 << 4 | i+1;
        SPI2BUF = 0X00;
        while(SPI2STATbits.SPIBUSY);
        PORTCbits.RC2 = 1;
    }
}

/*
 * Function:    maxSetIntensity(uint8_t intensityLevel)
 * 
 * Params:      intensityLevel  - Value from 0 to 15 or make use of macros at
 *                              header file.    
 * 
 * Overview:    Use this function to adjust the panel brightness.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxSetIntensity(uint8_t intensityLevel){
    uint8_t intensity;
    
    intensity = 0x0 << 4 | intensityLevel;
    
    maxWrite(INTENSITY_ADDR, intensity);
}

/*
 * Function:    maxTurnOff()
 * 
 * Params:   
 * 
 * Overview:    Use this function to put module at OFF state.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxTurnOff(){
    char cleanBuffer;
    
    cleanBuffer = SPI2BUF;
    
    PORTCbits.RC2 = 0;
    
    SPI2BUF = STATE_ADDR;
    SPI2BUF = STATE_SHUTDOWN;
    while(SPI2STATbits.SPIBUSY);
    
    PORTCbits.RC2 = 1;
}

/*
 * Function:    maxTurnOn()
 * 
 * Params:   
 * 
 * Overview:    Use this function to put module at ON state.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxTurnOn(){
    char cleanBuffer;
    
    cleanBuffer = SPI2BUF;
    
    PORTCbits.RC2 = 0;
    
    SPI2BUF = STATE_ADDR;
    SPI2BUF = STATE_NORMAL;
    while(SPI2STATbits.SPIBUSY);
    
    PORTCbits.RC2 = 1;
}

/*
 * Function:    maxInit(uint8_t outputDevice, uint8_t activeRows)
 * 
 * Params:      outputDevice    - Type of device you want to control. You must
 *                              use only the macros provided at header file.
 *              activeRows      - Value from 0 to 7, that controls the number of
 *                              active rows on panel.
 * 
 * Overview:    Simple init function to set up the module.
 * ------------------------------------------------------
 * Tested:      YES
 * ------------------------------------------------------
 */
void maxInit(uint8_t outputDevice, uint8_t activeRows){
    maxWrite(DECODE_ADDR, outputDevice);
    maxWrite(SCAN_ADDR, activeRows);
}