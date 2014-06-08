/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	dev_LCD_HD44780.c
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief      Expands LCD driver of the function to write to it through fprintf - to use LCD as a FILE
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "dev_LCD_HD44780.h"


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
/****************
 \brief Predefined mapping of [LCD data pins] to [MCU port pins]
 D0 D1 D2 D3 D4 D5 D6 D7 ?
 ****************/
static uint16_t lcd_device_data_predef[][8] =
{
/*0*/{ GPIO8, GPIO10, GPIO12, GPIO14, GPIO7, GPIO9, GPIO11, GPIO13 },
/*1*/{ GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7 },
/*2*/{ GPIO7, GPIO9, GPIO11, GPIO13, GPIO8, GPIO10, GPIO12, GPIO14  }
};
/****************
 \brief Predefined LCD ports & clocks for CMD(control-command) and DATA buses
 ****************/
// later on this could be static as you probably want to use the LCD only through cookie
S_dev_lcd lcds_predef[] =
{
/*0*/{ .cmd_port=GPIOD, .cmdp_clk=RCC_GPIOD, .data_port=GPIOE,  .datap_clk=RCC_GPIOE },
/*1*/{ .cmd_port=GPIOB, .cmdp_clk=RCC_GPIOB, .data_port=GPIOE,  .datap_clk=RCC_GPIOE }
};

#if __NOT_USED_ANYMORE
static lcd_device_data_port_t lcd_device_data_predef[] = {
    { .D0= GPIO7,   .D1= GPIO9,   .D2= GPIO11,  .D3= GPIO13,
      .D4= GPIO8,   .D5= GPIO10,  .D6= GPIO12,  .D7= GPIO14 },
    { .D0= GPIO0,   .D1= GPIO1,   .D2= GPIO2,  .D3= GPIO3,
      .D4= GPIO4,   .D5= GPIO5,  .D6= GPIO6,  .D7= GPIO7 }
};
#endif // __NOT_USED_ANYMORE

#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
void gpiod_isr(void) { _isru(&uarts[0]); }
void usart2_isr(void) { _isru(&uarts[1]); }
void usart3_isr(void) { _isru(&uarts[2]); }
void uart4_isr(void)  { _isru(&uarts[3]); }
void uart5_isr(void)  { _isru(&uarts[4]); }
void usart6_isr(void) { _isru(&uarts[5]); }
#endif // __NOT_IMPLEMENTED_YET__BUSY_WAITING

//____________________________________________________
// other variables

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
static ssize_t _iord(void *_cookie, char *_buf,size_t _n);
static ssize_t _iowr(void *_cookie,const char *_buf, size_t _n);
static int _ioseek(void *_cookie, off_t *_off,int _whence);
static int _ioclose(void *_cookie);
static void _txsignal(struct ringbuf *rb);
#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
static void _isru(S_dev_lcd *dev);
#endif // __NOT_IMPLEMENTED_YET__BUSY_WAITING

void usa_rxb(uint8_t ch);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
static inline size_t min(const size_t a,const size_t b)
{
    return a < b ? a : b;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________
    // DATAPINS
void LCD_setDataPins(uint8_t index, S_dev_lcd *dev){
    uint16_t _data_pins_all  = 0x0000;
    uint8_t i = 0;
    for(i=0;i<8;i++)
    {
        dev->data_pins[i] = lcd_device_data_predef[index][i];
        _data_pins_all |= dev->data_pins[i];
    }
    dev->data_pins_all = _data_pins_all;
}

uint16_t LCD_getMaskDataPins(S_dev_lcd *dev)
{
    uint16_t _data_pins_all = 0x0000;
    uint8_t i = 0;
    for(;i<8;i++)
        _data_pins_all |= dev->data_pins[i];
    return(_data_pins_all);
}
    //____________________________________________________
    // cookie - file access to lcd
FILE *fopenLCD(uint8_t index, uint8_t indexPins, uint8_t nCharsPerLine,
               uint8_t writeInsideOnly, uint8_t functionSet,
               uint8_t entryMode, uint8_t cursorMode,
               uint8_t *dbuf, size_t dbufsz)
{
    S_dev_lcd *dev = &lcds_predef[index];

    // initialize ring buffers - not using for lcd yet
    ringbuf_init(&(dev->data_ring),dbuf,dbufsz);
    //ringbuf_init(&(dev->rx_ring),rbuf,rbufsz);
    dev->data_ring.signal = _txsignal;

    //____________________________________________________
    // SW interface
    dev->actY = 0;
    dev->actX = 0;
    dev->nCharsPerLine = nCharsPerLine;
    dev->nLines = (functionSet & nBit_two_lines) != 0 ? 2: 1;
    dev->i_writeInsideOnly = writeInsideOnly;

    //____________________________________________________
    // PORTS
    // predef ports and clks
    dev->datap_clk = lcds_predef[index].datap_clk;
    dev->data_port = lcds_predef[index].data_port;
    dev->cmdp_clk = lcds_predef[index].cmdp_clk;
    dev->cmd_port = lcds_predef[index].cmd_port;
    //____________________________________________________
    // PINS
    // data pins
    LCD_setDataPins(indexPins,dev);
    // cmd pins
    dev->cmdRS = GPIO11;
    dev->cmdRW = GPIO13;
    dev->cmdEN = GPIO15;
    dev->cmd_pins_all = 0x0000 | (dev->cmdRS | dev->cmdRW | dev->cmdEN);

    //____________________________________________________
    // ENABLE HW
    // enable the clocks
    rcc_periph_clock_enable(dev->cmdp_clk);
    rcc_periph_clock_enable(dev->datap_clk);

    // init pins
    gpio_mode_setup(dev->data_port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, dev->data_pins_all);
    gpio_mode_setup(dev->cmd_port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, dev->cmd_pins_all);

    // reset lcd pins
	gpio_clear(dev->data_port, dev->data_pins_all); // Data bus low
    gpio_clear(dev->cmd_port, dev->cmd_pins_all); // Control bus low, Enable display

    //____________________________________________________
    //pwr pin -> transistor switch
    uint16_t pwrPin = GPIO15;
    uint32_t pwrClk = RCC_GPIOE;
    uint32_t pwrPort = GPIOE;

    rcc_periph_clock_enable(pwrClk);
    mswait(1);
    //gpio_mode_setup(pwrPort, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pwrPin);
    gpio_mode_setup(pwrPort, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, pwrPin);

    // turn the display off
	gpio_clear(pwrPort, pwrPin);

    // turn the display on
	gpio_set(pwrPort, pwrPin);

    //____________________________________________________
    // INIT LCD
    // initial sequence for buggy response - cheap china lcds ready flag bug fix
    LCD_writeCmd(dev,init_functionSet);
    LCD_waitBusy(sendCmd);
    LCD_writeCmd(dev,init_functionSet);
    LCD_waitBusy(sendData);
    LCD_writeCmd(dev,init_functionSet);

    // set initial params
    LCD_writeCmd(dev,functionSet); // written in 8bit mode
    //dev->i_functionSet = functionSet; // not able to change within LCD_setFunctionSet..

    LCD_writeCmd(dev,functionSet); // written in 4bit mode (if selected)
    LCD_writeCmd(dev,entryMode);
    LCD_writeCmd(dev,cursorMode);

    // init irqs
	//usart_enable_rx_interrupt(dev->device);
	//nvic_enable_irq(dev->irq);
    // take advantage of TIMer of RTC?

    // stdio stub
    cookie_io_functions_t stub = { _iord, _iowr, _ioseek, _ioclose };
    FILE *fp = fopencookie(dev,"rw+",stub);
    setvbuf(fp, NULL, _IONBF, 0);
    return fp;
}
    //____________________________________________________
    // testin'
void LCD_displayWriteCheck(S_dev_lcd *dev)
{
    uint8_t xmax = dev->nCharsPerLine - 1;
    uint8_t ymax = dev->nLines;
    uint8_t x = 0;
    uint8_t y = 0;

    LCD_clear(dev);
    LCD_waitBusy(dispCheck);
    while(1){
        LCD_gotoxy(dev,x,y);
        LCD_writeChar(dev,'0'+x);
        if(x == xmax){ y++; x=0; }
        else         { x++; }
        if(y == ymax) break;
    }
    LCD_waitBusy(dispCheck);
}

void dev_LCD_checkSeek(FILE* flcd)
{
    uint8_t b = 0;
    while(b!=32) // b reset value should be nCharsPerLine * nLines
    {
        fseek(flcd,b,SEEK_SET);
        fputc('x',flcd);
        twait(10);
        b++;
    }
}
    //____________________________________________________
    // low level

static ssize_t _iord(void *_cookie, char *_buf,size_t _n)
{
    // dont support reading now
    UNUSED(_cookie);
    UNUSED(_buf);
    UNUSED(_n);
    return 0;
}

static ssize_t _iowr(void *_cookie, const char *_buf, size_t _n)
{
    // tohle bude volat druhou funkci LCD_write s parametrem DATA (vždy)
    // pomoci fopencookie nebude možné nastavovat LCD -> pouze psát data
    S_dev_lcd *dev = (S_dev_lcd *)_cookie;
    // prozatím vynechám buffer
    int written = 0;
    //int c = 0;
    uint8_t i = 0;
    do {
        LCD_printChar(dev,(uint8_t)_buf[i]);
        _n--;
        written++;
    } while(_n>0);//_buf[i]!='\0');

    return written;
#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
    do {
        //nvic_disable_irq(dev->irq);

        c = ringbuf_write_buffer_partial(&dev->tx_ring, _n, (const uint8_t *)_buf);

        //usart_enable_tx_interrupt(dev->device);
        //nvic_enable_irq(dev->irq);

        _n -= c;
        _buf += c;
        written += c;

        if (_n > 0)                // wait for the write some character succeeded
            while (ringbuf_write_available(&dev->tx_ring) < min(_n, dev->tx_ring.mask-1))
                {}; // generic idle
    } while (_n > 0);
    return written;
#endif // __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
}

static int _ioseek(void *_cookie, off_t *_off,int _whence)
{
    S_dev_lcd *dev = (S_dev_lcd *)_cookie;

    uint16_t pos = 0;
    switch(_whence){
        case(SEEK_SET): pos = 0; break;
        case(SEEK_CUR): /*GET_POS..*/; break;
        case(SEEK_END): pos = 0x20; break;
        default: break;
    }
    // add offset
    if(*_off <= dev->nCharsPerLine * dev->nLines )
    { // offset is inside the display
        if(*_off >= dev->nCharsPerLine)
        { // second line
            pos = LCD_C_CUR_ADDRESS_L2 + (*_off - dev->nCharsPerLine );
        }
        else
        { // first line
            pos = LCD_C_CUR_ADDRESS_L1 + *_off;
        }

    }
    LCD_writeCmd(dev,pos);

    return(0);
}

static int _ioclose(void *_cookie)
{
    UNUSED(_cookie);
    return EINVAL;
}

#if __NOT_IMPLEMENTED_YET

struct ringbuf *ser_txbuf(uint8_t index)
{
    return &uarts[index].tx_ring;
}
#endif // __NOT_IMPLEMENTED_YET

#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
static void _isru(uart_device_t *dev)
{
    uint8_t ch;

    if (usart_get_interrupt_source(dev->device,USART_SR_TXE))
    {
        if (ringbuf_read_byte(&dev->tx_ring, &ch))
            usart_send(dev->device, ch);
        else
            usart_disable_tx_interrupt(dev->device);
    }

    if (usart_get_interrupt_source(dev->device,USART_SR_RXNE))
    {
        ch = (char)usart_recv(dev->device);

        //usa_rxb(ch);

        if (!ringbuf_write_byte(&dev->rx_ring, ch))
        {
            // data overrun occurs, current char will not be added
            dev->stats_rxoverruns++;
        }

        if (proto_rx64_extract(&dev->rx_ring) == PROTO_NOTMY)
            ringbuf_skip(&dev->rx_ring,1);

    }
}
#endif // __NOT_IMPLEMENTED_YET__BUSY_WAITING

static void _txsignal(struct ringbuf *rb)
{
    UNUSED(rb);
    /*
    for (int i=0; i< 6; i++)
    {
        if (rb != &uarts[i].tx_ring)
            continue;

        usart_enable_tx_interrupt(uarts[i].device);
    }*/
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

