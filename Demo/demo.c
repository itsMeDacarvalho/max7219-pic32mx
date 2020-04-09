/******************************************************************
* FILENAME :    
*       main.c
*
* DESCRIPTION :
*       This is a example program to test max7219 library. If it
*       runs with no errors it must display a happy face on 8x8
*       led matrix.
*       
*       CLK - Pin 52
*       CS  - Pin 22
*       DIN - Pin 53
*
* AUTHOR :    Daniel Carvalho        START DATE :    06 Apr 2020
*
* VERSION   DATE
* -------   09Apr20
*******************************************************************/

#include "config_bits.h"
#include "max7219.h"
#include "spi.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>

#define UART_BAUDRATE 115200
#define SPI_CLK 7000000

void initConfig(){
    TRISCbits.TRISC2 = 0;
    
    spiInit();
    printf("[SPI] - Done.\n\r");
    
    spiSetClk(SPI_CLK);
    printf("[SPI CLK] - %d\n\r", SPI_CLK);
}

void delay(){
    int i;
    
    for(i=0; i<500000; i++){
        asm("nop");
    }
}

int main(int argc, char* argv[]){
    int i;
    initConfig();
    
    maxTurnOn();
    maxInit(DECODE_LEDMATRIX, SCAN_ALL);
    maxSetIntensity(INTENSITY_MEDIUM);
    maxClearPanel();
    
    while(1){
        maxPrintLetter(')');
    }

    return 0;
}