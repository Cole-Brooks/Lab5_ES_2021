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

//////////////
// function: delay
// Purpose: delays a certain number of seconds
///////////////////
void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;
	
	// Storing start time
	clock_t start_time = clock();
	
	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)
	;
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
		dt_out = dt_out + dt;
		//delay(dt);
		
	}
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
		if(input == 'G')  // user entered G
		{
			double adc_val = adc_get_double();
			
			// do some c nonsense to convert float to string
			char out[8];
			sprintf(out,"%d.%02u", (int) adc_val, (int) ((adc_val - (int) adc_val ) * 100) );
			
			print("V = ");
			print(out);
			print(" V\n");
		}
		if(input == 'M')  // user entered M, will also print ascii of everything following M
		{
			int num_temp = 0; // temporary storage of n and dt
			int n = 0;
			int dt = 0;
			int gotn = 0;	
			itoa(input, output_str,10);
			print(output_str);
			print("\n");
			while (input != 10){  // not at end of string
				input = usart_rx();
				if (input == 44){ // found comma
					input = usart_rx();
					while (input != 44){
						input = usart_rx();
						itoa(input, output_str,10);
						num_temp += atoi(output_str);
					}
					if (gotn == 0){
						n = num_temp;
						gotn = 1;
					}
					else if (gotn == 1){
						dt = num_temp;
					}
					
				}
			}


			print("\n\n");
			measure_multiple(n,dt);
		}
		if(input == 4) // EOT
		{
			// do nothing (prevents double print at end for some reason)
		}
    }
}


