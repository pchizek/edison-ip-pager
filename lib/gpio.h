
class gpio{

public:

    // Constructor
    gpio(int pin_num[]);

    // Set pinmode
    int set_mode(int pin_num[], bool direction[]);

    // Write
    int set_level(int pin_num[], bool level[]);

    //  Read
    int get_level(int pin_num[], bool level[]);

};