/**
*******************************************************************************
* @file    types.h
* @author  Richmond Ko
* @brief   Contains various constants for different devices.
*
* Constants used by the motor, sensors and ZigBee along with other misc. devices
* are found here.
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 14-May-2012 Added namespaces for MOTOR, ZIGBEE and SENSOR,
*                    removed tSensorType,
*                    added sensor identifiers.
*   <li> 15-May-2012 Added times and delay constants for ZigBee class.
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/
#ifndef TYPES_H
#define TYPES_H

//NOTE: namespaces are just a way to group constants and variables.
// To access a namespace member, ie: to access maxSpeed, use MOTOR::maxSpeed.

//motor constants definitions
namespace MOTOR
{
    static const int maxSpeed = 255;
    static const int slowSpeed = 210;
    static const int stopSpeed = 0;

    //enums
    typedef enum tMovementMode
    {
        tForward,
        tLeft,
        tRight,
        tSlow,
        tStop,
        tStartUp
    }tMovementMode;
}

//ZigBee constants definitions
namespace ZIGBEE
{
    //change here the needed baud rate
    static const int baudRate = 9600;
    //change here the needed 64-bit destination address
    static const uint32_t destinationSH = 0x0013A200;
    static const uint32_t destinationSL = 0x407012CC;
}

//Sensor identifier constants
namespace SENSOR
{
    //change here the user specified sensor identifers
    static const uint8_t temperatureId = 0xFF;
    static const uint8_t phId = 0xFE;
    static const uint8_t salinityId = 0xFD;
    static const uint8_t compassId = 0xFC;
    static const double salinityBaudRate = 38400;
    static const int compassBaudRate = 9600;
}

#endif /* TYPES_H */
