/*
 * Lab5.c
 *
 * Created: 4/8/2021 10:24:57 AM
 * Author : Cole Brooks
 *			Thomas Butler
 *
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 16000000L // 16 MHz
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) -1)


////////////////////////////////////////////////////////////////////
//
// Function: usart_tx
//
// Arguments: uint8_t data:
//
// Purpose: Sends data to serial monitor
//
////////////////////////////////////////////////////////////////////
void usart_tx(uint8_t data)
{
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = data;
}

///////////////////////////////
// Function: print
// Purpose: print message byte by byte
///////////////////////////////
void print(char msg[]){
	int i;
	for (i = 0; i < strlen(msg); i++) {
		usart_tx(msg[i]);
	}
}

////////////////////////////////////////////////////////////////////
//
// Function: usart_init
//
// Arguments: Void 
// 
// Purpose: set baud rate, enable rx and tx, and set
//			frame formate: 8 data, 2 stop bit 
//			
////////////////////////////////////////////////////////////////////
void usart_init(void)
{
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

////////////////////////////////////////////////////////////////////
//
// Function: adc_read
//
// Arguments: void
//
// Purpose: reads from ADC and returns value
//
////////////////////////////////////////////////////////////////////
uint16_t adc_read(void)
{
	// Trigger a voltage read
	ADCSRA |= (1<<ADSC);
	
	
	while(ADSC == 1)
	{
		// Conversion Processing
	}
	
	return ADCW;
	
}


////////////////////////////////////////////////////////////////////
//
// Function: usart_rx
//
// Arguments: Void
//
// Purpose: Receives data from serial monitor
//
////////////////////////////////////////////////////////////////////
uint16_t usart_rx(void)
{
	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};
	// Return received data
	return UDR0;

}

////////////////////////////////////////////////////////////////////
//
// Function: adc_init
//
// Arguments: void
//
// Purpose: initializes the analog to digital converter
//
////////////////////////////////////////////////////////////////////
void adc_init(void)
{
	// Set ADC voltage reference and input channel (Port A1)
	ADMUX |= (1<<REFS0) | (1<<MUX0);  //AVcc as voltage reference
	// Set up the status register
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	sei(); // enable global interrupts
}

////////////////////////////////////////////////////////////////////
// Function: main
//
// Purpose: Drives the program
//
////////////////////////////////////////////////////////////////////
int main(void)
{	
	// Initializations
	usart_init();    
	adc_init();
	
	// main loop
    while (1) 
    {
		uint16_t input = usart_rx();
		char output_str[5];
		if(input == 71)  // user entered G
		{
			uint16_t adc_val = adc_read();
			//float adc_double = (adc_val * 5.0)/1023;
			//char adc_str[5];
			// convert to string and print
			itoa(adc_val, output_str,10);
			//char output[10];
			//sprintf(output, "V = %.3f", adc_double);
			print("V = ");
			print(output_str);
			print(" V\n");
		}
		if(input == 77)  // user entered M, will also print ascii of everything following M
		{	
			itoa(input, output_str,10);
			print(output_str);
			print("\n");
			input = usart_rx();
			while (input != 10){  // comma found, figure out how to get integer following comma
				itoa(input, output_str,10);
				print(output_str);
				print("\n");
				input = usart_rx();
			}
			print("\n\n");
		}
		if(input == 4) // EOT
		{
			// do nothing (prevents double print at end for some reason)
		}
    }
}


