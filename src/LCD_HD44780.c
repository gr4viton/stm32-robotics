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
        case(setEnable): mswait(1); break;  // Wait At Least 450 ns!!!
        case(dispCheck): mswait(500); break;  // long for refreshment - LCD_displayWriteCheck
        default: mswait(5);
    }

}

void LCD_clear(S_dev_lcd *dev)
{
    LCD_writeCmd(dev,LCD_C_CLR0);
}

// indexed from zero
uint8_t LCD_gotoxy(S_dev_lcd *dev, uint8_t x, uint8_t y){
    if(x>HD44780_P_XMAX-1) return('x');
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

void LCD_writeChar(S_dev_lcd *dev,uint8_t ch){
    LCD_write(dev,ch,sendData);
}
void LCD_writeCmd(S_dev_lcd *dev,uint8_t cmd_data){
    LCD_write(dev,cmd_data,sendCmd);
    if( (cmd_data & 0xE0) == 0x20 )
    { // Set Interface Length
        if(cmd_data & 0x08 )
        {// number of lines bit = 1 == 2lines
            dev->nLines = 2;
        }
        else dev->nLines = 1;
    }
}

void LCD_write(S_dev_lcd *dev, uint8_t data, E_waitType wType){
    gpio_clear(dev->cmd_port, dev->cmdRW); // cmd: writing
    if(wType == sendCmd){
        gpio_clear(dev->cmd_port, dev->cmdRS); // cmd: sending cmd
        LCD_waitBusy(setEnable);
        LCD_writeByte(dev,data, sendCmd);
    }
    else {
        gpio_set(dev->cmd_port, dev->cmdRS); // cmd: sending data
        LCD_waitBusy(setEnable);
        LCD_writeByte(dev,data, sendData);
    }
}

#ifdef LCD_4DATA_BIT

void LCD_writePort(uint8_t data){
    uint16_t LCD_port = 0x0000;
    if( data & GPIO4) LCD_port |= LCD_D4;
    if( data & GPIO5) LCD_port |= LCD_D5;
    if( data & GPIO6) LCD_port |= LCD_D6;
    if( data & GPIO7) LCD_port |= LCD_D7;

    gpio_port_write(PLCD_DATA,LCD_port);
}

void LCD_writeByte(uint8_t data, E_waitType wType){
    gpio_clear(PLCD_CMD,LCD_EN);
    // write HIGH NIBBLE of the data/command on D7-4
    uint8_t nibble = data & 0xF0;
    LCD_writePort( nibble );
        gpio_set(PLCD_CMD, LCD_EN);
            LCD_waitBusy(setEnable);
        gpio_clear(PLCD_CMD,LCD_EN);
    LCD_waitBusy(wType);

    // write the LOW NIBBLE of the data/command on D7-4
    nibble = (data<<4) & 0xF0;
    LCD_writePort(nibble);
        gpio_set(PLCD_CMD, LCD_EN);
            LCD_waitBusy(setEnable);
        gpio_clear(PLCD_CMD,LCD_EN);
    LCD_waitBusy(wType);
}
#endif // LCD_4DATA_BIT
#ifdef LCD_8DATA_BIT

void LCD_writePort(S_dev_lcd *dev, uint8_t data){
    uint16_t LCD_port = 0x0000;
    uint8_t i=0;
    for(;i<8;i++)
        if( data & ((1LU)<<i))
            LCD_port |= dev->data_pins[i];
    gpio_port_write(dev->data_port, LCD_port); // expose data/cmd on data bus
}

void LCD_writeByte(S_dev_lcd *dev, uint8_t data, E_waitType wType){
    gpio_clear(dev->cmd_port,dev->cmdEN);
    LCD_writePort( dev, data );
        gpio_set(dev->cmd_port, dev->cmdEN);
        __asm__("nop");__asm__("nop");__asm__("nop");
        gpio_clear(dev->cmd_port, dev->cmdEN);
    LCD_waitBusy(wType);
}
#endif // LCD_8DATA_BIT



#if __NOT_USED_ANYMORE
void INIT_LCD(void)
{
    // write something here? - i.e the initialization from <dev_LCD_HD44780.c> ??
    /*
    // enable the clocks
    rcc_periph_clock_enable(RCC_LCD_DATA);
    rcc_periph_clock_enable(RCC_LCD_CMD);

    // init pins
	gpio_mode_setup(PLCD_DATA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_DATA_ALL);
	gpio_mode_setup(PLCD_CMD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CMD_ALL);
	// pin value
	gpio_clear(PLCD_DATA, LCD_DATA_ALL); // Data bus low
    gpio_clear(PLCD_CMD, LCD_RS|LCD_RW); // Control bus low
    gpio_clear(PLCD_CMD,LCD_EN);        // Enable display
    //----------------------------------------------------------
*/
/*
        //if(b==0) LCD_pos0();
        //LCD_writeCmd(0x38); // 2 line - no backlight
        //LCD_writeCmd(0x30); // 1 line + backlight
        //LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)

        LCD_writeCmd(0x30); asmwait(10);
        LCD_writeCmd(0x30); asmwait(10);
        LCD_writeCmd(0x30); asmwait(10);

        //LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)
        LCD_writeCmd(0x30); // Function set (8a-bit interface, 2 lines, 5*7 Pixels) + back
        LCD_writeCmd(0x06); // entry mode - normal

        LCD_writeCmd(0x0F); // Turn on visible blinking-block cursor
        LCD_writeCmd(LCD_C_CLR0); // Home (move cursor to top/left character position)
*/


    //char ch = 's';

/*
    //LCD_writeCmd(0x28); // 2 line 5x7 dots
    // LCD Commands = http://www.geocities.com/dinceraydin/lcd/commands.htm
    LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)
    LCD_writeCmd(0x0F); // Turn on visible blinking-block cursor

    LCD_writeCmd(0x06); // Display Shift :OFF, Increment Address Counter
    */
}
#endif // __NOT_USED_ANYMORE

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
