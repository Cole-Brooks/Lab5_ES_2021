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

ISR(ADC_vect)
{
}

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
	PRR |= (0<<PRADC);
	ADCSRA |= (1<<ADSC);
	
	
	while(ADSC == 1)
	{
		// Conversion Processing
	}
	
	return ADC;
	
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
void usart_rx(void)
{
	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};
	// Return received data
	if(UDR0 == 71)  // user entered G
	{
		float adc_val = adc_read();
		char adc_str[5];
		// convert to string and print
		sprintf(adc_str, "V = %.3f V\n", adc_val);
		print(adc_str);
	}
	if(UDR0 == 77)  // user entered M
	{
		print("t = \n");	
	}
	if(UDR0 == 4) // EOT
	{
		// do nothing (prevents double print at end for some reason)
	}

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
	ADMUX |= (1<<REFS0) | (1<<MUX0);  //AVcc as voltage reference, input channel ADC1
	// Set up the status register
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
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
		usart_rx();
    }
}


