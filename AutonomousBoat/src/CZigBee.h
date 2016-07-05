/**
*******************************************************************************
* @file    CZigBee.h
* @author  Richmond Ko
* @brief   CZigBee class declaration.
*
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 15-May-2012 Added flashLED() method. Added members:
*                    XBeeAddress64 destinationAddress;
*                    ZBTxStatusResponse txStatus;
*                    static const int statusLEDPin = 13;
*                    Removed SH and SL param of sendDataTo()
*   <li> 17-May-2012 Added readRssi() method, added some members, changed
*                    flashLED() method.
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/
#ifndef CZIGBEE_H_
#define CZIGBEE_H_

//----------------------- INCLUDED FILES --------------------------------------
#include <XBee.h>   //XBee library, see XBee examples in Arduino IDE

/**
*******************************************************************************
* @brief CZigBee class.
*
* The CZigBee class will have the ability to send data to a specified destination
* address (64-bit serial number of an XBee modem).
******************************************************************************/
class CZigBee
{
public:
    /**
    *****************************************************************************
    * @brief Constructor.
    ****************************************************************************/
    CZigBee();

    /**
    *****************************************************************************
    * @brief Destructor.
    ****************************************************************************/
    ~CZigBee();

    /**
    *****************************************************************************
    * @brief Intialization of ZigBee method.
    *
    * Initialize the XBee device by declaring an XBee object from the XBee
    * library the calling the begin method and setting the baud rate
    * of the XBee object. To change the baud rate, see types.h
    *
    * @see types.h
    ****************************************************************************/
    void initZigBee();

    /**
    *****************************************************************************
    * @brief Send data method.
    *
    * Sending of data requires the data itself and the size of the data in bytes.
    *
    * @param   payload     user defined data
    * @param   size        size of the payload in bytes.
    * @return  returns a boolean if data is sent successfully or not.
    * @see types.h for destination address
    ****************************************************************************/
    boolean sendData(uint8_t* payload, int size);

    /**
    *****************************************************************************
    * @brief Send data method.
    *
    * Sending of data requires the data itself and the size of the data in bytes.
    *
    * @param   payload     user defined data
    * @param   size        size of the payload in bytes.
    * @return  returns a boolean if data is sent successfully or not.
    * @see types.h for destination address
    ****************************************************************************/
    boolean sendDataLL(uint8_t* payload, int size);
	
private:
    /**
    *****************************************************************************
    * @brief Class scope holder of methods' coolean returns.
    ****************************************************************************/
    boolean m_returnVal;

    /**
    *****************************************************************************
    * @brief XBee object
    ****************************************************************************/
    XBee m_myXBee;    //from the XBee library

    /**
    *****************************************************************************
    * @brief XBee destination address object.
    ****************************************************************************/
    XBeeAddress64 m_destinationAddress;

    /**
    *****************************************************************************
    * @brief Transmit status object.
    ****************************************************************************/
    TxStatusResponse m_txStatus;

    /**
    *****************************************************************************
    * @brief XBee response object.
    ****************************************************************************/
    XBeeResponse m_reponse;

    /**
    *****************************************************************************
    * @brief XBee 16-bit response object.
    ****************************************************************************/
    Rx16Response m_rx16Response;

    /**
    *****************************************************************************
    * @brief XBee  64-bit response object.
    ****************************************************************************/
    Rx64Response m_rx64Response;
};

#endif /* CZIGBEE_H_ */
