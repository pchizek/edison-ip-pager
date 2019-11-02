
// Defines for instructions


class lcd1602a{

public:

    // constructor
    lcd1602a(int gpio_rs, int gpio_rw, int gpio_en, int gpio_bus[8]);


    int clear();

    int shift();

    void write(bool instr, uint8_t data);
    uint8_t read(bool instr);
    
    void write4(bool instr, uint8_t data);
    uint8_t read4(bool instr);

    void write8(bool instr, uint8_t data);
    uint8_t read8(bool instr);

    void read_ram(bool cg_dd, uint8_t address, uint8_t *buf[]);
    void write_ram(bool cg_dd, uint8_t address, uint8_t *buf[]);

    void clear();

    /*
    void start();
    void stop();
    */

    // Cursor

    // Write data to RAM, return pointer

    // Read data from ram

    // Clear ram, decrement pointer

private:

    int rs_pin;
    int rw_pin;
    int en_pin;
    int bus_pins[8];

    bool bus_out;

}



