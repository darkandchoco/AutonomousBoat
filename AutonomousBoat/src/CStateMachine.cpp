#include "CStateMachine.h"
#define FORTESTING
//#define FORACTUAL

void CStateMachine::initStateMachine()
{
    m_myBoat.init();
    m_nextState = m_stStoreHomeBase;
    m_wayPoint = 0;
}

int CStateMachine::getNextState()
{
    return m_nextState;
}

void CStateMachine::resetGoHome()
{
    m_isHomeLat = NULL;
    m_isHomeLong = NULL;
}

void CStateMachine::forward(int forwardTime)
{
    m_myBoat.move(MOTOR::tForward);
    delay(forwardTime);
    m_myBoat.move(MOTOR::tStop);
}

void CStateMachine::turn(MOTOR::tMovementMode direction, int turnTime)
{
    m_myBoat.move(direction);
    m_myBoat.move(MOTOR::tSlow);
    delay(turnTime);
    m_myBoat.move(MOTOR::tStop);
}

void CStateMachine::stStoreHomeBase()
{
    m_result = NULL;
    m_tries = 0;

    while(m_result != true)
    {
        if(m_tries == m_maxTries)
        {
            m_result = true;
        }
        else
        {
            m_result = m_myBoat.storeHomeLongLat();

            if(m_result != true)
            {
                m_tries++;
            }
        }
    }

    if(m_tries == m_maxTries)
    {
        Serial.println("0");
        m_nextState = m_stGoingHome;//go back home state
    }
    else
    {
        Serial.println("0");
        m_nextState = m_stSending;
    }
}

void CStateMachine::stSending()
{
    m_result = NULL;
    m_xbeeResult = NULL;
    m_tries = 0;
    m_xbeeTries = 0;

    while(m_result != true)
    {
        if(m_tries == m_maxTries)
        {
            m_result = true;
        }
        else
        {
            m_result = m_myBoat.readAndSendSensors();

            if(m_result != true)
            {
                m_tries++;
            }
            else
            {
            m_result = m_myBoat.readAndSendLongLat();

            if(m_result != true)
            {
                m_tries++;
            }
            }
        }
    }

    if(m_tries == m_maxTries)
    {
        Serial.println("1");
        m_nextState = m_stGoingHome;//go back home state
    }
    else
    {
        Serial.println("1");
        m_nextState = m_stWriting;
    }
}

void CStateMachine::stWriting()
{
    m_result = NULL;
    m_tries = 0;

    while(m_result != true)
    {
        if(m_tries == m_maxTries)
        {
            m_result = true;
        }
        else
        {
            m_result = m_myBoat.readSensorsAndWriteToSdCard();

            if(m_result != true)
            {
                m_tries++;
            }
            else
            {
                m_result = m_myBoat.readAndWriteLL();

                if(m_result != true)
                {
                    m_tries++;
                }
            }
        }
    }

    if(m_tries == m_maxTries)
    {
        Serial.println("2");
        m_nextState = m_stGoingHome;//go back home state
    }
    else
    {
        m_wayPoint++;
        Serial.println("2");
        if(m_wayPoint == 1)
        {
            m_nextState = m_stGoTo1;
        }
        else if(m_wayPoint == 2)
        {
            m_nextState = m_stGoTo2;
        }
        else if(m_wayPoint == 3)
        {
            m_nextState = m_stGoTo3;
        }
        else if(m_wayPoint == 4)
        {
            m_nextState = m_stGoTo4;
        }
        else if(m_wayPoint == 5)
        {
            m_nextState = m_stGoTo5;
        }
        else if(m_wayPoint == 6)
        {
            m_wayPoint = 0;
            m_nextState = m_stGoingHome;
        }
    }
}

void CStateMachine::stGoTo(long latitude, long longitude, boolean isToHome, int forwardTime, int turnTime, int acceptGPSError)
{
    m_result = NULL;
    boolean returnCompass = false;
    long currentLong;
    long currentLat;
    long homeLong;
    long homeLat;
    long destLat;
    long destLong;
    int currentBearing;

    delay(3000);

    while(m_result != true)
    {
        m_result = m_myBoat.storeCurrentLongLat();
    }

    while(returnCompass != true)
    {
        returnCompass = m_myBoat.storeCurrentCompass();
    }

    currentBearing = m_myBoat.getCurrentBearing();
    m_myBoat.getCurrentLongLat(currentLong, currentLat);
    m_myBoat.getHomeBase(homeLong, homeLat);

    if(isToHome == true)
    {
        destLat = homeLat;
        destLong = homeLong;
    }
    else
    {

#ifdef FORACTUAL
        destLat = latitude;
        destLong = longitude;
#endif

#ifdef FORTESTING
        destLat = homeLat;
        destLong = homeLat;
#endif

    }

    if(m_isHomeLat != true)
    {
        if (abs(destLat - currentLat) < acceptGPSError )
        {
            //stop
            //latitude approximately at destination already
            m_isHomeLat = true;
        }
        else if(destLat < currentLat)
        {
            //gone north
            //must go south
            if(abs(currentBearing - 180)  <= 10)
            {
                //im at south, go forward
                this->forward(forwardTime);
            }
            else
            {
                if(currentBearing > 180)
                {
                    //turn left a bit
                    this->turn(MOTOR::tLeft, turnTime);
                }
                else if(currentBearing < 180)
                {
                    //turn right a bit
                    this->turn(MOTOR::tRight, turnTime);
                }
            }
        }
        else if(destLat > currentLat)
        {
            //gone south
            //must go north
            if(currentBearing  <= 10 || currentBearing >= 350)
            {
                //im at north, go forward
                this->forward(forwardTime);
            }
            else
            {
                if(currentBearing > 180)
                {
                    //turn right a bit
                    this->turn(MOTOR::tRight, turnTime);
                }
                else if(currentBearing < 180)
                {
                    //turn left a bit
                    this->turn(MOTOR::tLeft, turnTime);
                }
            }
        }
    }
    else
    {
        if(m_isHomeLong != true)
        {
            if (abs(destLong - currentLong) < acceptGPSError )
            {
                //stop
                //longitude approximately at home base already
                m_isHomeLong = true;
            }
            else if(destLong < currentLong)
            {
                //gone east
                //must go west
                if(abs(currentBearing - 270)  <= 10)
                {
                    //im at west, go forward!
                    this->forward(forwardTime);
                }
                else
                {
                    if(currentBearing < 270 && currentBearing > 90)
                    {
                        //turn right a bit
                        this->turn(MOTOR::tRight, turnTime);
                    }
                    else if(currentBearing > 270 && currentBearing < 90)
                    {
                        //turn left a bit
                        this->turn(MOTOR::tLeft, turnTime);
                    }
                }
            }
            else if(destLong > currentLong)
            {
                //gone west
                //must go east
                if(abs(currentBearing - 90)  <= 10)
                {
                    //im at east, go forward!
                    this->forward(forwardTime);
                }
                else
                {
                    if(currentBearing < 270 && currentBearing > 90)
                    {
                        //turn left a bit
                        this->turn(MOTOR::tLeft, turnTime);
                    }
                    else if(currentBearing > 270 && currentBearing < 90)
                    {
                        //turn right a bit
                        this->turn(MOTOR::tRight, turnTime);
                    }
                }
            }
        }
    }
    if(m_isHomeLong == true)
    {
        this->resetGoHome();
        Serial.println("h");
        m_nextState = m_stSending;
    }
    else if(m_wayPoint == 1)
    {
        Serial.println("3");
        m_nextState = m_stGoTo1;
    }
    else if(m_wayPoint == 2)
    {
        Serial.println("4");
        m_nextState = m_stGoTo2;
    }
    else if(m_wayPoint == 3)
    {
        Serial.println("5");
        m_nextState = m_stGoTo3;
    }
    else if(m_wayPoint == 4)
    {
        Serial.println("6");
        m_nextState = m_stGoTo4;
    }
    else if(m_wayPoint == 5)
    {
        Serial.println("7");
        m_nextState = m_stGoTo5;
    }
    else if(m_wayPoint == 0)
    {
        Serial.println("8");
        m_nextState = m_stGoingHome;
    }
}
