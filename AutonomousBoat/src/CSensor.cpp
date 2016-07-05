/**
*******************************************************************************
* @file    CSensor.cpp
* @author  Richmond Ko
* @brief   CSensor class implementation.
*
* Revision History:
* <ul>
*   <li> 14-May-2012 File created.
*   <li> 17-May-2012 Implemented the new methods.
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/

//----------------------- INCLUDED FILES --------------------------------------
#include "CSensor.h"
#include "types.h"


//----------------------- METHODS OF CLASS CSensor ----------------------------
CSensor::CSensor()
{

}

CSensor::~CSensor()
{

}

//-----------------------------------------------------------------------------
// Description : Initialize the sensors.
//
void CSensor::initSensors()
{
    analogReference(DEFAULT);

    m_gpsSerial = new SoftwareSerial(m_gpsSensorRXPin, m_gpsSensorTXPin);
    m_gpsSerial->begin(ZIGBEE::baudRate);

    m_salinitySerial = new SoftwareSerial(m_salinitySensorRXPin,m_salinitySensorTXPin);
    m_salinitySerial->begin(SENSOR::salinityBaudRate);

    m_compassSerial = new SoftwareSerial(m_compassSensorRXPin,m_compassSensorTXPin);
    m_compassSerial->begin(SENSOR::compassBaudRate);
}

//-----------------------------------------------------------------------------
// Description : This method reads 20 samples of analog signal from the
//               temperature sensor, averages it and returns it.
//
void CSensor::readTemperature(int& temperatureValue)
{
    temperatureValue = analogRead(m_temperatureSensorPin);
}

//-----------------------------------------------------------------------------
// Description : This method reads 20 samples of analog signal from the
//               ph sensor, averages it and returns it.
//
void CSensor::readPh(int& phValue)
{
    phValue = analogRead(m_phSensorPin);
}

//-----------------------------------------------------------------------------
// Description : This method sends a command to the EC circuit, get the reply
//               string of the EC circuit which is the sensors value, returns
//               this string.
//
void CSensor::readSalinity(String& salinityString)
{
       m_temp = 0;
       m_salCtr = 0;
       salinityString = "";
       m_sensorStringComplete = false;
       m_salinitySerial->flush();

       m_salinitySerial->print("c\r");
       delay(1000);
       m_salinitySerial->listen();

       for(m_ctr = 0; m_ctr < 11; m_ctr++)
       {
           m_salinity[m_ctr] = 0;
       }

       while (m_sensorStringComplete != true)
       {
           if(m_salinitySerial->available())
           {
               m_temp = m_salinitySerial->read();
               if (m_temp == '\r')
               {
                   m_salinity[m_salCtr] = '\0';
                   salinityString = m_salinity;
                   m_sensorStringComplete = true;
                   m_salinitySerial->flush();
               }
               else
               {
                   m_salinity[m_salCtr] = m_temp;
                   m_salCtr++;
               }
           }
       }

       m_salinitySerial->print("e\r");
       delay(1000);
       //salinityString = "70000,99,67";
}

//-----------------------------------------------------------------------------
// Description : reads the digital compass
//
boolean CSensor::readCompass(String& compassString)
{
    //reset used variables and holders
    m_compassSerial->listen();
    m_compassCtr = 0;
    m_tries = 0;
    m_temp = 0;
    m_compass[0] = 0;
    m_compass[1] = 0;
    m_compass[2] = 0;
    m_compass[3] = 0;
    m_compass[4] = 0;
    m_compass[5] = 0;

    m_compassSerial->flush();

    while(m_compassCtr < 5)
    {
        if(m_tries == 5000)
        {
            return false;
        }
        if(m_compassSerial->available())
        {
            m_temp = m_compassSerial->read();
        }
        else if (!m_compassSerial->available())
        {
            m_tries++;
            continue;
        }
        if (m_temp == 2 && m_compassCtr == 0)
        {
            m_compass[m_compassCtr] = m_temp;
            m_compassCtr++;
        }
        else if (m_temp == 3 && m_compassCtr == 4)
        {
            m_compass[m_compassCtr] = m_temp;
            m_compassCtr++;
            compassString = m_compass;
            m_returnVal = true;
        }
        else if(m_compassCtr == 1 || m_compassCtr == 2 || m_compassCtr == 3)
        {
            if(m_temp != 2)
            {
                m_compass[m_compassCtr] = m_temp;
                m_compassCtr++;
            }
            else
            {

            }
        }
    }

    return m_returnVal;
}

boolean CSensor::readGPS(String& gpsString)
{
    m_gpsSerial->listen();
    m_isGpsStringEnd = NULL;
    m_temp = 0;
    m_gpsCtr = 0;
    m_tries = 0;
    gpsString = "";

    for(m_ctr = 0; m_ctr <= 70; m_ctr++)
    {
        m_gpsArray[m_ctr] = 0;
    }

    m_gpsSerial->flush();

    while(m_isGpsStringEnd != true)
    {
        if(m_tries == 30000)
        {
            return false;
        }
        if(m_gpsSerial->available())
        {
            m_temp = m_gpsSerial->read();

            if(m_temp == '\r')
            {
                m_gpsArray[m_gpsCtr] = '\0';
                gpsString = m_gpsArray;
                m_isGpsStringEnd = true;
            }
            else
            {
                m_gpsArray[m_gpsCtr] = m_temp;
                m_gpsCtr++;
            }
        }
        else if (!m_gpsSerial->available())
        {
            m_tries++;
            continue;
        }
    }

    return m_isGpsStringEnd;
}

void CSensor::initGPS()
{
    delay(1000);
    m_gpsSerial->print("$PUNV,SET,00,0000,0,1000,9600,0010*cc\r\n");
    delay(1000);
}
