
#include <stdint.h>
#include "digipot.h"
#include "spi.h"

MCP43XX::MCP43XX( Pot pot, MemorySize memory_size, WiperConfiguration config )
  : port_SS( port_SS ),
  	pin_SS( pin_SS ),
    pot_address(static_cast<Address>(pot)), // Cast the pot number to volatile wiper address
    max_value(memory_size + config) // Potentiometer configurations allow "memory_size + 1" values, for setting the "full-scale" wiper position.
{
 // Main program initializes the SPI,
}

byte MCP43XX::highByte(uint16_t uint16)
{
  return (byte)(uint16>>8);
}

byte MCP43XX::lowByte(uint16_t uint16)
{
  return (byte)(uint16 & 0x00FF);
}

word MCP43XX::max_value(void) const
{
  return m_max_value;
}

bool MCP43XX::increment(void)
{
  return transfer(m_pot_address, command_increment);
}

bool MCP43XX::decrement(void)
{
  return transfer(m_pot_address, command_decrement);
}

bool MCP43XX::set(word value)
{
  return transfer(m_pot_address, command_write, min(value, m_max_value));
}

bool MCP43XX::get(word& value) const
{
  return transfer(m_pot_address, command_read, data_mask_word, value);
}

word MCP43XX::get(void) const
{
  word result = 0xFFFF;
  get(result);
  return result;
}

// // toggleSS() replaces select() and deselect() because for now 
// // there is no need to change SPI parameters for different slaves.
// void MCP43XX::select(void) const
// {
//   SPI.setBitOrder(MSBFIRST);
//   SPI.setDataMode(SPI_MODE0);
//   SPI.setClockDivider(SPI_CLOCK_DIV128);
//   digitalWrite(m_select_pin, LOW);
// }

// void MCP43XX::deselect(void) const
// {
//   digitalWrite(m_select_pin, HIGH);
// }
void MCP43XX::toggleSS(void) const
{
	// Replaces select() and deselect()
	port_SS ^= 0x01U << pin_SS;
}

byte MCP43XX::build_command(Address address, Command command) const
{
  return ((address << 4) & address_mask)
       | ((command << 2) & command_mask)
       | cmderr_mask;
}

word MCP43XX::build_command(Address address, Command command, word data) const
{
  return (build_command(address, command) << 8)
       | (data & data_mask_word);
}

bool MCP43XX::transfer(Address address, Command command) const
{
  // Transfers increment or decrement command byte only
  toggleSS();
  byte result = spi_transfer_sync(build_command(address, command));
  toggleSS();
  return result & cmderr_mask;
}

bool MCP43XX::transfer(Address address, Command command, word data) const
{
  // Transfers command byte plus data byte
  toggleSS();
  word cmd = build_command(address, command, data);
  bool valid = spi_transfer_sync(highByte(cmd)) & cmderr_mask;

  if (valid)
  {
    SPI.transfer(lowByte(cmd));
  }

  toggleSS();
  return valid;
}
    
bool MCP43XX::transfer(Address address, Command command, word data, word& result) const
{
  toggleSS();

  word cmd = build_command(address, command, data);
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

} // end namespace arduino
} // end namespace icecave

