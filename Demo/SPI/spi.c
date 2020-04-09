#include "spi.h"

void spiInit(){
    char fifoReset;
    
    SPI2CONbits.ON = 0;         // Disable SPI2
    
    SPI2CONbits.CKP = 1;        // IDLE state as 0
    SPI2CONbits.CKE = 0;        // Clock active transition from 1 to 0
    SPI2CONbits.SMP = 0;
    SPI2CONbits.MODE16 = 0;     // Use 16 bit SPI interface
    SPI2CONbits.MODE32 = 0;     // Use 32 bit SPI interface
    SPI2CONbits.ENHBUF = 1;
    SPI2CONbits.MSSEN = 1;
    SPI2CONbits.MSTEN = 1;      // Enable master mode
    
    while(SPI2STATbits.SPIRBE == 0){
        fifoReset = SPI2BUF;
    }
    
    SPI2STATbits.SPIROV = 0;    // Clear overflow error flag
    SPI2CONbits.ON = 1;         // Enable SPI2 module
    
}

void spiSetClk(int clkFreq){
    SPI2BRG = (PBCLK + clkFreq)/(2 * clkFreq) - 1;
}