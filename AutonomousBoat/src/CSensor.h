/**
*******************************************************************************
* @file    CSensor.h
* @author  Richmond Ko
* @brief   CSensor class declaration.
*
* Revision History:
* <ul>
*   <li> 14-May-2012 File created.
*   <li> 17-May-2012 Removed readSensors(), replaced with readTemperature(),
*                    readPh() and readSalinity().
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/
#ifndef CSENSOR_H_
#define CSENSOR_H_

//----------------------- INCLUDED FILES --------------------------------------
#include <Arduino.h>    //for using String class
#include <SoftwareSerial.h> //for software serial class

/**
*******************************************************************************
* @brief CSensor class.
*
* The CSensor class will have the ability to read the different sensor values
* and return it to the user.
******************************************************************************/
class CSensor
{
public:
    /**
    *****************************************************************************
    * @brief Constructor.
    ****************************************************************************/
    CSensor();

    /**
    *****************************************************************************
    * @brief Destructor.
    ****************************************************************************/
    ~CSensor();

    /**
    *****************************************************************************
    * @brief Initialize the sensors.
    *
    * This method sets the analog reference of the ADC to DEFAULT for the
    * temperature and ph sensors, it also sets the pins to be used as software
    * serial of the Atlas Scientific 3-in-1 sensor. Sets the pins to be used by
    * the gps and the digital compass.
    ****************************************************************************/
    void initSensors();

    /**
    *****************************************************************************
    * @brief Read the analog signal of the temperature sensor.
    *
    * This method reads the analog signal input in temperatureSensorPin, it gets
    * 20 samples and averages the readings, then assigns the value to the passed
    * by reference holder.
    ****************************************************************************/
    void readTemperature(int& temperatureValue);

    /**
    *****************************************************************************
    * @brief Read the analog signal of the ph sensor.
    *
    * This method reads the analog signal input in phSensorPin, it gets 20
    * samples and averages the readings, then assigns the value to the passed
    * by reference holder.
    ****************************************************************************/
    void readPh(int& phValue);

    /**
    *****************************************************************************
    * @brief Read the string value of the sensor.
    *
    * This method sends a command to the EC circuit to get the sensor values,
    * it is packaged in a String class that is passed by reference.
    *
    * @return   Returns the String value of the sensor.
    ****************************************************************************/
    void readSalinity(String& salinityString);

    /**
    *****************************************************************************
    * @brief Read the analog signal of the digital compass.
    *
    * This method gets a reading from 0 to 359 from the digital compass.
    * it is packaged in a String class that is passed by reference.
    *
    * @param compassString   Returns the String value of the sensor.
    * @return                Returns true if success.
    ****************************************************************************/
    boolean readCompass(String& compassString);

    /**
    *****************************************************************************
    * @brief Read the GPS module.
    *
    * This method reads a GPS sentence from the GPS module.
    *
    * @param     gpsString   String where the GPS sentence is stored.
    * @return                Returns true if success.
    ****************************************************************************/
    boolean readGPS(String& gpsString);

    /**
    *****************************************************************************
    * @brief Set the GPS module to only sent GPRMC sentences
    *
    * This method sets a command sequence to the GPS module and SET the module
    * to only send GPRMC sentences.
    ****************************************************************************/
    void initGPS();

private:
    /**
    *****************************************************************************
    * @brief A boolean to know of sensor string is complete.
    ****************************************************************************/
    boolean m_sensorStringComplete;

    /**
    *****************************************************************************
    * @brief A boolean holder for methods' returns
    ****************************************************************************/
    boolean m_returnVal;

    /**
    *****************************************************************************
    * @brief A boolean to know if GPS string is finished.
    ****************************************************************************/
    boolean m_isGpsStringEnd;

    /**
    *****************************************************************************
    * @brief Temporary storage variable for serial read.
    ****************************************************************************/
    char m_temp;

    /**
    *****************************************************************************
    * @brief Temperature sensor pin.
    ****************************************************************************/
    static const int m_temperatureSensorPin = 0;

    /**
    *****************************************************************************
    * @brief Ph sensor pin.
    ****************************************************************************/
    static const int m_phSensorPin = 1;

    /**
    *****************************************************************************
    * @brief GPS sensor RX pin.
    ****************************************************************************/
    static const uint8_t m_gpsSensorRXPin = 2;

    /**
    *****************************************************************************
    * @brief GPS sensor TX pin.
    ****************************************************************************/
    static const uint8_t m_gpsSensorTXPin = 3;

    /**
    *****************************************************************************
    * @brief Salinity sensor RX pin.
    ****************************************************************************/
    static const uint8_t m_salinitySensorRXPin = 18;

    /**
    *****************************************************************************
    * @brief Salinity sensor TX pin.
    ****************************************************************************/
    static const uint8_t m_salinitySensorTXPin = 19;

    /**
    *****************************************************************************
    * @brief Compass sensor RX pin.
    ****************************************************************************/
    static const uint8_t m_compassSensorRXPin = 6;

    /**
    *****************************************************************************
    * @brief Compass sensor TX pin.
    ****************************************************************************/
    static const uint8_t m_compassSensorTXPin = 7;

    /**
    *****************************************************************************
    * @brief A member variable to store the analog readings.
    ****************************************************************************/
    int m_analogReading;

    /**
    *****************************************************************************
    * @brief A member variable to store the count from the loop of the compass.
    ****************************************************************************/
    int m_compassCtr;

    /**
    *****************************************************************************
    * @brief Class scope counter for all loops.
    ****************************************************************************/
    int m_ctr;

    /**
    *****************************************************************************
    * @brief Tries counter to count number of tries for compass and GPS.
    ****************************************************************************/
    int m_tries;

    /**
    *****************************************************************************
    * @brief GPS character counter for gps array.
    ****************************************************************************/
    int m_gpsCtr;

    /**
    *****************************************************************************
    * @brief Salinity array counter
    ****************************************************************************/
    int m_salCtr;

    /**
    *****************************************************************************
    * @brief A member String to input command to salinity sensor.
    ****************************************************************************/
    String m_inputString;

    /**
    *****************************************************************************
    * @brief A member variable to store the reading from the compass.
    ****************************************************************************/
    char m_compass[6];

    /**
    *****************************************************************************
    * @brief Character array to store salinity data.
    ****************************************************************************/
    char m_salinity[12];

    /**
    *****************************************************************************
    * @brief Software serial object for salinity sensor.
    ****************************************************************************/
    SoftwareSerial* m_salinitySerial;

    /**
    *****************************************************************************
    * @brief Software serial object for digital compass.
    ****************************************************************************/
    SoftwareSerial* m_compassSerial;

    /**
    *****************************************************************************
    * @brief Software serial object for GPS.
    ****************************************************************************/
    SoftwareSerial* m_gpsSerial;

    /**
    *****************************************************************************
    * @brief A 70 byte character array to store the GPS sentence.
    ****************************************************************************/
    char m_gpsArray[70];
};

#endif /* CSENSOR_H_ */
