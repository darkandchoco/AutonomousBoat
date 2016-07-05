/**
*******************************************************************************
* @file    CMotor.h
* @author  Richmond Ko
* @brief   CMotor class declaration.
*
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/
#ifndef CMOTOR_H_
#define CMOTOR_H_

/**
*******************************************************************************
* @brief CMotor class.
*
* CMotor class implements the functionality of a motor device, it can support
* forward, left, right, slow down, start up and stop.
******************************************************************************/
class CMotor
{
public:
    /**
    *****************************************************************************
    * @brief Constructor.
    ****************************************************************************/
    CMotor();

    /**
    *****************************************************************************
    * @brief Destructor.
    ****************************************************************************/
    ~CMotor();

    /**
    *****************************************************************************
    * @brief Initialize motor method.
    *
    * To initialize the motor, make sure to set the pins that are being used by
    * the motor to OUTPUT.
    ****************************************************************************/
    void initMotor();

    /**
    *****************************************************************************
    * @brief Forward method.
    *
    * The motor will turn when this method is called, the fwdPin will be asserted
    * to HIGH or analog pwm 255.
    ****************************************************************************/
    void forward();

    /**
    *****************************************************************************
    * @brief Left method.
    *
    * The fin connected to the motor IC will switch, enabling the boat to move
    * to the left.
    ****************************************************************************/
    void left();

    /**
    *****************************************************************************
    * @brief Right method.
    *
    * The fin connected to the motor IC will switch, enabling the boat to move
    * to the right.
    ****************************************************************************/
    void right();

    /**
    *****************************************************************************
    * @brief Slow method.
    *
    * The motor will receive a PWM signal of less than 255, enabling the motor
    * to slow down.
    ****************************************************************************/
    void slow();

    /**
    *****************************************************************************
    * @brief Stop method.
    *
    * The motor will receive a PWM signal of 0 or DIGITAL LOW, enabling the motor
    * to stop.
    ****************************************************************************/
    void stop();

    /**
    *****************************************************************************
    * @brief Startup method.
    *
    * The motor will receive an incremented value of a PWM signal from 0 to 255
    * with delay intervals of 50ms.
    ****************************************************************************/
    void startUp();

private:
    /**
    *****************************************************************************
    * @brief Forward pin.
    ****************************************************************************/
    static const int m_fwdPin = 9;

    /**
    *****************************************************************************
    * @brief Left pin.
    ****************************************************************************/
    static const int m_leftPin = 16;

    /**
    *****************************************************************************
    * @brief Right pin.
    ****************************************************************************/
    static const int m_rightPin = 17;

    /**
    *****************************************************************************
    * @brief Class scope counter for all loops.
    ****************************************************************************/
    int m_ctr;
};

#endif /* CMOTOR_H_ */
