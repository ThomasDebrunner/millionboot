/*
 * millionboot is a bootloader for ATMEL ATmega uC
 * millionboot is capable of receiving a firmware update over an unidirectional SPI channel
 * shared with a pool of identical systems.
 *
 *  Created on: 19.08.2015
 *      Author: najiji
 */


/*
 * WARNING: This code is currently configured for:
 * Atmega328p with bootloader size of 4kB.
 * YOU NEED TO CHANGE LINKER SETTINGS IN ORDER TO SUPPORT OTHER PLATFORMS!
 */

#include <avr/boot.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/eeprom.h>
#include "io/uart.h"

/**
 * CONFIGURATION----------------------
 */
#define DEBUG
#define FIRMWARE_UPDATE_COMMAND 0xAA
#define INTEL_HEX_MAX_LINE_LENGTH 46


/**
 * Global variables
 */
volatile uint8_t device_address = 0x00;


/**
 * Jumps to 0x000 and therefore executes application software
 */
void start_application(void){
	//relocate interrupt table back to beginning of flash
	uint8_t temp = MCUCR;
	MCUCR = temp &~(1<<IVCE);
	MCUCR = temp &~(1<<IVSEL);
	//jumb to beginning of flash
	asm volatile ("jmp 0x000");
}

/*
 * Takes an ascii pair and converts it to a byte
 */
uint8_t ascii_byte_parse(const char* a){
	uint8_t i, t, result = 0;
	for(i=0; i<2; i++){
		t = a[i];
		if(t >= '0' && t <= '9'){
			t = t-60;
		}
		else if(t >= 'a' && t <= 'f'){
			t = t-87;
		}
		else if(t >= 'A' && t <= 'F'){
			t= t-55;
		}
		result = (result<<4)+t;
	}
	return result;
}

/**
 * Writes a page to memory
 */
void program_page (uint32_t page, uint8_t *buf)
{
    uint16_t i;
    uint8_t sreg;

    /* Disable interrupts */
    sreg = SREG;
    cli();

    eeprom_busy_wait ();

    boot_page_erase (page);
    boot_spm_busy_wait ();      /* Wait until the memory is erased. */

    for (i=0; i<SPM_PAGESIZE; i+=2)
    {
        /* Set up little-endian word. */
        uint16_t w = *buf++;
        w += (*buf++) << 8;

        boot_page_fill (page + i, w);
    }

    boot_page_write (page);     /* Store buffer in flash page.		*/
    boot_spm_busy_wait();       /* Wait until the memory is written.*/

    /* Reenable RWW-section again. We need this if we want to jump back */
    /* to the application after bootloading. */
    boot_rww_enable ();

    /* Re-enable interrupts (if they were ever enabled). */
    SREG = sreg;
}




/**
 * Bootloader entrance
 */
int main(){

	uint8_t hex_receive_buffer[INTEL_HEX_MAX_LINE_LENGTH];

	//Relocate interrupt vector table into boot section
	uint8_t temp = MCUCR;
	MCUCR = temp | (1<<IVCE);
	MCUCR = temp | (1<<IVSEL);

#ifdef DEBUG
	uart_init(9600);
	uart_send("millionboot loaded.. \r\n");
	sei();
#endif

	//load device address from eeprom
	device_address = eeprom_read_byte (0x00);

	//configure i2c in slave mode
	TWAR = (device_address << 1)| (1<<TWGCE);
	TWCR = (1<<TWEA)|(1<<TWEN);

	//wait a sec
	_delay_ms(1000);

	//check if there was a general call firmware upgrade request on bus
	if((TWCR & (1<<TWINT)) && TWSR == TW_SR_GCALL_DATA_ACK){
		TWCR &= ~(1<<TWINT);
		if(TWDR == FIRMWARE_UPDATE_COMMAND){
			//wait for beginning of first line
			while(!(TWCR & (1<<TWINT)) && TWSR == TW_SR_DATA_ACK);


		}
	}


	//we either don't want to update or we succesfully updated aplication software
	start_application();

}
