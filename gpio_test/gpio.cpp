
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "gpio.h"

#define ERR_FILE -1
#define ERR_WRITE -2
#define ERR_EXPORT -3

#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define BUFFER_SZ 50

// Declare 
gpio::gpio(int pin_num[]){

    //Path Variables
    char gpio_exp_path[BUFFER_SZ];
    char gpio_dir_path[BUFFER_SZ];
    char gpio_val_path[BUFFER_SZ];

    // Initialize files and set their paths
    int elements = (sizeof(pin_num)/sizeof(*pin_num));
    for (int i=0;i<elements;i++){
    
        // Create filenames for all gpios declared
        snprintf(gpio_exp_path,BUFFER_SZ,GPIO_EXPORT_PATH,pin_num[i]);
        snprintf(gpio_dir_path,BUFFER_SZ,GPIO_DIRECTION_PATH,pin_num[i]);
        snprintf(gpio_val_path,BUFFER_SZ,GPIO_VALUE_PATH,pin_num[i]);
        
        // Check for direction file
        int fd_dir = open(gpio_dir_path,O_WRONLY);

        // If it can't be opened, export
        if (fd_dir < 0){

            // Set export paths?
            int fd_export = open(gpio_exp_path,O_WRONLY);

            if (fd_export < 0){
                perror("Can't open export file.");
                //return ERR_FILE;
            }

            char buf[15];
            sprintf(buf,"%d",pin_num[i]);

            int err = write(fd_export, buf, sizeof(buf));

            close(fd_dir);

            // Try to reopen exported direction file
            fd_dir = open(gpio_dir_path,O_WRONLY);
            int fd_val = open(gpio_val_path,O_WRONLY);

            if (fd_dir < 0){
                perror("Can't open exported GPIO direction file.");
                //return ERR_EXPORT;
            }
            
            if (fd_val < 0){
                perror("Can't open exported GPIO direction file.");
                //return ERR_EXPORT;
            }

            // Close both files
            close(fd_dir);
            close(fd_val);

        }




    }
}

int gpio::set_mode(int pin_num[], bool direction[]){
    
    // Define variable to use as the path to the direction file
    char gpio_dir_path[BUFFER_SZ];

    // Initialize files and set their paths
    int elements = (sizeof(pin_num)/sizeof(*pin_num));
    for (int i=0;i<elements;i++){
        
        // Define file name
        snprintf(gpio_dir_path,BUFFER_SZ,GPIO_DIRECTION_PATH,pin_num[i]);

        // Open direction file
        int fd_dir = open(gpio_dir_path,O_WRONLY);

        if (fd_dir < 1){
            perror("Can't open direction file.");
            return ERR_FILE;
        }

        // Set direction
        int err;
        if (direction[i]){
            err = write(fd_dir,"out",sizeof("out"));
        }
        else{
            err = write(fd_dir,"in",sizeof("in"));
        }

        // Close the direction file for this pin
        close(fd_dir);

        if (err){
            return ERR_WRITE;
        }

    }

    return 0;

}

int gpio::set_level(int pin_num[], bool level[]){
  
    // Define variable to use as the path to the direction file
    char gpio_val_path[BUFFER_SZ];
    char buf[2];

    // Initialize files and set their paths
    int elements = (sizeof(pin_num)/sizeof(*pin_num));
    for (int i=0;i<elements;i++){
        
        // Define file name
        snprintf(gpio_val_path,BUFFER_SZ,GPIO_VALUE_PATH,pin_num[i]);

        // Open direction file
        int fd_val = open(gpio_val_path,O_WRONLY);

        if (fd_val < 0){
            perror("Can't open value file.");
        }

        // Put into buffer
        sprintf(buf,"%d",(int)level[i]);

        // Set level
        int err = write(fd_val,buf,sizeof(buf));

        // Close the direction file for this pin
        close(fd_val);

        if (err){
            return ERR_WRITE;
        }

    }

    return 0;
}

int gpio::get_level(int pin_num[], bool level[]){
     
    // Define variable to use as the path to the direction file
    char gpio_val_path[BUFFER_SZ];
    char buf[2];

    // Initialize files and set their paths
    int elements = (sizeof(pin_num)/sizeof(*pin_num));
    for (int i=0;i<elements;i++){
        
        // Define file name
        snprintf(gpio_val_path,BUFFER_SZ,GPIO_VALUE_PATH,pin_num[i]);

        // Open direction file
        int fd_val = open(gpio_val_path,O_WRONLY);


        if (fd_val < 0){
            perror("Can't open value file.");
        }

        // Put into buffer
        read(fd_val,buf,sizeof(buf));

        if (buf == "1"){
            level[i] = true;
        }
        else{
            level[i] = false;
        }

        // Close the direction file for this pin
        close(fd_val);

    }

    return 0;

}