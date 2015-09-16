
## Notes



	* SPI mode for the digipot is in the spi_init() function among other things. CPOL & CPHA
	
	* You can disconnect the terminals of the ersistor network individually. TCON register. Might be good for energy saving

	* Take care not to engage WiperLock technology or you can't write shit


## Functions to write:

	* initialise() the TCON register
	* shut_down()
	* transit_byte( byte, wiper)
	* read_value( wiper ) or read_state( error command bit CMDERR )
	* make the SPI automatic (ISR?) so the CPU doesn't have to worry about it when it's doing stuff


## Notes
	
	* Right now the spi function is called 4 times, you can call it with a 4 byte array I'm sure
	* slave_select variable is an address, check to see how that behaves, especially in the toggleSS() or toggle_ss() function. This assumes it's PORTB, also. Maybe remove it altogether.
	* Volatile wipers 3 & 4 reside at addresses 6 & 7, while 1 & 2 are at 1 & 2. Make sure your two enumerations work.