/**
*******************************************************************************
* @file    CMotor.cpp
* @author  Richmond Ko
* @brief   CMotor class implementation.
*
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 14-May-2012 Applied MOTOR namespace usage.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/

//----------------------- INCLUDED FILES --------------------------------------
#include "CMotor.h"
#include <Arduino.h>    //for Arduino core library APIs
#include "types.h"

//----------------------- METHODS OF CLASS CMotor ------------------------------
CMotor::CMotor()
{

}

CMotor::~CMotor()
{

}

//-----------------------------------------------------------------------------
// Description : Initialization of motor.
//
void CMotor::initMotor()
{
    pinMode(m_fwdPin, OUTPUT);
    pinMode(m_leftPin, OUTPUT);
    pinMode(m_rightPin, OUTPUT);
}

//-----------------------------------------------------------------------------
// Description : To move forward at max speed, assert a PWM signal of 255.
//
void CMotor::forward()
{
    analogWrite(m_fwdPin, MOTOR::maxSpeed);
}

void CMotor::left()
{
    digitalWrite(m_leftPin, HIGH);
}

void CMotor::right()
{
    digitalWrite(m_rightPin, HIGH);
}

//-----------------------------------------------------------------------------
// Description : To reduce the speed of the motor, assert a PWM signal of less
//               than 255.
//
void CMotor::slow()
{
    analogWrite(m_fwdPin, MOTOR::slowSpeed);
}

//-----------------------------------------------------------------------------
// Description : To stop the movement of the motor, assert a PWM signal of 0.
//
void CMotor::stop()
{
    digitalWrite(m_leftPin, LOW);
    digitalWrite(m_rightPin, LOW);
    analogWrite(m_fwdPin, MOTOR::stopSpeed);
}

//-----------------------------------------------------------------------------
// Description : Start the motor slowly by incrementing PWM
//
void CMotor::startUp()
{
    for(m_ctr = 0; m_ctr <= MOTOR::maxSpeed; m_ctr++)
    {
        analogWrite(m_fwdPin, m_ctr);
        delay(50);
    }
}
