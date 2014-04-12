/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.c
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "LCD_HD44780.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DEFINITIONS
//____________________________________________________
// static variables
//____________________________________________________
// other variables
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________

void LCD_waitBusy(E_waitType wType){
    //uint16_t times=0;
    //uint8_t i =0;
    switch(wType){
        case(sendCmd): mswait(5); break;  // Wait 5ms for command writes
        case(sendData): mswait(1); break;     // and 200us for data writes.
        case(setRS):
        case(setEnable): //mswait(1); break;  // Wait At Least 450 ns!!!
            //__asm__("nop");__asm__("nop");__asm__("nop");
            mswait(1);
            break;
        case(dispCheck): mswait(500); break;  // long for refreshment - LCD_displayWriteCheck
        default: mswait(5);
    }

}
void LCD_nextLine(S_dev_lcd *dev)
{
    if(dev->actY>=dev->nLines-1) dev->actY=0;
    else dev->actY++;
    LCD_gotoxy(dev,0,dev->actY);
}

void LCD_clear(S_dev_lcd *dev)
{
    dev->actX = 0;
    dev->actY = 0;
    LCD_writeCmd(dev, lcd_cmd_clearAllReturn);
}

void LCD_home(S_dev_lcd *dev)
{
    dev->actX = 0;
    dev->actY = 0;
    LCD_writeCmd(dev, lcd_cmd_cursorHome);
}

// indexed from zero
uint8_t LCD_gotoxy(S_dev_lcd *dev, uint8_t x, uint8_t y)
{
    if(x>HD44780_P_XMAX-1) return('x');
    dev->actX = x;
    dev->actY = y;
    switch(y){
        case(0):
            LCD_writeCmd(dev, LCD_C_CUR_ADDRESS_L1+x); break;
        case(1):
            LCD_writeCmd(dev, LCD_C_CUR_ADDRESS_L2+x); break;
        default:
            return('y');
    }
    return(0);
}

void LCD_writeChar(S_dev_lcd *dev,uint8_t ch)
{
    LCD_write(dev,ch,sendData);
}

void LCD_writeCmd(S_dev_lcd *dev,uint8_t cmd_data)
{
    LCD_write(dev, cmd_data, sendCmd);

}

void LCD_write(S_dev_lcd *dev, uint8_t data, E_waitType wType)
{
    gpio_clear(dev->cmd_port, dev->cmdRW); // cmd: writing
    if(wType == sendCmd){
        gpio_clear(dev->cmd_port, dev->cmdRS); // cmd: sending cmd
        LCD_waitBusy(setRS);
        LCD_writeByte(dev,data, sendCmd);
    }
    else {
        gpio_set(dev->cmd_port, dev->cmdRS); // cmd: sending data
        LCD_waitBusy(setRS);
        LCD_writeByte(dev, data, sendData);
        dev->actX++;
    }
}

#if __NOT_USED_ANYMORE
void LCD_exposePortValue(uint8_t data){
    uint16_t LCD_port = 0x0000;
    if( data & GPIO4) LCD_port |= LCD_D4;
    if( data & GPIO5) LCD_port |= LCD_D5;
    if( data & GPIO6) LCD_port |= LCD_D6;
    if( data & GPIO7) LCD_port |= LCD_D7;

    gpio_port_write(PLCD_DATA,LCD_port);
}
#endif // __NOT_USED_ANYMORE


void LCD_writeByte(S_dev_lcd *dev, uint8_t data, E_waitType wType)
{
    gpio_clear(dev->cmd_port,dev->cmdEN);
    if( (dev->i_functionSet & nBit_bus_8bit) != 0)
    { // 8bit dataBus mode
        LCD_writePort(dev, data, wType);
    }
    else
    { // 4bit dataBus mode - Lower then Higher nibble ? OR THE OTHER WAY!!
        LCD_writePort(dev, (data & 0xF0),      wType); // Higher nibble
        LCD_writePort(dev, ((data<<4) & 0xF0), wType); // Lower nibble
    }

    if(wType == sendCmd)
    {
        LCD_saveSettings(dev,data);
    }
}

void LCD_saveSettings(S_dev_lcd* dev, uint8_t cmd_data)
{
    if( (cmd_data&nBit_functionSet) != 0 && (cmd_data&zeroBits_functionSet)==0 )
    { // now we know that the signal was functionSet setting
        dev->i_functionSet = cmd_data;
        // number of lines
        if((cmd_data & nBit_two_lines) != 0) dev->nLines = 2;
        else dev->nLines = 1;
    }
    else if( (cmd_data&nBit_entryMode) != 0 && (cmd_data&zeroBits_entryMode)==0 )
    { // now we know that the signal was functionSet setting
        dev->i_entryMode = cmd_data;
    }
    else if( (cmd_data&nBit_cursorMode) != 0 && (cmd_data&zeroBits_cursorMode)==0 )
    { // now we know that the signal was functionSet setting
        dev->i_entryMode = cmd_data;
    }
}

void LCD_writePort(S_dev_lcd *dev,uint8_t data, E_waitType wType)
{
    LCD_exposePortValue( dev, data );
        gpio_set(dev->cmd_port, dev->cmdEN);
            LCD_waitBusy(setEnable);
        gpio_clear(dev->cmd_port, dev->cmdEN);
    LCD_waitBusy(wType);
}

void LCD_exposePortValue(S_dev_lcd *dev, uint8_t data)
{
    dev->dportVal = 0x0000;
    uint8_t i=0;
    if((dev->i_functionSet & nBit_bus_8bit) == 0)
    { // 4bit data bus mode
        i=4; // start to expose from 4th bit
    }
    for(i=0;i<8;i++)
        if( data & ((1LU)<<i))
            dev->dportVal |= dev->data_pins[i];
    gpio_port_write(dev->data_port, dev->dportVal); // expose data/cmd on data bus
}

void LCD_printChar(S_dev_lcd *dev, char ch)
{
    if( (ch == '\n') || (ch == '\r') )
    {
        LCD_nextLine(dev);
    }
    else
    {
        LCD_writeChar(dev,ch);
    }
    if(dev->i_writeInsideOnly != 0)
    {
        if(dev->actX >= dev->nCharsPerLine)
        {
            LCD_nextLine(dev);
        }
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
