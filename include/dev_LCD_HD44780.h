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
#if __NOT_USED_ANYMORE
/****************
 @brief
 ****************/
typedef enum _E_lcdBaseAddresses
{
    bAdd_functionSet    = 0x20, // = 1<<5
    b_functionSet       = 5,
    ba_entryMode        = 0x04,
    ba_cursorMode       = 0x08
}E_lcdBaseAddresses;
#endif // __NOT_USED_ANYMORE


/****************
 @brief LCD function set bit predefs
 ****************/
typedef enum _E_lcdFunctionSetBits
{
    zeroBits_functionSet = 0xC0, //=0b1100 0000
    nBit_functionSet = 1<<5,     //=0b1110 0000
    nBit_font_5x10   = 1<<2, //=0x04
    nBit_two_lines   = 1<<3, //=0x08
    nBit_bus_8bit    = 1<<4  //=0x10
}E_lcdFunctionSetBits;

/****************
 @brief LCD function set parameters
  Prepared to be bit-wise logical or-ed
  (font_5x8 | two_lines | bus_4bit)
  (two_lines|font_5x7|bus_4bit)
  0x20|0x28|0x20
  base address = 0x20
 ****************/
typedef enum _E_lcdFunctionSet
{
    // 5x8 is counted with a line of pixels for cursor line
    font_5x8  = nBit_functionSet,                     //=0x20
    font_5x7  = nBit_functionSet,                     //=0x20
    font_5x10 = nBit_functionSet|nBit_font_5x10,      //=0x24
    one_line  = nBit_functionSet,                     //=0x20
    two_lines = nBit_functionSet|nBit_two_lines,      //=0x28
    bus_4bit  = nBit_functionSet,                     //=0x20
    bus_8bit  = nBit_functionSet|nBit_bus_8bit,       //=0x30
    init_functionSet = bus_8bit                       //=0x30
}E_lcdFunctionSet;

/****************
 @brief LCD entry mode set bit predefs
 ****************/
typedef enum _E_lcdEntryModeBits
{
    zeroBits_entryMode      = 0xF8, //=0b1111 1000
    nBit_entryMode          = 1<<2, //=0b0000 0100
    nBit_displayShifts      = 1<<0, //=0x01
    nBit_incrementingCursor = 1<<1  //=0x02
}E_lcdEntryModeBits;

/****************
 @brief LCD entry mode selection
 base address = 0x04
 ****************/
 typedef enum _E_lcdEntryMode
 {
    decrCursor_NoShiftDisp    = nBit_entryMode,                         //=0x04
    decrCursor_ShiftDisp      = nBit_entryMode|nBit_displayShifts,      //=0x05
    incrCursor_NoShiftDisp    = nBit_entryMode|nBit_incrementingCursor, //=0x06
    incrCursor_ShiftDisp      = nBit_entryMode|nBit_incrementingCursor|nBit_displayShifts, //=0x07

    writes2left_cursorMovesOnScreen     = nBit_entryMode,                         //=0x04
    writes2left_displayMovesWithCursor  = nBit_entryMode|nBit_displayShifts,      //=0x05
    writes2right_cursorMovesOnScreen    = nBit_entryMode|nBit_incrementingCursor, //=0x06
    writes2right_displayMovesWithCursor = nBit_entryMode|nBit_incrementingCursor|nBit_displayShifts //=0x07

 }E_lcdEntryMode;

 /****************
  @brief LCD cursor mode set bit predefs
  ****************/
 typedef enum _E_lcdCursorModeBits
 {
    zeroBits_cursorMode      = 0xF0, //=0b1111 0000
    nBit_cursorMode          = 1<<3, //=0b0000 1000
    nBit_display_on          = 1<<2, //=0x02
    nBit_cursor_on           = 1<<1, //=0x01
    nBit_cursor_blinking     = 1<<0 //=0x00
 }E_lcdCursorModeBits;

 /****************
  @brief LCD cursor mode selection
  Prepared to be bit-wise logical or-ed
  (display_on | cursor_on | cursor_notBlinking)
  base address = 0x08
  ****************/
 typedef enum _E_lcdCursorMode
 {
    display_on         = nBit_cursorMode|nBit_display_on,      //=0x0C
    display_off        = nBit_cursorMode,                      //=0x08
    cursor_on          = nBit_cursorMode|nBit_cursor_on,       //=0x0A
    cursor_off         = nBit_cursorMode,                      //=0x08
    cursor_blinking    = nBit_cursorMode|nBit_cursor_blinking, //=0x09
    cursor_notBlinking = nBit_cursorMode                       //=0x08
 }E_lcdCursorMode;

/****************
 @brief LCD function basic function addresses
 ****************/
typedef enum _E_lcdBasicFunctionAddresses
{

#define LCD_C_CUR_ADDRESS_L1        0x80
#define LCD_C_CUR_ADDRESS_L2        0xC0
#define LCD_C_CHAR_GENERATOR_ADD    0x40

// REWRITE TO ENUMS
//#define LCD_C_INIT                  0x30

#define LCD_C_DISP_BLANK        0x08
#define LCD_C_SCROLL_LEFT       0x18
#define LCD_C_SCROLL_RIGHT      0x1E
    lcd_add_line1   = 1<<7, //=0x80,
    lcd_add_line2   = 0xC0, //=1<<7|1<<6
    lcd_add_chargen = 1<<6, //=0x40 //=64

    lcd_cmd_clearAllReturn = 1<<0,
    lcd_cmd_cursorHome = 1<<1,
    lcd_cmd_displayBlank = display_off,

    nBit_shiftin = 1<<4,
    nBit_shiftDisplay = 1<<3,
    nBit_shiftRight = 1<<2,

    lcd_cmd_shiftDisplayRight = nBit_shiftin|nBit_shiftDisplay|nBit_shiftRight,
    lcd_cmd_shiftDisplayLeft = nBit_shiftin|nBit_shiftDisplay,
    lcd_cmd_moveCursorRight = nBit_shiftin|nBit_shiftRight,
    lcd_cmd_moveCursorLeft = nBit_shiftin

}E_lcdBasicFunctionAddresses;

//____________________________________________________
// structs
/****************
 @brief
 ****************/
typedef struct _S_lcdDevice{
    uint32_t cmd_port;
    uint32_t data_port;
    uint32_t cmdp_clk;
    uint32_t datap_clk;

    uint16_t data_pins[8];
    uint16_t data_pins_all;

    uint16_t cmdRS;
    uint16_t cmdRW;
    uint16_t cmdEN;
    uint16_t cmd_pins_all;


    struct ringbuf data_ring;
    //struct ringbuf rx_ring;
    //____________________________________________________
    // interface = prefix i_
    // write only inside the visible frame
    uint8_t i_writeInsideOnly;

    volatile uint8_t i_functionSet;
    volatile uint8_t i_cursorMode;
    volatile uint8_t i_entryMode;

    uint8_t actX;
    uint8_t actY;

    //____________________________________________________
    // HW&SW
    uint8_t nCharsPerLine;
    uint8_t nLines;
    uint16_t dportVal;

    //uint8_t irq;
    //uint8_t af;

    uint32_t stats_rxoverruns;
    uint32_t stats_rxerrors;
} S_dev_lcd;
//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_dev_lcd lcds_predef[];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
/****************
 \brief
 \param
 \retval
 ****************/
void LCD_setDataPins(uint8_t index, S_dev_lcd *dev);

/****************
 \brief Returns all pins in one port all_pins port
 Not used much as all_pins are counted in LCD_setDataPins
 although -> you can call this function after you change i.e one pin
 \param
 \retval
 ****************/
uint16_t LCD_getMaskDataPins(S_dev_lcd *dev);

/****************
 \brief
 \param
 \retval
 ****************/
void LCD_displayWriteCheck(S_dev_lcd *dev);

/****************
 \brief
 \param
 \retval
 ****************/
FILE *fopenLCD(uint8_t index, uint8_t indexPins, uint8_t nCharsPerLine,
               uint8_t writeInsideOnly, uint8_t functionSet,
               uint8_t entryMode, uint8_t cursorMode,
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
