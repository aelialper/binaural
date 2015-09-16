
// MCP434X & MCP436X
// http://www.farnell.com/datasheets/614162.pdf

#ifndef Mcp43XX_h
#define Mcp43XX_h

// #include "spi.h"
#include <stdint.h>


/**
 * This class controls Microchip's MCP43XX range of digital potentiometers.
 * http://ww1.microchip.com/downloads/en/DeviceDoc/22060b.pdf
 *
 * The supported chips are configured as follows:
 *
 * MCP 4 X Y Z
 *       | | |
 *       | | +- Z = Wiper Configuration (1 = Potentiometer, 2 = Rheostat).
 *       | |
 *       | +--- Y = Memory Type (3 = 7-bit RAM, 4 = 7-bit EEPROM, 5 = 8-bit RAM, 6 = 8-bit EEPROM).
 *       |
 *       +----- X = Number of pots (1 or 2).
 *
 * Note that the MCP41X1 chips multiplex the SDI and SDO on a single pin.
 * To use these chips with a standard SPI interface as on the Arduino you will need to 
 * connect the SDI/SDO pin on the pot to the Arduino's MISO pin, then bridge the MISO pin to the MOSI pin
 * with a resistor (3k9 resistor seems to work well).
 *
 * This class has only been tested with the MCP4151.
 */
class MCP43XX
{
  public:
    // NOTE this is used to map to the Adress enumeration. 
    // Wiper 3 & 4 are actually at address 6 & 7
    enum Pot 
    {
      pot_0 = 0x00,
      pot_1 = 0x01,
      pot_2 = 0x02,
      pot_3 = 0x03
    };
    
    enum MemorySize
    {
      memory_7bit = 0x7F,
      memory_8bit = 0xFF
    };
    
    enum WiperConfiguration
    {
      rheostat = 0x0,
      potentiometer = 0x1
    };

    /**
     * select_pin  - The pin to use for the SPI slave select signal, defaults to the main slave select pin on your Arduino.
     * pot         - For the 2-pot variants (MCP42XX), the potentiometer to control. Must be pot_0 for MCP41XX chips.
     * memory_size - memory_7bit for MCP4X3X and MCP4X4X, memory_8bit for MCP4X5X and MCP4X6X.
     */
    MCP4XXX(byte port_SS, byte pin_SS, Pot pot = pot_0, MemorySize memory_size = memory_8bit, WiperConfiguration config = potentiometer);
    
    /**
     * Retrieve the maximum value allowed for the wiper position.
     *
     * The maximum value depends on the device's memory type and wiper configuration.
     *
     * 7-bit devices have a maximum value of 127 for rheostats and 128 for potentiometers.
     * 8-bit devices have a maximum value of 255 for rheostats and 256 for potentiometers.
     *
     * The higher value on potentiometers (MCP4XX1) facilitates direct connection of the wiper to the "A" terminal (also known as "full-scale").
     * Confusingly the rheostat devices (MCP4XX2) have only a wiper pin and "B" terminal pin, (not "A").
     */
    word max_value(void) const;

    bool increment(void); // Increases the wiper position, returns success.
    bool decrement(void); // Decreases the wiper position, returns success.
    bool set(word value); // Sets the wiper position between 0 and max_value() , returns success.
    bool get(word& value) const; // value is assigned the wiper position if success, returns success.
    word get(void) const; // Returns wiper position, returns 0xFFFF is failure.

    byte highByte(uint16_t uint16);
    byte lowByte(uint16_t uint16);

  private:

    const byte port_SS; // slave select port
    const byte pin_SS;  // slave select pin

    const static byte address_mask       = B11110000;
    const static byte command_mask       = B00001100;
    const static byte cmderr_mask        = B00000010;
    const static byte data_mask          = B00000001;
    const static word data_mask_word     = 0x01FF;

    enum Address
    {
        address_pot0_wiper = B0000,
        address_pot1_wiper = B0001,
        address_pot2_wiper = B0110,
        address_pot3_wiper = B0111,
        address_tcon       = B0100,
        address_status     = B0101
    };
    
    enum Command
    {
        command_write      = B00,
        command_read       = B11,
        command_increment  = B01,
        command_decrement  = B10
    };

    /**
     * Select this device for SPI communication
     *
     * Configures SPI for communication with MCP devices, and sends slave-select pin LOW.
     */
    void select(void) const;

    /**
     * Cease SPI communications with this device.
     *
     * Sends the slave-select pin HIGH.
     */
    void deselect(void) const;

    /**
     * Build an 8-bit command.
     */
    byte build_command(Address address, Command command) const;

    /**
     * Build a 16-bit command.
     */
    word build_command(Address address, Command command, word data) const;

    /**
     * Transfer an 8-bit command.
     */    
    bool transfer(Address address, Command command) const;
    
    /**
     * Transfer a 16-bit command.
     *
     * Result is TRUE if address/command combination is valid; otherwise, FALSE.
     */
    bool transfer(Address address, Command command, word data) const;
    
    /**
     * Transfer a 16-bit command, and read the response.
     *
     * Result is TRUE if address/command combination is valid; otherwise, FALSE.
     *
     * The result argument is populated with the 9-bit response only if return value is TRUE.
     */
    bool transfer(Address address, Command command, word data, word& result) const;

    byte m_select_pin;
    Address m_pot_address;
    word m_max_value;


};

#endif // Mcp43XX_h

        // // Not implemented
        // // Connect / disconnect A,B or W terminals
        // void connect(bool terminal_a, bool wiper, bool terminal_b);
        // void disconnect(bool shutdown);
        // bool hw_shutdown();