/**
 * AVR32 UC3 template for TTK4147
 */

// include header files for all drivers that have been imported from AVR Software Framework (ASF).
#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

// defines for USB UART
#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A AVR32_PIN_PA31
#define RESPONSE_A AVR32_PIN_PA30
#define TEST_B AVR32_PIN_PA29
#define RESPONSE_B AVR32_PIN_PA28
#define TEST_C AVR32_PIN_PA27
#define RESPONSE_C AVR32_PIN_PB00

// declare interrupts
__attribute__((__interrupt__)) 
static void interrupt_J3(void);

void init()
{
	// board init
	sysclk_init();
	board_init();
	busy_delay_init(BOARD_OSC0_HZ);
	
	// interrupts init
	cpu_irq_disable();
    INTC_init_interrupts();
	INTC_register_interrupt(&interrupt_J3,   AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
	cpu_irq_enable();

	//  stdio init
	stdio_usb_init(&CONFIG_USART_IF);

	// Specify that stdout and stdin should not be buffered.

#if defined(__GNUC__) && defined(__AVR32__)
	setbuf(stdout, NULL);
	setbuf(stdin,  NULL);
#endif
}

/*********************************************************************
User decelerations
*********************************************************************/

/*********************************************************************
Interrupt routines
*********************************************************************/
__attribute__((__interrupt__)) 
static void interrupt_J3(void) 
{ 

}

/*********************************************************************
Functions
*********************************************************************/
int main (void){
	
	// initialize
	init();
	
	// start code from here
	gpio_set_gpio_pin(RESPONSE_A);
	gpio_set_gpio_pin(RESPONSE_B);
	gpio_set_gpio_pin(RESPONSE_C);
	while(1){
		while (gpio_get_pin_value(TEST_A));
		
		gpio_clr_gpio_pin(RESPONSE_A);
		busy_delay_us(5);
		gpio_set_gpio_pin(RESPONSE_A);
		
				
		while (gpio_get_pin_value(TEST_B));
		
		gpio_clr_gpio_pin(RESPONSE_B);
		busy_delay_us(5);
		gpio_set_gpio_pin(RESPONSE_B);
		
				
		while (gpio_get_pin_value(TEST_C));
		
		gpio_clr_gpio_pin(RESPONSE_C);
		busy_delay_us(5);
		gpio_set_gpio_pin(RESPONSE_C);
	}
}
