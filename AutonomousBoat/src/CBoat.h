/**
*******************************************************************************
* @file    CBoat.h
* @author  Richmond Ko
* @brief   CBoat class declaration.
*
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 14-May-2012 Removed all sensor classes, added CSensor as a member.
*                    Applied MOTOR namespace usage.
*   <li> 17-May-2012 Removed member pins, moved to CSensors.
*                    Added getSignalStrength() method.
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/
#ifndef CBOAT_H
#define CBOAT_H

//----------------------- INCLUDED FILES --------------------------------------
#include <Arduino.h>    //standard arduino header
#include "types.h"      //constants
//member classes:
#include "CMotor.h"
#include "CSdCard.h"
#include "CZigBee.h"
#include "CSensor.h"

/**
*******************************************************************************
* @brief CBoat class.
*
* CBoat class will have the ability to move, read and send sensor values,
* write data to an SD card, read GPS and magnetometer values. It HAS a motor,
* ZigBee modem, sensors(ph, temperature and salinity), magnetometer(compass),
* GPS, and an SD card.
*
* <ul>
*   <li>Just declare "CBoat userVariable" to initialize a CBoat object
*   <li>To use CBoat's methods, just use "userVariable.method(parameters)",
*       ie: CBoat myBoat; myBoat.move(MOTOR::tForward);
* </ul>
******************************************************************************/

class CBoat
{
public:
    /**
    *****************************************************************************
    * @brief Constructor.
    ****************************************************************************/
    CBoat();

    /**
    *****************************************************************************
    * @brief Destructor.
    ****************************************************************************/
    ~CBoat();

    /**
    *****************************************************************************
    * @brief CBoat initialization method.
    *
    * This method initializes the different devices it has like the motor, sensors
    * and other devices.
    ****************************************************************************/
    void init();

    /**
    *****************************************************************************
    * @brief CBoat move method.
    *
    * This method allows the boat to control its motors, available movement modes
    * are forward, left, right, stop and slow.
    *
    * @param    movementMode    The movement mode specified.
    ****************************************************************************/
    void move(MOTOR::tMovementMode movementMode);

    /**
    *****************************************************************************
    * @brief This method reads sensors' values and send them via XBee
    *
    * This method reads the temperature, ph, salinity sensor and compass values, appends
    * the sensor identifiers and sends them to the designated 64-bit destination
    * address of an XBee defined in types.h.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean readAndSendSensors();

    /**
    *****************************************************************************
    * @brief This method reads sensors' values and writes them to the sd card.
    *
    * This method reads the temperature, ph, salinity sensor and compass values,
    * appends the sensor identifiers and writes them on the sd card.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean readSensorsAndWriteToSdCard();

    /**
    *****************************************************************************
    * @brief Get a GPRMC sentence from the GPS.
    *
    * This method reads the GPS module and only gets the GPRMC sentence and stores
    * it in the member variable m_GPRMCString.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean getGPRMC();

    /**
    *****************************************************************************
    * @brief Get the longitude and latitude from a GPRMC sentence
    *
    * This method processes the read GPRMC sentence and extracts the longitude
    * and latitude data and stores it in the member variable m_longlat.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean getLongAndLat();

    /**
    *****************************************************************************
    * @brief Get the longitude, latitude, date and time from a GPRMC sentence
    *
    * This method processes the read GPRMC sentence and extracts the longitude,
    * latitude, time and date data and stores it in the member variable
    * m_longLatTimeDate.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean getLongAndLatDateTime();

    /**
    *****************************************************************************
    * @brief Sends the longitude and latitude data via XBee to the base station
    *
    * This method processes the read GPRMC sentence and extracts the longitude,
    * latitude data and sends it to the base station via XBee.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean readAndSendLongLat();

    /**
    *****************************************************************************
    * @brief Reads longitude, latitude, time and date data and writes it to the
    *        SD Card.
    *
    * This method processes the read GPRMC sentence and extracts the longitude,
    * latitude, time and date data and writes it to the SD card.
    *
    * @return   returns true if success.
    * @see types.h
    ****************************************************************************/
    boolean readAndWriteLL();
	
    /**
    *****************************************************************************
    * @brief Stores the longitude and latitude of the home base.
    *
    * This method processes the read GPRMC sentence and extracts the longitude,
    * latitude then stores it as its home base.
    *
    * @return   returns true if success.
    ****************************************************************************/
    boolean storeHomeLongLat();

    /**
    *****************************************************************************
    * @brief Stores the current longitude and latitude.
    *
    * This method processes the read GPRMC sentence and extracts the longitude,
    * latitude then stores it as the boat's current coordiates.
    *
    * @return   returns true if success.
    ****************************************************************************/
    boolean storeCurrentLongLat();

    /**
    *****************************************************************************
    * @brief Stores the current bearing of the boat.
    *
    * This method reads the on board digital compass and stores its current
    * bearing in degrees.
    *
    * @return   returns true if success.
    ****************************************************************************/
    boolean storeCurrentCompass();

    /**
    *****************************************************************************
    * @brief Gets the stored current longitude and latitude.
    *
    * This method just returns the stored coordinates.
    ****************************************************************************/
    void getCurrentLongLat(long& longitude, long& latitude);

    /**
    *****************************************************************************
    * @brief Gets the stored home base longitude and latitude.
    *
    * This method just returns the stored home base coordinates.
    ****************************************************************************/
    void getHomeBase(long& longitude, long& latitude);

    /**
    *****************************************************************************
    * @brief Gets the stored current bearing.
    *
    * This method returns the stores current bearing in degrees.
    ****************************************************************************/
    int getCurrentBearing();

private:
    /**
    *****************************************************************************
    * @brief boolean return value used by the class
    ****************************************************************************/
    boolean m_returnVal;

    /**
    *****************************************************************************
    * @brief boolean return value used by the read and send sensors
    ****************************************************************************/
    boolean m_returnValXbee;

    /**
    *****************************************************************************
    * @brief boolean return value from sd.
    ****************************************************************************/
    boolean m_returnValSd;

    /**
    *****************************************************************************
    * @brief boolean holder for read and send.
    ****************************************************************************/
    boolean m_sendIsSuccess;

    /**
    *****************************************************************************
    * @brief boolean holder for read and write.
    ****************************************************************************/
    boolean m_writeIsSuccess;

    /**
    *****************************************************************************
    * @brief boolean holder for return value of GPS
    ****************************************************************************/
    boolean m_returnValGps;

    /**
    *****************************************************************************
    * @brief boolean holder for return value of GPRMC
    ****************************************************************************/
    boolean m_returnValGPRMC;

    /**
    *****************************************************************************
    * @brief boolean holder if GPRMC sentence is valid.
    ****************************************************************************/
    boolean m_isValid;

    /**
    *****************************************************************************
    * @brief boolean holder for return value of getLongLat.
    ****************************************************************************/
    boolean m_returnValLongLat;

    /**
    *****************************************************************************
    * @brief boolean holder if long lat is finished.
    ****************************************************************************/
    boolean m_isLongLatFinish;

    /**
    *****************************************************************************
    * @brief A character to hold direction of home base longitude.
    ****************************************************************************/
    char m_homeBaseLongDir;

    /**
    *****************************************************************************
    * @brief A character to hold direction of home base latitude
    ****************************************************************************/
    char m_homeBaseLatDir;

    /**
    *****************************************************************************
    * @brief A character to hold direction of current longitude.
    ****************************************************************************/
    char m_currentLongDir;

    /**
    *****************************************************************************
    * @brief A character to hold direction of current latitude
    ****************************************************************************/
    char m_currentLatDir;

    /**
    *****************************************************************************
    * @brief Temperature sensor value holder.
    ****************************************************************************/
    int m_temperatureSensorValue;

    /**
    *****************************************************************************
    * @brief Ph sensor value holder.
    ****************************************************************************/
    int m_phSensorValue;

    /**
    *****************************************************************************
    * @brief Comma counter.
    ****************************************************************************/
    int m_commaCtr;

    /**
    *****************************************************************************
    * @brief Sensors data byte counter.
    ****************************************************************************/
    int m_sensorsDataCtr;

    /**
    *****************************************************************************
    * @brief Class scope counter for all loops.
    ****************************************************************************/
    int m_ctr;

    /**
    *****************************************************************************
    * @brief Longitude latitude array counter.
    ****************************************************************************/
    int m_longLatCtr;

    /**
    *****************************************************************************
    * @brief Temporary comma counter.
    ****************************************************************************/
    int m_tempCommaCtr;

    /**
    *****************************************************************************
    * @brief GPS string counter.
    ****************************************************************************/
    int m_gpsStringCtr;

    /**
    *****************************************************************************
    * @brief Compass current bearing value
    ****************************************************************************/
    int m_currentBearing;

    /**
    *****************************************************************************
    * @brief long integer to hold home base' longitude value.
    ****************************************************************************/
    long m_homeBaseLongitude;

    /**
    *****************************************************************************
    * @brief long integer to hold home base' latitude value.
    ****************************************************************************/
    long m_homeBaseLatitude;

    /**
    *****************************************************************************
    * @brief long integer to hold current longitude value.
    ****************************************************************************/
    long m_currentLongitude;

    /**
    *****************************************************************************
    * @brief long integer to hold current latitude value.
    ****************************************************************************/
    long m_currentLatitude;

    /**
    *****************************************************************************
    * @brief String to hold GPS data.
    ****************************************************************************/
    String m_GPRMCString;

    /**
    *****************************************************************************
    * @brief String to hold salinity data.
    ****************************************************************************/
    String m_salinityString;

    /**
    *****************************************************************************
    * @brief String to hold compass data.
    ****************************************************************************/
    String m_compassString;

    /**
    *****************************************************************************
    * @brief String to hold home base' longitude.
    ****************************************************************************/
    String m_homeLongitude;

    /**
    *****************************************************************************
    * @brief String to hold home base' latitude.
    ****************************************************************************/
    String m_homeLatitude;

    /**
    *****************************************************************************
    * @brief String to hold current longitude.
    ****************************************************************************/
    String m_currLongitude;

    /**
    *****************************************************************************
    * @brief String to hold current latitude.
    ****************************************************************************/
    String m_currLatitude;

    /**
    *****************************************************************************
    * @brief String to hold current bearing
    ****************************************************************************/
    String m_currCompass;

    /**
    *****************************************************************************
    * @brief Char array to hold current bearing.
    ****************************************************************************/
    char m_currentCompass[4];

    /**
    *****************************************************************************
    * @brief 7 byte character array to store GPS sentence start.
    ****************************************************************************/
    char m_gpsStrt[7];

    /**
    *****************************************************************************
    * @brief 9 byte character array to store current longitude.
    ****************************************************************************/
    char m_tempLong[9];

    /**
    *****************************************************************************
    * @brief 10 byte character array to store current latitude.
    ****************************************************************************/
    char m_tempLat[10];

    /**
    *****************************************************************************
    * @brief 25 byte character array to store long lat data.
    ****************************************************************************/
    char m_longlat[25];

    /**
    *****************************************************************************
    * @brief 43 byte character array to store long, lat, time and date data.
    ****************************************************************************/
    char m_longLatTimeDate[43];

    /**
    *****************************************************************************
    * @brief Motor object.
    ****************************************************************************/
    CMotor m_myMotor;

    /**
    *****************************************************************************
    * @brief Sd card object.
    ****************************************************************************/
    CSdCard m_mySdCard;

    /**
    *****************************************************************************
    * @brief XBee object
    ****************************************************************************/
    CZigBee m_myZigBee;

    /**
    *****************************************************************************
    * @brief Sensors object.
    ****************************************************************************/
    CSensor m_mySensors;
};

#endif /* CBOAT_H */
