/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "spi.h"
#include "digipot_c.h"
#include "optimized.h"

int main(void)
{
    
    spi_init();
    
    /* insert your hardware initialization here */
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
