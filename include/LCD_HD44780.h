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

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% from HD44780 datasheet
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
R/S	R/W	D7	D6	D5	D4	D3	D2	D1	D0	Instruction/Description
________________________________________________________________________________________
0	0	0	0	0	0	0	0	0	1	Clear Display and Home the Cursor
0	0	0	0	0	0	0	0	1	*	Return Cursor and LCD to Home Position
0	0	0	0	0	0	0	1	ID	S	Set Cursor Move Direction
0	0	0	0	0	0	1	D	C	B	Enable Display/Cursor
0	0	0	0	0	1	SC	RL	*	*	Move Cursor/Shift Display
0	0	0	0	1	DL	N	F	*	*	Set Interface Length
0	0	0	1	A	A	A	A	A	A	Move Cursor into CGRAM
0	0	1	A	A	A	A	A	A	A	Move Cursor to Display
0	1	BF	*	*	*	*	*	*	*	Poll the "Busy Flag"
1	0	D	D	D	D	D	D	D	D	Write a Character to the Display at the Current Cursor Position
1	1	D	D	D	D	D	D	D	D	Read the Character on the Display at the Current Cursor Position

"*" - Not Used/Ignored. This bit can be either "1" or "0"
Set Cursor Move Direction:
 ID - Increment the Cursor After Each Byte Written to Display if Set
 S - Shift Display when Byte Written to Display

Enable Display/Cursor
 D - Turn Display On(1)/Off(0)
 C - Turn Cursor On(1)/Off(0)
 B - Cursor Blink On(1)/Off(0)

Move Cursor/Shift Display
 SC - Display Shift On(1)/Off(0)
 RL - Direction of Shift Right(1)/Left(0)

Set Interface Length
 DL - Set Data Interface Length 8(1)/4(0)
 N - Number of Display Lines 1(0)/2(1)
 F - Character Font 5x10(1)/5x7(0)

Poll the "Busy Flag"
 BF - This bit is set while the LCD is processing

Move Cursor to CGRAM/Display
 A - Address

Read/Write ASCII to the Display
 D - Data
 */

#ifndef LCD_HD44780_H_INCLUDED
#define LCD_HD44780_H_INCLUDED

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes

//#include <stdio.h>
#include <libopencm3/stm32/gpio.h>

#include "primitives/ringbuf.h"
#include "waitin.h"

#include "dev_LCD_HD44780.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)

// HD44780 physical parameters
//#define LCD_8DATA_BIT
#define LCD_4DATA_BIT
#define HD44780_P_XMAX      16
#define HD44780_P_YMAX      2
//____________________________________________________
//constants (do not change)

// CONTROL COMMANDS
#define LCD_C_CLR0                  0x01
#define LCD_C_POS0                  0x02

#if __NOT_USED_ANYMORE
#define LCD_C_ENTRY_LEFT_CMOVE      0x04
#define LCD_C_ENTRY_RIGHT_CSTAY     0x05
#define LCD_C_ENTRY_RIGHT_CMOVE     0x06
#define LCD_C_ENTRY_LEFT_CSTAY      0x07

#define LCD_C_CUR_MOV_LEFT          0x10
#define LCD_C_CUR_MOV_RIGHT         0x14
#define LCD_C_CUR_VIS_NO            0x0C
#define LCD_C_CUR_VIS_STATIC        0x0E
#define LCD_C_CUR_VIS_BLINK         0x0F

#define LCD_C_8BIT_1L_5x7_LIGHT     0x30
#define LCD_C_8BIT_1L_5x10_LIGHT    0x34
#define LCD_C_8BIT_1L_5x7           0x38
#define LCD_C_8BIT_1L_5x10          0x3C

#define LCD_C_8BIT_2L_5x7_LIGHT     0x3A
#define LCD_C_8BIT_2L_DARK          0x38

#define LCD_C_4BIT_2L_5x7_LIGHT     0x32



#define LCD_C_CUR_ADDRESS_L1        0x80
#define LCD_C_CUR_ADDRESS_L2        0xC0
#define LCD_C_CHAR_GENERATOR_ADD    0x40

// REWRITE TO ENUMS
//#define LCD_C_INIT                  0x30

#define LCD_C_DISP_BLANK        0x08
#define LCD_C_SCROLL_LEFT       0x18
#define LCD_C_SCROLL_RIGHT      0x1E
#endif // __NOT_USED_ANYMORE

//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
typedef enum _E_waitType{
    sendCmd,   // after enable clear when sending command
    sendData,  // after enable clear when sending data
    setEnable, // after enable set
    setRS,     // after change of RS
    dispCheck  // in function LCD_displayWriteCheck
    }E_waitType;
//____________________________________________________
// structs
//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS

/****************
 \brief  Busy waiting for interval defined by type of operation
  Times are set according to datasheet
 \param
 \retval
 ****************/
void LCD_waitBusy(E_waitType wType);
    //____________________________________________________
    // conrol
/****************
 \brief  Clears display and saved graphical characters, moves cursor to 0,0
 \param
 ****************/
void LCD_clear(S_dev_lcd *dev);
/****************
 \brief   Returns cursor to start position 0,0 and changes nothing else
 \param
 ****************/
void LCD_home(S_dev_lcd *dev);

/****************
 \brief  Set cursor to next line
 \param
 ****************/
void LCD_nextLine(S_dev_lcd *dev);

/****************
 \brief  Writes blank spaces on [lineNum]-th line of LCD
 \param
 ****************/
void LCD_clearLine(S_dev_lcd *dev, uint8_t lineNum);

/****************
 \brief Set cursor to position x,y of the display register
 \param
 ****************/
uint8_t LCD_gotoxy(S_dev_lcd *dev, uint8_t x, uint8_t y);

    //____________________________________________________
    // write

#if __NOT_IMPLEMENTED_YET
void LCD_printString(char *s);
#endif // __NOT_IMPLEMENTED_YET

/****************
 \brief   Prints char to the display - additional preprocessing
 on newline character, tab, writeInsideOnly - handling
 \param
 ****************/
void LCD_printChar(S_dev_lcd *dev, char ch);

/****************
 \brief  Write byte with data/control bit set = wType
 \param
 \retval
 ****************/
void LCD_write(S_dev_lcd *dev, uint8_t data, E_waitType wType);

/****************
 \brief  Write char with data bit set
 \param
 \retval
 ****************/
void LCD_writeChar(S_dev_lcd *dev, uint8_t ch);

/****************
 \brief  Write byte with control bit set
 \param
 \retval
 ****************/
void LCD_writeCmd(S_dev_lcd *dev, uint8_t cmd_data);

/****************
 \brief  Exposes port value and clocks with ENable control pin
 \param
 \retval
 ****************/
void LCD_writePort(S_dev_lcd *dev, uint8_t data, E_waitType wType);

/****************
 \brief  Exposes individual pin values on lcd output port
 \param
 \retval
 ****************/
void LCD_exposePortValue(S_dev_lcd *dev, uint8_t data);

/****************
 \brief
 \param
 \retval
 ****************/
void LCD_writeByte(S_dev_lcd *dev, uint8_t data, E_waitType wType);

/****************
 \brief Refresh i_functionSet, i_entryMode & cursorMode from actual command
 \param
 ****************/
void LCD_saveSettings(S_dev_lcd* dev, uint8_t data);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES


#endif // LCD_HD44780_H_INCLUDED
