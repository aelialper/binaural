//
//  digipot_c.h
//  
//
//  Created by Ali on 10/2/15.
//
//

#ifndef ____digipot_c__
#define ____digipot_c__

#include <stdio.h>
#include <avr/io.h>


extern static void toggleSS(void) const;
extern static byte highByte(uint16_t uint16);
extern static byte lowByte(uint16_t uint16);
extern bool digipot_increment(void);
extern bool digipot_decrement(void);
extern bool digipot_set(word value);
extern bool digipot_get(word& value) const;
extern static byte build_command(Address address, Command command) const;

/* At this point increment and decrement are enough for me
extern static word build_command_data(Address address, Command command, word data) const;
extern bool digipot_transfer_command(Address address, Command command) const;
extern bool digipot_transfer_data(Address address, Command command, word data) const;
extern bool digipot_transfer_data_err(Address address, Command command, word data, word& result) const;
 */
#endif /* defined(____digipot_c__) */
