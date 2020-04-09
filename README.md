# max7219-pic32mx
MAX7219 library for use with PIC32MX microcontroller

**Supported and Tested Microcontrollers**
Model | Supported | Tested
------|-----------|--------
PIC32MX795F512L | ✅ | ✅

> **Note:** If you successfully tested this library in other microcontroller please report to me in order to update the list of supported microcontrollers.

## Setup
- Clone the repository to your project folder .
- Make sure you include max7219.h and max7219.c in your program file.

## Usage
- In order to use this library, you need to configure SPI2 module in your device, with maximum baudrate of 10M Hz. You can find how to do it here: http://ww1.microchip.com/downloads/en/devicedoc/61106g.pdf . 
- After SPI module is properly configured, you are ready to use the library functions.
> **Note:** Additional information about the functions of this library can be found in the comments of the files present in the repository.

## Relevant Functions
```c
void maxTurnOn()
// Turn ON/OFF the module.
```

```c
void maxInit(uint8_t outputDevice, uint8_t activeRows)
// Set the type of device will be controlled by MAX7219 and how many rows you want to work.
// Use provided macros in max7219.h file.
```

```c
void maxClearPanel()
// Clear all leds on panel.
````

```c
void maxPrintNumber(int number)
// Display a number with 2 digit format.
```

```c
void maxPrintLetter(char letter)
// Display a letter centered on panel.
```

> **Important:** Make sure you always use `<maxTurnOn()>` and `<maxInit()>` before everything in your code.
