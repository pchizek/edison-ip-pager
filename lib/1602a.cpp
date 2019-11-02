
#include "1602a.h"

lcd1602a(int gpio_rs, int gpio_rw, int gpio_en, int gpio_bus[8])
    : rs_pin(gpio_rs),rw_pin(gpio_rw),en_pin(gpio_en),bus_pins(gpio_bus)
{

    uint8_t pins_declared;

    // Find which pins have been declared
    for (uint8_t i=0;i<8;i++){
        
        pins_declared |= ((if(bus_pins[i]))<<i);

    }

    // Alias as write function
    if (pins_declared & 0xF0){
        /*
        &write = write8;
        &read = read8;
        */
    }

    else {
        &write = write4;
        &read = read4;
    }

}

/*
 * write4: 
 * Writes either an instruction or data to the LCD, using only 4 pins.
 */
void lcd1602a::write4(bool instr, uint8_t data){

    if (!bus_out){

    }

    // drop E

    // wait 1 us

    // RS set/cleared if instruction or data

    // RW goes low

    // wait tAS

    // E goes high for 140 ns

    // E goes low at some point?

    // Send data on 4 lines

    // Send second nibble of data

}

/*
 * read4: 
 * Read data or busy flags.
 */
uint8_t lcd1602a::read4(bool instr, uint8_t data){

    // drop E

    // wait 1 us

    // RS set/cleared if instruction or data

    // RW goes low

    // wait tAS

    // E goes high for 140 ns

    // E goes low at some point?

    // Send data on 4 lines

    // Send second nibble of data

    // Shift to be input

    // poll first nibble

    // wait

    // Poll second nibble


}

void lcd1602::read_ram(bool cg_dd, uint8_t address, uint8_t *buf[]){

    // Set start address of read 
    address |= (1<<(6+cg_dd));

    // Write start address
    this->write(false,address);

    // Read data from concurrent addresses into buffer
    for (int i=0;i<sizeof(*buf);i++){
        *buf[i]=this->read(true);
    }

}

void lcd1602::write_ram(bool cg_dd, uint8_t address, uint8_t *buf[]){
    
    // Set start address of read 
    address |= (1<<(6+cg_dd));

    // Write start address
    this->write(false,address);

    // Read data from concurrent addresses into buffer
    for (int i=0;i<sizeof(*buf);i++){
        this->write(true,*buf[i]);
    }
}
