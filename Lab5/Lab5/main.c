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
#include <time.h>

#define F_CPU 16000000L // 16 MHz
#include <util/delay.h>
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) -1)
#define ADC_CONVERSION 5.0/1024

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
//
// Function: print
//
// Purpose: print message byte by byte
//
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
}

////////////////////////////////////////////////////////////////////
//
// Function: adc_get_double
//
// Arguments: void
//
// Purpose: gets an ADC conversion and converts the output to a double
//
//
////////////////////////////////////////////////////////////////////
double adc_get_double(void)
{
	double adc = adc_read();
	return adc * 5 / 1024;
}

/////////////////////////
// Function: measure_multiple
// Purpose: execute multiple measurements following M command
/////////////////////////
void measure_multiple(int n, int dt){
	char out[5];
	int dt_out = dt;
	while (n > 0){
		print("t = ");
		sprintf(out,"%i",dt_out);
		print(out);
		print(" s, V = ");
		double v = adc_get_double();
		sprintf(out,"%d.%02u", (int) v, (int) ((v - (int) v ) * 100) );
		print(out);
		print("V\n");
		
		n = n - 1;
		
		int dt_buff = dt;
		while(dt_buff > 0)
		{
			_delay_ms(1000);
			dt_buff --;
		}
	}
}

////////////////////////
// Function: get_string
// Purpose: gets string from serial monitor
///////////////
const char* get_string(char input_str[]){
	char buffer[10];
	uint16_t input = usart_rx();
	
	int i = 0;
	while (input != 10){
		itoa(input, buffer, 10);
		//print(buffer);
		input_str[i] = atoi(buffer);
		i = i + 1;
		input = usart_rx();
	}
	
	return input_str;
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
		char input_str[10] = {};
		char output_str[20];
		get_string(input_str);
		
		print("input_str: ");
		print(input_str);
		print("\n");
	
		
		if (input_str[0] == 71){ // G
			double v = adc_get_double();
			//print("G");
			sprintf(output_str,"V = %d.%02u V", (int) v, (int) ((v - (int) v ) * 100) );
			print(output_str);
		}
		if (input_str[0] == 77){ // M
			measure_multiple(5,10); // dummy values right now
		}
    }
}


