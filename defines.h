/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * General stdiodemo defines
 *
 * $Id: defines.h 2186 2010-09-22 10:25:15Z aboyapati $
 */

/* CPU frequency */
#define F_CPU 16000000UL

/* UART baud rate */
#define UART_BAUD  9600

/* 11 bits temp mode  */
#define bit11

#define BAUD_PRESCALLER (((F_CPU / (UART_BAUD * 16UL))) - 1)    //The formula that does all the required maths

/* HD44780 LCD port connections */
#define HD44780_RS D, 7
#define HD44780_RW D, 5
#define HD44780_E  D, 6
/* The data bits have to be not only in ascending order but also consecutive. */
#define HD44780_D4 B, 0

/* Whether to read the busy flag, or fall back to
   worst-time delays. */
#define USE_BUSY_BIT 1

#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

//---------------------TEMP DEFINES-----------------
#define THERM_PORT PORTD
#define THERM_DDR DDRD
#define THERM_PIN PIND
#define THERM_DQ PD3
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)

#define THERM_CMD_CONVERTTEMP    0x44
#define THERM_CMD_RSCRATCHPAD    0xbe
#define THERM_CMD_WSCRATCHPAD    0x4e
#define THERM_CMD_CPYSCRATCHPAD	 0x48
#define THERM_CMD_RECEEPROM	     0xb8
#define THERM_CMD_RPWRSUPPLY     0xb4
#define THERM_CMD_SEARCHROM	     0xf0
#define THERM_CMD_READROM	     0x33
#define THERM_CMD_MATCHROM	     0x55
#define THERM_CMD_SKIPROM	     0xcc
#define THERM_CMD_ALARMSEARCH	 0xec

#ifdef bit11
#define THERM_DECIMAL_STEPS_11BIT	125
#else
#define THERM_DECIMAL_STEPS_12BIT	625
#endif
//-----------------------------------------------------



//-----------------HEATER DEFINES----------------
#define HEAT_PORT PORTB
#define HEAT_DDR DDRB
#define HEAT_PIN PINB
#define HEAT_DQ PB5

#define HEAT_OUTPUT_MODE() HEAT_DDR|=(1<<HEAT_DQ)
#define HEAT_LOW() HEAT_PORT&=~(1<<HEAT_DQ)
#define HEAT_HIGH() HEAT_PORT|=(1<<HEAT_DQ)



//--------------------auto mode led definitions---------------
#define BUTT_LED PC0
#define BUTT_LED_OUTPUT() DDRC=(1<<PC0)
#define BUTT_LED_HIGH()   PORTC|=(1<<PC0);
#define BUTT_LED_LOW()    PORTC&=~(1<<PC0);
//----------------------------------------------------

//-----------------switch definitions ----------------------
#define SWITCH_DQ 		   PC5
#define SWITCH_INPUT()    DDRC &= ~(1<<PC5)
#define SWITCH_PIN        PINC
#define SWITCH_ON         (SWITCH_PIN & (1<<SWITCH_DQ))
//-------------------------------------------------------


//----------------heater led in manual mode definitions---------------
#define SWITCH_LED 		     PC4
#define SWITCH_LED_OUTPUT() DDRC=(1<<PC0)
#define SWITCH_LED_HIGH()   PORTC|=(1<<PC0);
#define SWITCH_LED_LOW()    PORTC&=~(1<<PC0);

