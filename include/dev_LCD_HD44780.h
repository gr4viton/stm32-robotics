/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

#ifndef DEV_LCD_HD44780_H_INCLUDED
#define DEV_LCD_HD44780_H_INCLUDED

/*
TODO:
- create github lib
- makefile?
- create README and add TODOlist
- doxygen function comments
- doxygen docs
- nextLine function
- get '\n' and other control chars from _iowr and process them
- 4bit mode
- use timer interrupts instead of active waitin' functions
- reading busy bit
- graphical characters
- game of snake
- equalizer showin
*/


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes
#include <stdio.h>
#include <errno.h>

#include <libopencm3/stm32/gpio.h>

#include "defines.h"
#include "primitives/ringbuf.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
//____________________________________________________
//constants (do not change)

// later for interrupt timer wait contra current not active noping
#define __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE 0

//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs

typedef struct _S_lcdDevice{
    //uint32_t device;
    uint32_t cmd_port;
    uint32_t data_port;
    //uint8_t irq;
    //uint8_t af;
    uint32_t cmdp_clk;
    uint32_t datap_clk;

    uint16_t data_pins[8];
    uint16_t data_pins_all;

    uint16_t cmdRS;
    uint16_t cmdRW;
    uint16_t cmdEN;
    uint16_t cmd_pins_all;

    //lcd_device_cmd_port_t cmd;
    //uint32_t txport;
    //uint32_t rxport;
    //uint16_t txpin;
    //uint16_t rxpin;

    uint8_t nCharsPerLine;
    uint8_t nLines;

    struct ringbuf data_ring;
    //struct ringbuf rx_ring;

    uint32_t stats_rxoverruns;
    uint32_t stats_rxerrors;
} S_lcdDevice;

#if __NOT_USED_ANYMORE
// define used data_port pins
typedef struct {
    uint16_t D0;
    uint16_t D1;
    uint16_t D2;
    uint16_t D3;
    uint16_t D4;
    uint16_t D5;
    uint16_t D6;
    uint16_t D7;
} lcd_device_data_port_t;
// define used cmd_port pins
typedef struct {
    uint16_t RS;
    uint16_t RW;
    uint16_t EN;
} lcd_device_cmd_port_t;
#endif // __NOT_USED_ANYMORE

//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS
/****************
 \brief
 \param
 \retval
 ****************/
void LCD_setDataPins(uint8_t index, S_lcdDevice *dev);

/****************
 \brief
 \param
 \retval
 ****************/
uint16_t LCD_getMaskDataPins(S_lcdDevice *dev);

/****************
 \brief
 \param
 \retval
 ****************/
void LCD_displayWriteCheck(S_lcdDevice *dev);

/****************
 \brief
 \param
 \retval
 ****************/
FILE *fopenLCD(uint8_t index, uint8_t a_nCharsPerLine,
               uint8_t functionSet, uint8_t entryMode, uint8_t cursorMode,
               uint8_t *dbuf, size_t dbufsz);

/****************
 \brief
 \param
 \retval
 ****************/
void dev_LCD_checkSeek(FILE* flcd);

#if __NOT_IMPLEMENTED_YET

struct ringbuf *ser_txbuf(uint8_t index);
#endif // __NOT_IMPLEMENTED_YET

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
#include "LCD_HD44780.h"


#endif // DEV_LCD_HD44780_H_INCLUDED
