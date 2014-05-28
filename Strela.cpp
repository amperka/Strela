
/* Strela.cpp
 * Strela library 
 *
 *
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include <Arduino.h>
#include <Wire.h> 

#include <Strela.h>


ISR(TIMER4_OVF_vect)          // interrupt service routine for software PWM
{
   PORTB |= _BV(5); //pin 9 HIGH

}

ISR(TIMER4_COMPD_vect)          // interrupt service routine for software PWM
{
    PORTB &= ~_BV(5); //pin 9 LOW

}

namespace {
    
    uint8_t lcdPinMode = 0;
    uint8_t ledState = 0;
    uint8_t lcdPinState = 0;
    
    int _currentMotorSpeed_1 = 0;
    int _currentMotorSpeed_2 = 0;
    //_pwmCounter = 0;
    bool _motorConnection_1 = 0;
    bool _motorConnection_2 = 0;
    
    bool strelaInit=false;
    
  
    void _setMotorSpeed_1(int speed)
    {
        _currentMotorSpeed_1 = abs(speed);
    
        //Motor 1 on PWM works on interrupts
    
        if (_currentMotorSpeed_1 >= 255)
        {
            TIMSK4 &= ~(1<<TOIE4) & ~(1<<OCIE4D); //disable interrupts on TIMER4 OVF and OCR4D
            PORTB |= _BV(5); //pin 9 HIGH
        } 
        else if (_currentMotorSpeed_1 == 0)
        {
            TIMSK4 &= ~(1<<TOIE4) & ~(1<<OCIE4D);  //disable interrupts on TIMER4 OVF and OCR4D
            PORTB &= ~_BV(5); //pin 9 LOW
        }
        else 
        {
            TCCR4C &= ~(1<<COM4D1);  //disable PWM on Pin 6
 
            //set PWM period
            OCR4D = _currentMotorSpeed_1;
            //enable interrupts on TIMER4 OVF and OCR4D
            TIMSK4 |= (1<<TOIE4)|(1<<OCIE4D);
        }
    
    }

    void _setMotorSpeed_2(int speed)
    {
        _currentMotorSpeed_2 = abs(speed);
    
        //Motor2 PWM is hardware 
    
        if (_currentMotorSpeed_2 >= 255)
        {
            //disable communicate pin 10 with OCR4 and PWM
            TCCR4A &= ~(1<<PWM4B) & ~(1<<COM4B1);
            PORTB |= _BV(6); //pin 10 HIGH
        } 
        else if (_currentMotorSpeed_2 == 0)
        {
            //disable communicate pin 10 with OCR4 and PWM
            TCCR4A &= ~(1<<PWM4B) & ~(1<<COM4B1); 
            PORTB &= ~_BV(6); //pin 10 LOW
        }
        else 
        {   
            //set PWM period
            OCR4B = _currentMotorSpeed_2;    
            TCCR4A |= (1<<PWM4B)|(1<<COM4B1); //enable communicate pin 10 with OCR4 and enable PWM
        }
    }

    void _setMotorDirections(
                bool direction_1,
                bool direction_2)
    {
        bool direction = direction_1;
        if (direction ^ _motorConnection_1)    
            PORTD |= _BV(4);  //digitalWrite(4, HIGH)
        else
            PORTD &= ~_BV(4); //digitalWrite(4, LOW)
        
        direction = direction_2;
        if (direction ^ _motorConnection_2)
            PORTE |= _BV(2);
        else
            PORTE &= ~_BV(2);
        
    }

    void _setMotorDirection(
                uint8_t mot,
                bool direction)
    {
        if (mot == M1)
        {
            if (direction ^ _motorConnection_1)
                PORTD |= _BV(4);  //digitalWrite(4, HIGH)
            else
                PORTD &= ~_BV(4); //digitalWrite(4, LOW)
        }
        else if (mot == M2)
       {    
            if (direction ^ _motorConnection_2)
                PORTE |= _BV(2);
            else
                PORTE &= ~_BV(2);
        }    
    }
   
    void twiPinMode(uint8_t address, uint8_t pinModeByte)
    {
        Wire.begin();
        Wire.beginTransmission(address);
        Wire.write(WIRE_IO_CONFIG_MODE); // i/o config mode
        Wire.write(pinModeByte); // 
        Wire.endTransmission();    
    }
    
    
    void twiWriteOut(uint8_t address, uint8_t value)
    {
        Wire.beginTransmission(address);
        Wire.write(WIRE_OUTPUT_WRITE_MODE); // output write mode
        Wire.write(value);
        Wire.endTransmission();
    }

    uint8_t twiReadIn(uint8_t address, uint8_t pin)
    {
    
        Wire.beginTransmission(address);
        Wire.write(WIRE_INPUT_READ_MODE); // input read mode
        Wire.endTransmission();
 
        uint8_t n = Wire.requestFrom(address, (uint8_t)1);
        // TODO: is reading without wait is right?
        uint8_t state = Wire.read();
    
        return (bool)(state & (1 << pin));
    }


    
}



void uPinMode(uint8_t pin, uint8_t mode)
{
    STRELA_INIT_CHECK;
        
    if (pin < NUM_DIGITAL_PINS) // standart IO
        {
            pinMode( pin, mode );
            return;
        }
        
      
    if ( pin > Q7)  //we can,t change pinMode on GPUX on addr 0x20 or more
    {
        return;
    }
    else 
    {
        if (mode != OUTPUT)
            lcdPinMode |= 1 << ( _LC(pin) );
        else
            lcdPinMode &= ~(1 << ( _LC(pin) ));       
         
            twiPinMode(LCD_TWI_ADDR, lcdPinMode);
    }                
}


void uDigitalWrite(uint8_t pin, uint8_t val)
{
    STRELA_INIT_CHECK;
        
    if (pin < NUM_DIGITAL_PINS) // standart IO
    {
        digitalWrite( pin, val );
        return;
    }
    
    if ( pin > LAST_PIN)  //NOT ON PIN
    {
        if ((pin > L1-1) && (pin < L4+1)) // it is LED?
        {
            uint8_t ld = pin - L1;
            (val) ? ledState |= 1 << (7 - ld) : ledState &= ~(1 << (7 - ld));
            twiWriteOut(GPUX_TWI_ADDR, ~ledState);   // LED is lighting on 0
        }
        return;
    }
    
    if ( pin > Q7)  //led or button
    {
        if ( pin > LS3 ) // only LED
        {
            uint8_t curPin = pin - LS0;
            (val) ? ledState |= 1 << curPin : ledState &= ~(1 << curPin);
            twiWriteOut(GPUX_TWI_ADDR, ~ledState); // LED is lighting on 0
        }
        return;
    }   
    
    else // pin>=Q0 && pin<=Q7
    {
        uint8_t curPin = _LC(pin);
        (val) ? lcdPinState |= 1 << curPin : lcdPinState &= ~(1 << curPin);
        twiWriteOut(LCD_TWI_ADDR, lcdPinState);
    }        
}

uint8_t uDigitalRead(uint8_t pin)
{
    STRELA_INIT_CHECK;
        
    if (pin < NUM_DIGITAL_PINS) // standart IO
        return digitalRead( pin );
    
    if ( pin > LAST_PIN)  //NOT ON PIN
    {
        if ((pin > S1-1) && (pin < S4+1)) // it is Button?
            return !twiReadIn(GPUX_TWI_ADDR, (3 - (pin - S1))); // !, pressed button return 0, need 1
            
        else return LOW;
    }
    
    if ( pin > Q7)  //led or button
    {
        uint8_t curPin = pin - LS0;
        return !twiReadIn(GPUX_TWI_ADDR, curPin); // !, pressed button return 0, need 1
            
    }   
    
    else // pin>=Q0 && pin<=Q7
    {
        uint8_t curPin = pin - Q0;
        return twiReadIn(LCD_TWI_ADDR, curPin);
    }        
}

void strelaInitialize()
{
   // Configure I2C i/o chip
    twiPinMode(GPUX_TWI_ADDR, WIRE_IO_CONFIGURATION);
    
    // Configure I2C LCD i/o chip to OUTPUT
    twiPinMode(LCD_TWI_ADDR, 0xff);

    
    // Configure Pin

    DDRE |= _BV(2);           //pinMode(PE2, OUTPUT);
    DDRD |= _BV(4);           //pinMode(4, OUTPUT);
    DDRB |= _BV(5) | _BV(6);  //pinMode(9,10, OUTPUT);
    
    
    // Configure software PWM4D Timer to fast PWM
    TCCR4A |= (1<<COM4B1); //enable communicate pin 10 and OCR4
    TCCR4D &= ~(1<<WGM41) & ~(1<<WGM40); 
    TC4H =0;
    
    strelaInit = true;
    
} 


void motorConnection(
            bool direction_1,
            bool direction_2)
{
    STRELA_INIT_CHECK;
        
    _motorConnection_1 = direction_1;
    _motorConnection_2 = direction_2;    
}

void motorSpeed(uint8_t motor, int speed)
{
    STRELA_INIT_CHECK;
        
    _setMotorDirection(motor, (speed < 0));  

    if (motor = M1)
        _setMotorSpeed_1(speed);
    else if (motor = M2)
        _setMotorSpeed_1(speed);
        
}

void drive(
            int motorSpeed_1,
            int motorSpeed_2)
{ 
    STRELA_INIT_CHECK;
        
    _setMotorDirections((motorSpeed_1 < 0), (motorSpeed_2 < 0));

    //Motor 1 on PWM works on interrupts
    _setMotorSpeed_1(motorSpeed_1);
    
    //Motor2 PWM is hardware 
    _setMotorSpeed_2(motorSpeed_2);

}

/*
bool Strela::buttonRead(uint8_t btn)
{
    if (btn < 4) 
        return twiReadIn(GPUX_TWI_ADDR, (3 - btn));
    else
        return uDigitalRead(btn);    
}

void Strela::ledWrite(
            uint8_t ld,
            bool state)
{
    if (ld < 4) 
    {
        (state) ? ledState |= 1 << (7 - ld) : ledState &= ~(1 << (7 - ld));
        twiWriteOut(GPUX_TWI_ADDR, ledState);  
    }
    else 
        uDigitalWrite(ld, state);
}

*/
