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
/*
 inspiration:
 inductor
*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "dev_serial.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
typedef struct {
    uint32_t device;
    uint32_t clk;
    uint8_t irq;
    uint8_t af;
    uint32_t txpclk;
    uint32_t rxpclk;
    uint32_t txport;
    uint32_t rxport;
    uint16_t txpin;
    uint16_t rxpin;

    struct ringbuf tx_ring;
    struct ringbuf rx_ring;

    uint32_t stats_rxoverruns;
    uint32_t stats_rxerrors;

} predef_uart_device_t;
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DEFINITIONS
//____________________________________________________
// static variables
static ssize_t _iord(void *_cookie, char *_buf,size_t _n);
static ssize_t _iowr(void *_cookie,const char *_buf, size_t _n);
static int _ioseek(void *_cookie, off_t *_off,int _whence);
static int _ioclose(void *_cookie);
static void _isru(predef_uart_device_t *dev);
static void _txsignal(struct ringbuf *rb);

static predef_uart_device_t uarts[] =
{
    /*0*/{ .device=USART1, .irq=NVIC_USART1_IRQ, .txport=GPIOA, .txpin=GPIO9,  .rxport=GPIOA, .rxpin=GPIO10, .af=GPIO_AF7, .clk=RCC_USART1, .txpclk=RCC_GPIOA, .rxpclk=RCC_GPIOA },
    /* used begin */
    /*1*/{ .device=USART2, .irq=NVIC_USART2_IRQ, .txport=GPIOD, .txpin=GPIO5,  .rxport=GPIOD, .rxpin=GPIO6,  .af=GPIO_AF7, .clk=RCC_USART2, .txpclk=RCC_GPIOD, .rxpclk=RCC_GPIOD  },
    /*2*/{ .device=USART3, .irq=NVIC_USART3_IRQ, .txport=GPIOD, .txpin=GPIO8,  .rxport=GPIOD, .rxpin=GPIO9,  .af=GPIO_AF7, .clk=RCC_USART3, .txpclk=RCC_GPIOD, .rxpclk=RCC_GPIOD  },
    /* used end */
    /*3*/{ .device=UART4,  .irq=NVIC_UART4_IRQ,  .txport=GPIOC, .txpin=GPIO10, .rxport=GPIOC, .rxpin=GPIO11, .af=GPIO_AF8, .clk=RCC_UART4 , .txpclk=RCC_GPIOC, .rxpclk=RCC_GPIOC  },
    /*4*/{ .device=UART5,  .irq=NVIC_UART5_IRQ,  .txport=GPIOC, .txpin=GPIO12, .rxport=GPIOD, .rxpin=GPIO2,  .af=GPIO_AF8, .clk=RCC_UART5 , .txpclk=RCC_GPIOC, .rxpclk=RCC_GPIOD  },
    /*5*/{ .device=USART6, .irq=NVIC_USART6_IRQ, .txport=GPIOC, .txpin=GPIO6,  .rxport=GPIOC, .rxpin=GPIO7,  .af=GPIO_AF8, .clk=RCC_USART6, .txpclk=RCC_GPIOC, .rxpclk=RCC_GPIOC  },
};
//____________________________________________________
// other variables
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
static inline size_t min(const size_t a,const size_t b)
{
    return a < b ? a : b;
}

void usa_rxb(uint8_t ch);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________
    // Interrupt handling functions
void usart1_isr(void) { _isru(&uarts[0]); }
void usart2_isr(void) { _isru(&uarts[1]); }
void usart3_isr(void) { _isru(&uarts[2]); }
void uart4_isr(void)  { _isru(&uarts[3]); }
void uart5_isr(void)  { _isru(&uarts[4]); }
void usart6_isr(void) { _isru(&uarts[5]); }


FILE *fopenserial(uint8_t index,uint32_t baudrate, uint8_t *tbuf, size_t tbufsz, uint8_t *rbuf, size_t rbufsz)
{
    predef_uart_device_t *dev = &uarts[index];

    // initialize ring buffers
    ringbuf_init(&(dev->tx_ring),tbuf,tbufsz);
    ringbuf_init(&(dev->rx_ring),rbuf,rbufsz);

    dev->tx_ring.signal = _txsignal;

    // enable the clocks
    rcc_periph_clock_enable(dev->clk);
    rcc_periph_clock_enable(dev->rxpclk);
    rcc_periph_clock_enable(dev->txpclk);

    // init pins
    gpio_mode_setup(dev->txport, GPIO_MODE_AF, GPIO_PUPD_PULLUP, dev->txpin);
    gpio_mode_setup(dev->rxport, GPIO_MODE_AF, GPIO_PUPD_PULLUP, dev->rxpin);
    gpio_set_af(dev->txport, dev->af, dev->txpin);
    gpio_set_af(dev->rxport, dev->af, dev->rxpin);
    gpio_set_output_options(dev->rxport, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, dev->rxpin);

    // init usart
    usart_set_baudrate(dev->device, baudrate);
	usart_set_databits(dev->device, 8);
	usart_set_parity(dev->device, USART_PARITY_NONE);
	usart_set_stopbits(dev->device, USART_STOPBITS_1);
	usart_set_mode(dev->device, USART_MODE_TX_RX);
	usart_set_flow_control(dev->device, USART_FLOWCONTROL_NONE);

    // init irqs
	usart_enable_rx_interrupt(dev->device);
	nvic_enable_irq(dev->irq);

    // run the serail line
    usart_enable(dev->device);

    // stdio stub
    cookie_io_functions_t stub = { _iord, _iowr, _ioseek, _ioclose };
    FILE *fp = fopencookie(dev,"rw+",stub);
    setvbuf(fp, NULL, _IONBF, 0);
    return fp;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

    //____________________________________________________
    // Low level
struct ringbuf *ser_txbuf(uint8_t index)
{
    return &uarts[index].tx_ring;
}

static ssize_t _iord(void *_cookie, char *_buf,size_t _n)
{
    // dont support reading now
    UNUSED(_cookie);
    UNUSED(_buf);
    UNUSED(_n);
    return 0;
}

static ssize_t _iowr(void *_cookie,const char *_buf, size_t _n)
{
    predef_uart_device_t *dev = (predef_uart_device_t*)_cookie;

    int written = 0;
    int c = 0;
    do {
        nvic_disable_irq(dev->irq);

        c = ringbuf_write_buffer_partial(&dev->tx_ring, _n, (const uint8_t *)_buf);

        usart_enable_tx_interrupt(dev->device);
        nvic_enable_irq(dev->irq);

        _n -= c;
        _buf += c;
        written += c;

        if (_n > 0)                // wait for the write some character succeeded
            while (ringbuf_write_available(&dev->tx_ring) < min(_n, dev->tx_ring.mask-1))
                {}; // generic idle
    } while (_n > 0);
    return written;
}

static int _ioseek(void *_cookie, off_t *_off,int _whence)
{
    UNUSED(_whence);
    UNUSED(_off);
    UNUSED(_cookie);
    return EINVAL;
}

static int _ioclose(void *_cookie)
{
    UNUSED(_cookie);
    return EINVAL;
}


static void _isru(predef_uart_device_t *dev)
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
        /*
        if (!ringbuf_write_byte(&dev->rx_ring, ch))
        {
            // data overrun occurs, current char will not be added
            dev->stats_rxoverruns++;
        }
        */
        /*if (proto_rx64_extract(&dev->rx_ring) == PROTO_NOTMY)
            ringbuf_skip(&dev->rx_ring,1);*/
    }
}

static void _txsignal(struct ringbuf *rb)
{
    for (int i=0; i< 6; i++)
    {
        if (rb != &uarts[i].tx_ring)
            continue;

        usart_enable_tx_interrupt(uarts[i].device);
    }
}




