/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>

#define DDR_DIGIPOT DDRB // PORTB on binaural board
#define SS_DIGIPOT DDB2 // PORTB.2 on binural board

void setup()
{
	spi_init( );

	// Set SS pin b.5 as output for digipot
	DDR_DIGIPOT &= ~(1 << SS_DIGIPOT); // clear only the address that we will be using for SS
	DDR_DIGIPOT |= 1 << SS_DIGIPOT;

} 

void main()
{
    /* insert your hardware initialization here */
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */

    // unit tests
    MCP43XX.toggleSS(); // NOTE maybe you need reinterpret_cast<T> like explained in Real Time C++
	MCP43XX.toggleSS(); // test twice :)

	MCP43XX.increment();
	MCP43XX.decrement(); 
	MCP43XX.set(); 
	MCP43XX.get(); 

    // usage of SPI
    MCP43XX.transfer();
    //


}


