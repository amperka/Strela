// ---------------------------------------------------------------------------
// Created by Francisco Malpartida on 20/08/11.
// Copyright 2011 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// Thread Safe: No
// Extendable: Yes
//
// @file I2CIO.h
// This file implements a basic IO library using the PCF8574 I2C IO Expander
// chip.
// 
// @brief 
// Implement a basic IO library to drive the PCF8574* I2C IO Expander ASIC.
// The library implements basic IO general methods to configure IO pin direction
// read and write uint8_t operations and basic pin level routines to set or read
// a particular IO port.
//
//
// @version API 1.0.0
//
// @author F. Malpartida - fmalpartida@gmail.com
// ---------------------------------------------------------------------------
#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <inttypes.h>

#include <../Wire/Wire.h>
#include "I2CIO.h"

// CLASS VARIABLES
// ---------------------------------------------------------------------------


// CONSTRUCTOR
// ---------------------------------------------------------------------------
I2CIO::I2CIO ( )
{
   _i2cAddr     = 0x0;
   _dirMask     = 0xFF;    // mark all as INPUTs
   _shadow      = 0x0;     // no values set
   _initialised = false;
}

// PUBLIC METHODS
// ---------------------------------------------------------------------------

//
// begin
int I2CIO::begin (  uint8_t i2cAddr )
{
   _i2cAddr = i2cAddr;
   
   Wire.begin ( );
      
   _initialised = true;

   return ( _initialised );
}


//
// portMode
void I2CIO::portMode ( uint8_t dir )
{
    if (dir == OUTPUT) {
        Wire.beginTransmission(_i2cAddr);
        Wire.write(0x03); // i/o config mode
        Wire.write(0x00); // all outputs
        Wire.endTransmission();
    }
   
}

//
// write
int I2CIO::write ( uint8_t value )
{
    Wire.beginTransmission(_i2cAddr);
    Wire.write(0x01); // write output
    Wire.write(value);
    Wire.endTransmission();

    return true;
}
