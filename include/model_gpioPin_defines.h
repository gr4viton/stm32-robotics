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
#ifndef _XX_H_
#define _XX_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
//____________________________________________________
//constants (do not change)

/****************
 @brief number of pins defined in predef_gpioPin array
 ****************/
#define PREDEFCOUNT_GPIOPIN 20
/****************
 @brief defines of predef_gpioPin array indexes
 now as the predef_gpioPin is not ordered - it is defined one by one
 later implement the next #if statements and order predef_gpioPin
 ****************/
/*
#define PA15    0
#define PB3     1
#define PA3     2
#define PA1     3
#define PD6     4
#define PB7     5
#define PA7     6
#define PA5     7
#define PE5     8
#define PE1     9
*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// LATER ON PRODUCE THIS , and fill the predef_gpioPin

// as the predef_gpioPin is not ordered

#define PORTS_BASE      0x0000

#define PORTS_OFFSET    0x0010
#define PORT_BASE(PORT) ((PORTS_BASE)+(PORT)*(PORTS_OFFSET))

//____________________________________________________
// port bases
#define PA_BASE 		(PORT_BASE(0))
#define PB_BASE 		(PORT_BASE(1))
#define PC_BASE 		(PORT_BASE(2))
#define PD_BASE 		(PORT_BASE(3))
#define PE_BASE 		(PORT_BASE(4))
#define PH_BASE 		(PORT_BASE(5))
#define PI_BASE 		(PORT_BASE(6))


//____________________________________________________
// pin bases
#define PA(PIN) 		((PA_BASE)+(PIN))
#define PB(PIN) 		((PB_BASE)+(PIN))
#define PC(PIN) 		((PC_BASE)+(PIN))
#define PD(PIN) 		((PD_BASE)+(PIN))
#define PE(PIN) 		((PE_BASE)+(PIN))
#define PH(PIN) 		((PH_BASE)+(PIN))
#define PI(PIN) 		((PI_BASE)+(PIN))


//____________________________________________________
// pin indexes

/* PORTA */
#define PA0 		PA(0)
#define PA1 		PA(1)
#define PA2 		PA(2)
#define PA3 		PA(3)
#define PA4 		PA(4)
#define PA5 		PA(5)
#define PA6 		PA(6)
#define PA7 		PA(7)
#define PA8 		PA(8)
#define PA9 		PA(9)
#define PA10 		PA(10)
#define PA11 		PA(11)
#define PA12 		PA(12)
#define PA13 		PA(13)
#define PA14 		PA(14)
#define PA15 		PA(15)

/* PORTB */
#define PB0 		PB(0)
#define PB1 		PB(1)
#define PB2 		PB(2)
#define PB3 		PB(3)
#define PB4 		PB(4)
#define PB5 		PB(5)
#define PB6 		PB(6)
#define PB7 		PB(7)
#define PB8 		PB(8)
#define PB9 		PB(9)
#define PB10 		PB(10)
#define PB11 		PB(11)
#define PB12 		PB(12)
#define PB13 		PB(13)
#define PB14 		PB(14)
#define PB15 		PB(15)

/* PORTC */
#define PC0 		PC(0)
#define PC1 		PC(1)
#define PC2 		PC(2)
#define PC3 		PC(3)
#define PC4 		PC(4)
#define PC5 		PC(5)
#define PC6 		PC(6)
#define PC7 		PC(7)
#define PC8 		PC(8)
#define PC9 		PC(9)
#define PC10 		PC(10)
#define PC11 		PC(11)
#define PC12 		PC(12)
#define PC13 		PC(13)
#define PC14 		PC(14)
#define PC15 		PC(15)

/* PORTD */
#define PD0 		PD(0)
#define PD1 		PD(1)
#define PD2 		PD(2)
#define PD3 		PD(3)
#define PD4 		PD(4)
#define PD5 		PD(5)
#define PD6 		PD(6)
#define PD7 		PD(7)
#define PD8 		PD(8)
#define PD9 		PD(9)
#define PD10 		PD(10)
#define PD11 		PD(11)
#define PD12 		PD(12)
#define PD13 		PD(13)
#define PD14 		PD(14)
#define PD15 		PD(15)

/* PORTE */
#define PE0 		PE(0)
#define PE1 		PE(1)
#define PE2 		PE(2)
#define PE3 		PE(3)
#define PE4 		PE(4)
#define PE5 		PE(5)
#define PE6 		PE(6)
#define PE7 		PE(7)
#define PE8 		PE(8)
#define PE9 		PE(9)
#define PE10 		PE(10)
#define PE11 		PE(11)
#define PE12 		PE(12)
#define PE13 		PE(13)
#define PE14 		PE(14)
#define PE15 		PE(15)

/* PORTH */
#define PH0 		PH(0)
#define PH1 		PH(1)
#define PH2 		PH(2)
#define PH3 		PH(3)
#define PH4 		PH(4)
#define PH5 		PH(5)
#define PH6 		PH(6)
#define PH7 		PH(7)
#define PH8 		PH(8)
#define PH9 		PH(9)
#define PH10 		PH(10)
#define PH11 		PH(11)
#define PH12 		PH(12)
#define PH13 		PH(13)
#define PH14 		PH(14)
#define PH15 		PH(15)

/* PORTI */
#define PI0 		PI(0)
#define PI1 		PI(1)
#define PI2 		PI(2)
#define PI3 		PI(3)
#define PI4 		PI(4)
#define PI5 		PI(5)
#define PI6 		PI(6)
#define PI7 		PI(7)
#define PI8 		PI(8)
#define PI9 		PI(9)
#define PI10 		PI(10)
#define PI11 		PI(11)
#define PI12 		PI(12)
#define PI13 		PI(13)
#define PI14 		PI(14)
#define PI15 		PI(15)



//____________________________________________________
// exti multilines
// for stm32f4
#define NVIC_EXTI5_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI6_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI7_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI8_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI9_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI10_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI11_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI12_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI13_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI14_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI15_IRQ     NVIC_EXTI15_10_IRQ


//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
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
    //____________________________________________________
    // ..
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

#endif  // _XX_H_


