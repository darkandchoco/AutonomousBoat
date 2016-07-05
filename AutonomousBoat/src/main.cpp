/**
*******************************************************************************
* @file    main.cpp
* @author  Richmond Ko
* @brief   Contains setup() and loop() function.
*
* The functions setup() and loop() are called by the main function inside the
* Arduino core library.
* Revision History:
* <ul>
*   <li> 07-May-2012 File created.
*   <li> 10-May-2012 File modified.
*   <li> 14-May-2012 Cleaned code.
*   <li> 15-May-2012 Removed delay(1000) since there is already a built in delay
*                    for the sending methods.
*   <li> 16-May-2012 Added test plan and some notes.
*                    Added a state machine example.
*   <li> 09-Jun-2012 Refactored code. See SVN changelog.
* </ul>
* <p>
* Copyright (c) Richmond Ko 2012. All rights reserved.
******************************************************************************/

//----------------------- INCLUDED FILES --------------------------------------
#include "CStateMachine.h"
#define MACHINE1
//----------------------- DECLARATIONS ----------------------------------------
static const int forwardTime = 2000;
static const int turnTime = 1000;
static const int acceptGPSError = 200;
CStateMachine myStateMachine;
//----------------------- MAIN FUNCTIONS --------------------------------------
//Put required initializations here
void setup()
{
    //initialize the state machine
    myStateMachine.initStateMachine();
}

//Forever looping code
void loop()
{
/*
todo:
    -XBEE CHECKER
*/

#ifdef MACHINE1

    if(myStateMachine.getNextState() == myStateMachine.m_stStoreHomeBase)
    {
        myStateMachine.stStoreHomeBase();
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stSending)
    {
        myStateMachine.stSending();
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stWriting)
    {
        myStateMachine.stWriting();
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stGoTo1)
    {
        myStateMachine.stGoTo(14563678, 120993043, false, forwardTime, turnTime, acceptGPSError);
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stGoTo2)
    {
        myStateMachine.stGoTo(14563856, 120992941, false, forwardTime, turnTime, acceptGPSError);
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stGoTo3)
    {
        myStateMachine.stGoTo(14563813, 120992833, false, forwardTime, turnTime, acceptGPSError);
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stGoTo4)
    {
        myStateMachine.stGoTo(14563646, 120992923, false, forwardTime, turnTime, acceptGPSError);
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stGoTo5)
    {
        myStateMachine.stGoTo(14563517, 120992994, false, forwardTime, turnTime, acceptGPSError);
    }
    else if(myStateMachine.getNextState() == myStateMachine.m_stGoingHome)
    {
        myStateMachine.stGoTo(0, 0, true, forwardTime, turnTime, acceptGPSError);
    }
//for actual rizal pool, change 200 difference to 50 in stGoTo and
//destLat = latitude;
//destLong = longitude;
#endif

#ifdef MACHINE2

#endif
}
