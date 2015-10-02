//
//  digipot_c.c
//  
//
//  Created by Ali on 10/2/15.
//
//

#include "digipot_c.h"


const static byte port_SS =; // slave select port
const static byte pin_SS =;  // slave select pin


const static byte address_mask       = B11110000;
const static byte command_mask       = B00001100;
const static byte cmderr_mask        = B00000010;
const static byte data_mask          = B00000001;
const static word data_mask_word     = 0x01FF;

// Adress bytes
const static byte pot0       = B0000;
const static byte pot1       = B0001;
const static byte pot2       = B0110;
const static byte pot3       = B0111;
const static byte tcon       = B0100;
const static byte status     = B0101;

/* Only using increment and decrement and turn off commands
const static byte command_write      = B00;
const static byte command_read       = B11;
*/
const static byte command_increment  = B01;
const static byte command_decrement  = B10;

const static byte m_max_value        = 0x00FF; // Maximum potentiometer value


static void toggleSS(void) const
{
    /* Toggle slave select pin for digipot*/
    port_SS ^= _BV(pin_SS);
}

static byte highByte(uint16_t uint16)
{
    return (byte)(uint16>>8);
}

static byte lowByte(uint16_t uint16)
{
    return (byte)(uint16 & 0x00FF);
}

bool digipot_increment(void)
{
    return transfer(m_pot_address, command_increment);
}

bool digipot_decrement(void)
{
    return transfer(m_pot_address, command_decrement);
}

static byte build_command(Address address, Command command) const
{
    return ((address << 4) & address_mask)
    | ((command << 2) & command_mask)
    | cmderr_mask;
}

bool digipot_transfer_command(Address address, Command command) const
{
    // Transfers increment or decrement command byte only
    toggleSS();
    byte result = spi_transfer_sync(build_command(address, command));
    toggleSS();
    return result & cmderr_mask;
}

/* Increment and decrement and turn off are all that's necessary for the current edition
 
bool digipot_set(word value)
{
byte tmp = value < m_max_value ? value : m_max_value; // If data is larger than m_max_value then set that
byte valueToSet = tmp > 0 ? tmp : 0; // Check to see if it is also larger than zero
return transfer(m_pot_address, command_write, valueToSet);
}
 

bool digipot_get(word& value) const
{
return transfer(m_pot_address, command_read, data_mask_word, value);
}

 
word digipot_get(void) const
{
word result = 0xFFFF;
get(result);
return result;
}
 
static word build_command_data(Address address, Command command, word data) const
{
    return (build_command(address, command) << 8)
    | (data & data_mask_word);
}

bool digipot_transfer_data(Address address, Command command, word data) const
{
    // Transfers command byte plus data byte
    toggleSS();
    word cmd = build_command_data(address, command, data);
    bool valid = spi_transfer_sync(highByte(cmd)) & cmderr_mask;
    
    if (valid)
    {
        SPI.transfer(lowByte(cmd));
    }
    
    toggleSS();
    return valid;
}

bool digipot_transfer_data_err(Address address, Command command, word data, word& result) const
{
    toggleSS();
    
    word cmd = build_command_data(address, command, data);
    byte high_byte = spi_transfer_sync(highByte(cmd));
    bool valid = high_byte & cmderr_mask;
    
    if (valid)
    {
        result = ((high_byte & data_mask) << 8)
        | spi_transfer_sync(lowByte(cmd));
    }
    
    toggleSS();
    return valid;
}
*/


