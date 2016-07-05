/**
*******************************************************************************
* @file    CZigBee.cpp
* @author  Richmond Ko
* @brief   CZigBee class implementation.
*
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 14-May-2012 Applied ZIGBEE namespace usage.
*   <li> 14-May-2012 Added implementation of flashLED(),
*                    Added initializations of class members.
*                    Removed SH and SL parameters of sendDataTo()
*   <li> 17-May-2012 Changed implementation of flashLED() method.
*                    Added some initializations.
*                    Added status checking inside sendDataTo().
*                    Implemented readRssi() method.
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/

//----------------------- INCLUDED FILES --------------------------------------
#include "CZigBee.h"
#include "types.h"

//----------------------- METHODS OF CLASS CZigBee ----------------------------
CZigBee::CZigBee()
{

}

CZigBee::~CZigBee()
{

}

//-----------------------------------------------------------------------------
// Description : Initialize the XBee object here and set the baudrate.
//
void CZigBee::initZigBee()
{
    //initialize the XBee object to enable XBee library functions
    m_myXBee = XBee();
    //create an XBee 64-bit address object and set the destination address
    m_destinationAddress = XBeeAddress64(ZIGBEE::destinationSH,
            ZIGBEE::destinationSL);
    //init status response
    m_txStatus = TxStatusResponse();
    //set the desired baud rate.
    m_myXBee.begin(ZIGBEE::baudRate);

    //for rx mode
    m_reponse = XBeeResponse();
    m_rx16Response = Rx16Response();
    m_rx64Response = Rx64Response();
}

//-----------------------------------------------------------------------------
// Description : This method allows the user to sent data to a specified destination
//               in types.h
//
boolean CZigBee::sendData(uint8_t* payload, int size)
{
    //create an XBee tx request packet object
    Tx64Request myTxRequest = Tx64Request(m_destinationAddress, payload, size);

    //send the created tx request packet using UART/hardware serial.
    m_myXBee.send(myTxRequest);
    m_returnVal = true;
/*

    if (m_myXBee.readPacket(5000))
    {
        //check that it should be a tx status response
        if (m_myXBee.getResponse().getApiId() == TX_STATUS_RESPONSE)
        {
            m_myXBee.getResponse().getTxStatusResponse(m_txStatus);

            //get the delivery status
            if (m_txStatus.getStatus() == SUCCESS)
            {
                //packet sent successfully!
                m_returnVal = true;
            }
            else
            {
                // the receiving XBee did not receive our packet. is it powered on?
                m_returnVal = false;
            }
        }
    }
    else if (m_myXBee.getResponse().isError())
    {
        //error reading packet
        m_returnVal = false;
    }
    else
    {
        //local XBee did not provide a timely TX Status Response.
        //Radio is not configured properly or connected
        m_returnVal = false;
    }
*/

    return m_returnVal;
}

boolean CZigBee::sendDataLL(uint8_t* payload, int size)
{
    //create an XBee tx request packet object
    Tx64Request myTxRequest = Tx64Request(m_destinationAddress, payload, size);

    //send the created tx request packet using UART/hardware serial.
    m_myXBee.send(myTxRequest);

    return true;
}
