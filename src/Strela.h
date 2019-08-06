/*
 * Strela.h
 *
 *
 */

 
#ifndef STRELA_H
#define STRELA_H

#include <inttypes.h> 

#define GPUX_TWI_ADDR            0x20
#define LCD_TWI_ADDR             0x21

// IO-pin defines

#define BUZZER                   12

#define P1  11
#define P2  13
#define P3  5
#define P4  6

#define P5  A3
#define P6  A2
#define P7  A1
#define P8  A0

#define P9  8
#define P11 A4
#define P12 A5

#define IR_PIN P12

//I2C-expander IO pins

#define P10 37 

//LiquidCrystall
//LCx вид (для LiquidCrystal_I2C.h)
#define LC_ADDR    LCD_TWI_ADDR

#define LC4 0
#define LC5 1
#define LC6 2
#define LC7 3
#define LCEN 4
#define LCRW 5
#define LCRS 6

//Qx вид (для uSerialWrite())
#define Q0 30 //NUM_DIGITAL_PINS 
#define Q1 31
#define Q2 32
#define Q3 33
#define Q4 34
#define Q5 35
#define Q6 36
#define Q7 P10

//LED and Buttons
#define LS0 38
#define LS1 39
#define LS2 40
#define LS3 41

#define LS4 42
#define LS5 43
#define LS6 44
#define LS7 45

//Buttons
#define S1 50
#define S2 51
#define S3 52
#define S4 53
//Leds
#define L1 60
#define L2 61
#define L3 62
#define L4 63

//Motors
#define M1 0
#define M2 1

void strelaInitialize();

void uPinMode(uint8_t pin, uint8_t mode);

void uDigitalWrite(uint8_t pin, uint8_t val);

uint8_t uDigitalRead(uint8_t pin);

int uAnalogRead(uint8_t pin);

void uAnalogWrite(uint8_t pin, int val);

void drive(
    int motorSpeed_1,
    int motorSpeed_2);
    
void motorSpeed(uint8_t motor, int speed);

        
void motorConnection(
    bool direction_1,
    bool direction_2);

void stepperMotor(int stepsToMove, int stepDelay);


#endif
