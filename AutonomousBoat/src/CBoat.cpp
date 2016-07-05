/**
*******************************************************************************
* @file    CBoat.cpp
* @author  Richmond Ko
* @brief   CBoat class implementation.
*
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 14-May-2012 Added motor init and sensor init.
*                    Modified readAndSendSensors() method.
*   <li> 16-May-2012 Added code blocks for test plan 3.a.
*   <li> 17-May-2012 Changed implementation of the readAndSendSensors()
*                    Implemented getSignalStrength().
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/

//----------------------- INCLUDED FILES --------------------------------------
#include "CBoat.h"

//----------------------- METHODS OF CLASS CBoat ------------------------------
CBoat::CBoat()
{

}

CBoat::~CBoat()
{

}

//-----------------------------------------------------------------------------
// Description : Initialize all devices here.
//
void CBoat::init()
{
    m_myMotor.initMotor();
    m_myZigBee.initZigBee();
    m_mySensors.initSensors();
    m_mySensors.initGPS();
}

//-----------------------------------------------------------------------------
// Description : Movement implementation. Boat will move according to the
//               passed parameter values.
//
void CBoat::move(MOTOR::tMovementMode movementMode)
{
    if(movementMode == MOTOR::tForward)
    {
        m_myMotor.forward();
    }
    else if(movementMode == MOTOR::tSlow)
    {
        m_myMotor.slow();
    }
    else if(movementMode == MOTOR::tStop)
    {
        m_myMotor.stop();
    }
    else if(movementMode == MOTOR::tLeft)
    {
        m_myMotor.left();
    }
    else if(movementMode == MOTOR::tRight)
    {
        m_myMotor.right();
    }
    else if(movementMode == MOTOR::tStartUp)
    {
        m_myMotor.startUp();
    }
    else
    {
        return;
    }
}

//-----------------------------------------------------------------------------
// Description : This is the readAndSendSensors() implementation, it first reads the
//               temperature, then it appends the temperature identifier to the
//               sensorsData array before it appends the actual data. The same
//               is done with the other sensors. After all of this is done, this
//               method will send the created array of data.
//
boolean CBoat::readAndSendSensors()
{
    //create a uint8_t array with 15 bytes allocated space for temp, ph and
    //salinity, compass
    uint8_t sensorsData[] = {0, 0, 0, 0, 0, 0, 0, 0, 0,
            /*start of compass data*/0, 0, 0, 0, 0, 0/*1 bytes for id, 5 bytes
             * for actual compass string*/};

    uint8_t salinityHolder[] = {0, 0};

    m_commaCtr = 0;
    m_temperatureSensorValue = 0;
    m_phSensorValue = 0;
    m_salinityString = "";
    m_compassString = "";

    //read temperature sensor
    m_mySensors.readTemperature(m_temperatureSensorValue);

    //append temperature identifier (ID) for rowell to identify what data is next
    sensorsData[0] = SENSOR::temperatureId;
    //append MSB of temperature data
    //(because aRead has the chance to return 2 bytes of data)
    //ie: 255 decimal = 0xFF(1 byte) while 300 decimal = 0x012C(2 bytes)
    sensorsData[1] = m_temperatureSensorValue  >> 8 & 0xff;
    //append LSB of temperature data
    sensorsData[2] = m_temperatureSensorValue  & 0xff;

    //read ph sensor
    m_mySensors.readPh(m_phSensorValue);

    //append ph identifier
    sensorsData[3] = SENSOR::phId;
    //same technique used with temperature sensor
    sensorsData[4] = m_phSensorValue >> 8 & 0xff;
    sensorsData[5] = m_phSensorValue & 0xff;

    //append the salinity identifier
    sensorsData[6] = SENSOR::salinityId;

    m_mySensors.readSalinity(m_salinityString);

    //some counters
    m_sensorsDataCtr = 7;
    //7 because that was the last index of sensorsData that was used

    int salinityHolderCtr = 0;

    //process the whole string
    for(m_ctr = 0; m_ctr <= m_salinityString.length() - 1; m_ctr++)
    {
        //if there are already 2 commas then start storing the
        //next characters to the sensorsData array
        if(m_commaCtr == 2)
        {
            //for the first run, now its equal to 7
            //sensorsDataCtr++;
            //for the first run, store that character to sensorsData[7]
            //1 char = 1 byte = 1 uint8_t, so technically this is legal.
            salinityHolder[salinityHolderCtr] = m_salinityString.charAt(m_ctr);
            salinityHolderCtr++;
        }
        else if(m_salinityString.charAt(m_ctr) == ',')
        {
            m_commaCtr++;
        }
    }

    if(salinityHolderCtr == 1)
    {
        sensorsData[m_sensorsDataCtr] = 0x00;
        m_sensorsDataCtr++;
        sensorsData[m_sensorsDataCtr] = salinityHolder[0];
    }

    else
    {
        sensorsData[m_sensorsDataCtr] = salinityHolder[0];
        m_sensorsDataCtr++;
        sensorsData[m_sensorsDataCtr] = salinityHolder[1];
    }

    /**COMPASSSSSS**/
    //append compass id
    sensorsData[9] = SENSOR::compassId;

    //read compass
    m_returnVal = m_mySensors.readCompass(m_compassString);

    if(m_returnVal == true)
    {
        if((m_compassString.charAt(0) == 2) && (m_compassString.charAt(4) ==  3))
        {
            Serial.println("COK");
            m_sensorsDataCtr = 10;

            for(m_ctr = 0; m_ctr <= m_compassString.length() - 1; m_ctr++)
            {
                //should be 5 characters (0-4)
                sensorsData[m_sensorsDataCtr] = m_compassString.charAt(m_ctr);
                m_sensorsDataCtr++;
            }

            //send the data to the designated destination address.
            m_returnValXbee = m_myZigBee.sendData(sensorsData, sizeof(sensorsData));

            if(m_returnValXbee == true)
            {
                Serial.println("ZOK");
                m_sendIsSuccess = true;
            }
            else
            {
                Serial.println("ZE");
                m_sendIsSuccess = false;
            }
        }
        else
        {
            //Serial.println("CE");
            m_sendIsSuccess = false;
        }
    }
    else
    {
        Serial.println("CM");
        m_sendIsSuccess = false;
    }

    return m_sendIsSuccess;
}

boolean CBoat::readSensorsAndWriteToSdCard()
{
    //create a uint8_t array with 10 bytes allocated space for temp, ph and
    //salinity, compass
    uint8_t sensorsData[] = {0, 0, 0, 0, 0, 0, 0, 0, 0,
            /*start of compass data*/0, 0, 0, 0, 0, 0/*1 bytes for id, 5 bytes
             * for actual compass string*/};
    uint8_t salinityHolder[] = {0, 0};

    m_commaCtr = 0;
    m_temperatureSensorValue = 0;
    m_phSensorValue = 0;
    m_salinityString = "";
    m_compassString = "";

    //read temperature sensor
    m_mySensors.readTemperature(m_temperatureSensorValue);

    //append temperature identifier (ID) for rowell to identify what data is next
    sensorsData[0] = SENSOR::temperatureId;
    //append MSB of temperature data
    //(because aRead has the chance to return 2 bytes of data)
    //ie: 255 decimal = 0xFF(1 byte) while 300 decimal = 0x012C(2 bytes)
    sensorsData[1] = m_temperatureSensorValue  >> 8 & 0xff;
    //append LSB of temperature data
    sensorsData[2] = m_temperatureSensorValue  & 0xff;

    //read ph sensor
    m_mySensors.readPh(m_phSensorValue);

    //append ph identifier
    sensorsData[3] = SENSOR::phId;
    //same technique used with temperature sensor
    sensorsData[4] = m_phSensorValue >> 8 & 0xff;
    sensorsData[5] = m_phSensorValue & 0xff;

    //append the salinity identifier
    sensorsData[6] = SENSOR::salinityId;

    m_mySensors.readSalinity(m_salinityString);

    m_sensorsDataCtr = 7;
    //7 because that was the last index of sensorsData that was used

    int salinityHolderCtr = 0;

    //process the whole string
    for(m_ctr = 0; m_ctr <= m_salinityString.length() - 1; m_ctr++)
    {
        //if there are already 2 commas then start storing the
        //next characters to the sensorsData array
        if(m_commaCtr == 2)
        {
            //for the first run, now its equal to 7
            //sensorsDataCtr++;
            //for the first run, store that character to sensorsData[7]
            //1 char = 1 byte = 1 uint8_t, so technically this is legal.
            salinityHolder[salinityHolderCtr] = m_salinityString.charAt(m_ctr);
            salinityHolderCtr++;
        }
        else if(m_salinityString.charAt(m_ctr) == ',')
        {
            m_commaCtr++;
        }
    }

    if(salinityHolderCtr == 1)
    {
        sensorsData[m_sensorsDataCtr] = 0x00;
        m_sensorsDataCtr++;
        sensorsData[m_sensorsDataCtr] = salinityHolder[0];
    }

    else
    {
        sensorsData[m_sensorsDataCtr] = salinityHolder[0];
        m_sensorsDataCtr++;
        sensorsData[m_sensorsDataCtr] = salinityHolder[1];
    }

    /**COMPASSSSSS**/
    //append compass id
    sensorsData[9] = SENSOR::compassId;

    //read compass
    //Serial.println("READING COMPASS DATA..!");
    m_returnVal = m_mySensors.readCompass(m_compassString);

    if(m_returnVal == true)
    {
        if((m_compassString.charAt(0) == 2) && (m_compassString.charAt(4) ==  3))
        {
            Serial.println("COK");
            //next index is 10
            m_sensorsDataCtr = 10;

            for(m_ctr = 0; m_ctr <= m_compassString.length() - 1; m_ctr++)
            {
                //should be 5 characters (0-4)
                sensorsData[m_sensorsDataCtr] = m_compassString.charAt(m_ctr);
                m_sensorsDataCtr++;
            }

            for(m_ctr = 0; m_ctr < 15; m_ctr++)
            {
                m_returnValSd = m_mySdCard.write((int)sensorsData[m_ctr]);

                if(m_returnValSd == true)
                {
                    m_writeIsSuccess = true;
                }
                else
                {
                    Serial.println("SE");
                    return false;
                }
            }

            m_returnValSd = m_mySdCard.writeln();

            if(m_returnValSd == true)
            {
                m_writeIsSuccess = true;
            }
            else
            {
                //Serial.println("SE");
                m_writeIsSuccess = false;
            }
        }
        else
        {
            //Serial.println("CE");
            m_writeIsSuccess = false;
        }
    }
    else
    {
        Serial.println("CM");
        m_writeIsSuccess = false;
    }

    return m_writeIsSuccess;
}

boolean CBoat::getGPRMC()
{
    m_GPRMCString = "";

    m_returnValGps = m_mySensors.readGPS(m_GPRMCString);

    if(m_returnValGps == false)
    {
        Serial.println("GM");
        return false;
    }
    else if(m_returnValGps == true)
    {
        //Serial.println("GOK");
        Serial.println(m_GPRMCString);

        for(m_ctr = 0; m_ctr <= 6; m_ctr++)
        {
            m_gpsStrt[m_ctr] = 0;
        }

        for(m_ctr = 0; m_ctr <= 5; m_ctr++)
        {
            m_gpsStrt[m_ctr] = m_GPRMCString.charAt(m_ctr);
        }

        m_gpsStrt[6] = '\0';

        //Serial.println(m_gpsStrt);

        if(m_gpsStrt[0] == '$' && m_gpsStrt[1] == 'G' &&
           m_gpsStrt[2] == 'P' && m_gpsStrt[3] == 'R' &&
           m_gpsStrt[4] == 'M' && m_gpsStrt[5] == 'C')
        {
            Serial.println("RMOK");
            m_returnVal = true;
        }
        else
        {
            Serial.println("RMER");
            m_returnVal = false;
        }
    }

    return m_returnVal;
}

//get long and lat
boolean CBoat::getLongAndLat()
{
    m_isValid = NULL;
    m_isLongLatFinish = NULL;
    m_commaCtr = 0;
    m_gpsStringCtr = 0;
    m_longLatCtr = 0;
    m_tempCommaCtr = 0;

    for(m_ctr = 0; m_ctr <= 24; m_ctr++)
    {
        m_longlat[m_ctr] = 0;
    }

    m_returnValGPRMC = this->getGPRMC();

    if(m_returnValGPRMC ==  false)
    {
        return false;
    }
    else if(m_returnValGPRMC ==  true)
    {
        while(m_isValid != true)
        {
            if(m_commaCtr == 2)
            {
                if(m_GPRMCString.charAt(m_gpsStringCtr) == 'V')
                {
                    Serial.println("GV");
                    return false;
                }
                else if(m_GPRMCString.charAt(m_gpsStringCtr) == 'A')
                {
                    Serial.println("GA");
                    m_isValid = true;
                }
            }
            else if(m_GPRMCString.charAt(m_gpsStringCtr) == ',')
            {
                m_commaCtr++;
            }
            m_gpsStringCtr++;
        }
    }

    if(m_isValid == true)
    {
        m_commaCtr = 0;
        m_ctr = 0;

        while(m_isLongLatFinish != true)
        {
            if(m_commaCtr == 3)
            {
                m_longlat[m_longLatCtr] = m_GPRMCString.charAt(m_ctr);

                if(m_tempCommaCtr == 3)
                {
                    m_commaCtr = 7;
                }

                if(m_GPRMCString.charAt(m_ctr) == ',')
                {
                    m_tempCommaCtr++;
                }
                m_longLatCtr++;
                m_ctr++;
            }
            else if(m_commaCtr == 7)
            {
                m_longlat[24] = '\0';
                Serial.println("LLOK");
                m_isLongLatFinish = true;
                m_returnVal = true;
            }
            else if (m_GPRMCString.charAt(m_ctr) == ',')
            {
                m_commaCtr++;
                m_ctr++;
            }
            else
            {
                m_ctr++;
            }
        }
    }
    return m_returnVal;
}

boolean CBoat::getLongAndLatDateTime()
{
    m_isValid = NULL;
    m_isLongLatFinish = NULL;
    m_gpsStringCtr = 0;
    m_commaCtr = 0;
    m_longLatCtr = 0;
    m_tempCommaCtr = 0;

    for(m_ctr = 0; m_ctr <= 42; m_ctr++)
    {
        m_longLatTimeDate[m_ctr] = 0;
    }

    m_returnValGPRMC = this->getGPRMC();

    if(m_returnValGPRMC ==  false)
    {
        return false;
    }
    else if(m_returnValGPRMC ==  true)
    {
        while(m_isValid != true)
        {
            if(m_commaCtr == 2)
            {
                if(m_GPRMCString.charAt(m_gpsStringCtr) == 'V')
                {
                    Serial.println("GV");
                    return false;
                }
                else if(m_GPRMCString.charAt(m_gpsStringCtr) == 'A')
                {
                    Serial.println("GA");
                    m_isValid = true;
                }
            }
            else if(m_GPRMCString.charAt(m_gpsStringCtr) == ',')
            {
                m_commaCtr++;
            }
            m_gpsStringCtr++;
        }
    }

    if(m_isValid == true)
    {
        m_commaCtr = 0;
        m_ctr = 0;

        while(m_isLongLatFinish != true)
        {
            if(m_commaCtr == 1)
            {
                if(m_GPRMCString.charAt(m_ctr) == ',')
                {
                    m_longLatTimeDate[m_longLatCtr] = m_GPRMCString.charAt(m_ctr);
                    m_longLatCtr++;
                    m_ctr++;
                    m_commaCtr++;
                }
                else
                {
                    m_longLatTimeDate[m_longLatCtr] = m_GPRMCString.charAt(m_ctr);
                    m_longLatCtr++;
                    m_ctr++;
                }
            }
            else if(m_commaCtr == 3)
            {
                if(m_GPRMCString.charAt(m_ctr) == ',')
                {
                    m_tempCommaCtr++;
                }

                if(m_tempCommaCtr == 4)
                {
                    m_longLatTimeDate[m_longLatCtr] = m_GPRMCString.charAt(m_ctr);
                    m_longLatCtr++;
                    m_ctr++;
                    m_commaCtr = 7;
                }
                else
                {
                    m_longLatTimeDate[m_longLatCtr] = m_GPRMCString.charAt(m_ctr);
                    m_longLatCtr++;
                    m_ctr++;
                }
            }
            else if(m_commaCtr == 9)
            {
                if(m_GPRMCString.charAt(m_ctr) == ',')
                {
                    m_ctr++;
                    m_commaCtr++;
                }
                else
                {
                    m_longLatTimeDate[m_longLatCtr] = m_GPRMCString.charAt(m_ctr);
                    m_longLatCtr++;
                    m_ctr++;
                }
           }
           else if(m_commaCtr == 10)
           {
              m_longLatTimeDate[42] = '\0';
              //Serial.println(m_longLatTimeDate);
              Serial.println("LDTOK");
              m_isLongLatFinish = true;
              m_returnVal = true;
           }
           else if (m_GPRMCString.charAt(m_ctr) == ',')
           {
               m_commaCtr++;
               m_ctr++;
           }
           else
           {
               m_ctr++;
           }
       }
    }
    return m_returnVal;
}

boolean CBoat::readAndSendLongLat()
{
    uint8_t longLatArray[] = {0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0};

    longLatArray[0] = 0xAA;
    m_longLatCtr = 0;

    m_returnValLongLat = this->getLongAndLat();

    if(m_returnValLongLat == false)
    {
        Serial.println("LLER");
        return false;
    }
    else if(m_returnValLongLat == true)
    {
        m_longLatCtr = 1;
        Serial.println(m_longlat);

        for(m_ctr = 0; m_ctr <= 23; m_ctr++)
        {
            longLatArray[m_longLatCtr] = m_longlat[m_ctr];
            m_longLatCtr++;
        }

        m_returnValXbee = m_myZigBee.sendDataLL(longLatArray, sizeof(longLatArray));

        if(m_returnValXbee == false)
        {
            //Serial.println("ZE");
            m_returnVal =  false;
        }
        else if(m_returnValXbee == true)
        {
            //Serial.println("ZOK");
            m_returnVal = true;
        }
    }
    return m_returnVal;
}

boolean CBoat::readAndWriteLL()
{
    uint8_t longLatArray[] =
           {0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0};

    m_returnValLongLat = this->getLongAndLatDateTime();

    if(m_returnValLongLat == false)
    {
        Serial.println("LLER");
        return false;
    }
    else if(m_returnValLongLat == true)
    {
        //Serial.println(m_longLatTimeDate);

        for(m_ctr = 0; m_ctr <= 41; m_ctr++)
        {
            longLatArray[m_ctr] = m_longLatTimeDate[m_ctr];
        }

        for(m_ctr = 0; m_ctr <= 41; m_ctr++)
        {
            m_returnValSd = m_mySdCard.writeC((char)longLatArray[m_ctr]);

            if(m_returnValSd == true)
            {
                m_writeIsSuccess = true;
            }
            else
            {
                Serial.println("SE");
                return false;
            }
        }

        m_returnValSd = m_mySdCard.writeln();

        if(m_returnValSd == true)
        {
            m_writeIsSuccess = true;
        }
        else
        {
            //Serial.println("SE");
            m_writeIsSuccess = false;
        }
    }

    return m_writeIsSuccess;
}

boolean CBoat::storeHomeLongLat()
{
    m_homeLongitude = "";
    m_homeLatitude = "";
    m_homeBaseLongDir = 0;
    m_homeBaseLatDir = 0;

    for(m_ctr = 0; m_ctr <= 8; m_ctr++)
    {
        m_tempLong[m_ctr] = 0;
    }

    for(m_ctr = 0; m_ctr <= 9; m_ctr++)
    {
        m_tempLat[m_ctr] = 0;
    }

    m_returnVal = this->getLongAndLat();

    if(m_returnVal != true)
    {
        return false;
    }
    else
    {
        m_tempLong[0] = m_longlat[0];
        m_tempLong[1] = m_longlat[1];
        m_tempLong[2] = m_longlat[2];
        m_tempLong[3] = m_longlat[3];
        m_tempLong[4] = m_longlat[5];
        m_tempLong[5] = m_longlat[6];
        m_tempLong[6] = m_longlat[7];
        m_tempLong[7] = m_longlat[8];
        m_tempLong[8] = '\0';
        m_homeLongitude = m_tempLong;
        m_homeBaseLongitude = m_homeLongitude.toInt();

        m_tempLat[0] = m_longlat[12];
        m_tempLat[1] = m_longlat[13];
        m_tempLat[2] = m_longlat[14];
        m_tempLat[3] = m_longlat[15];
        m_tempLat[4] = m_longlat[16];
        m_tempLat[5] = m_longlat[18];
        m_tempLat[6] = m_longlat[19];
        m_tempLat[7] = m_longlat[20];
        m_tempLat[8] = m_longlat[21];
        m_tempLat[9] = '\0';
        m_homeLatitude = m_tempLat;
        m_homeBaseLatitude = m_homeLatitude.toInt();

        m_homeBaseLongDir = m_longlat[10];
        m_homeBaseLatDir = m_longlat[23];

        return true;
    }
}

boolean CBoat::storeCurrentLongLat()
{
    m_currLongitude = "";
    m_currLatitude = "";
    m_currentLongDir = 0;
    m_currentLatDir = 0;

    for(m_ctr = 0; m_ctr <= 8; m_ctr++)
    {
        m_tempLong[m_ctr] = 0;
    }

    for(m_ctr = 0; m_ctr <= 9; m_ctr++)
    {
        m_tempLat[m_ctr] = 0;
    }

    m_returnVal = this->getLongAndLat();

    if(m_returnVal != true)
    {
        return false;
    }
    else
    {
        m_tempLong[0] = m_longlat[0];
        m_tempLong[1] = m_longlat[1];
        m_tempLong[2] = m_longlat[2];
        m_tempLong[3] = m_longlat[3];
        m_tempLong[4] = m_longlat[5];
        m_tempLong[5] = m_longlat[6];
        m_tempLong[6] = m_longlat[7];
        m_tempLong[7] = m_longlat[8];
        m_tempLong[8] = '\0';
        m_currLongitude = m_tempLong;
        m_currentLongitude = m_currLongitude.toInt();

        m_tempLat[0] = m_longlat[12];
        m_tempLat[1] = m_longlat[13];
        m_tempLat[2] = m_longlat[14];
        m_tempLat[3] = m_longlat[15];
        m_tempLat[4] = m_longlat[16];
        m_tempLat[5] = m_longlat[18];
        m_tempLat[6] = m_longlat[19];
        m_tempLat[7] = m_longlat[20];
        m_tempLat[8] = m_longlat[21];
        m_tempLat[9] = '\0';
        m_currLatitude = m_tempLat;
        m_currentLatitude = m_currLatitude.toInt();

        m_currentLongDir = m_longlat[10];
        m_currentLatDir = m_longlat[23];

        return true;
    }
}

boolean CBoat::storeCurrentCompass()
{
    m_returnVal = m_mySensors.readCompass(m_currCompass);

    for(m_ctr = 0; m_ctr <= 3; m_ctr++)
    {
        m_currentCompass[m_ctr] = 0;
    }

    if(m_returnVal != true)
    {
       return false;
    }
    else
    {
        m_currentCompass[0] = m_currCompass.charAt(1);
        m_currentCompass[1] = m_currCompass.charAt(2);
        m_currentCompass[2] = m_currCompass.charAt(3);
        m_currentCompass[3] = '\0';
        m_currCompass = m_currentCompass;
        m_currentBearing = m_currCompass.toInt();

        return true;
    }
}

void CBoat::getCurrentLongLat(long& longitude, long& latitude)
{
    longitude = m_currentLongitude;
    latitude = m_currentLatitude;
}

void CBoat::getHomeBase(long& longitude, long& latitude)
{
    longitude = m_homeBaseLongitude;
    latitude = m_homeBaseLatitude;
}

int CBoat::getCurrentBearing()
{
    return m_currentBearing;
}
