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

volatile int flagA = 0;
volatile int flagB = 0;
volatile int flagC = 0;

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
static void interrupt_J3(void){ 
	if(gpio_get_pin_interrupt_flag(TEST_A)){
		flagA = 1;
		gpio_clear_pin_interrupt_flag(TEST_A);	
	}
	if(gpio_get_pin_interrupt_flag(TEST_B)){
		flagB = 1;
		gpio_clear_pin_interrupt_flag(TEST_B);
	}
	if(gpio_get_pin_interrupt_flag(TEST_C)){
		flagC = 1;
		gpio_clear_pin_interrupt_flag(TEST_C);
	}
}


/*static void interrupt_J3(void){
if(gpio_get_pin_interrupt_flag(TEST_A)){
gpio_clr_gpio_pin(RESPONSE_A);
busy_delay_us(5);
gpio_clear_pin_interrupt_flag(TEST_A);
gpio_set_gpio_pin(RESPONSE_A);

}
if(gpio_get_pin_interrupt_flag(TEST_B)){
gpio_clr_gpio_pin(RESPONSE_B);
busy_delay_us(5);
gpio_clear_pin_interrupt_flag(TEST_B);
gpio_set_gpio_pin(RESPONSE_B);
}
if(gpio_get_pin_interrupt_flag(TEST_C)){
gpio_clr_gpio_pin(RESPONSE_C);
busy_delay_us(5);
gpio_clear_pin_interrupt_flag(TEST_C);
gpio_set_gpio_pin(RESPONSE_C);
}

}
*/

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
	gpio_enable_pin_interrupt(TEST_A, 2);
	gpio_enable_pin_interrupt(TEST_B, 2);
	gpio_enable_pin_interrupt(TEST_C, 2);
	
	while(1){
		if(flagA){
			gpio_clr_gpio_pin(RESPONSE_A);
			cpu_delay_us(5, BOARD_OSC0_HZ);
			gpio_set_gpio_pin(RESPONSE_A);
			flagA = 0;
		}
		if(flagB){
			gpio_clr_gpio_pin(RESPONSE_B);
			cpu_delay_us(5, BOARD_OSC0_HZ);
			gpio_set_gpio_pin(RESPONSE_B);
			flagB = 0;
		}
		if(flagC){
			gpio_clr_gpio_pin(RESPONSE_C);
			cpu_delay_us(5, BOARD_OSC0_HZ);
			gpio_set_gpio_pin(RESPONSE_C);
			flagC = 0;
		}	
	}	
}




	/*// start code from here
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
	}*/