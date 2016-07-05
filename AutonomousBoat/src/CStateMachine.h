#ifndef CSTATEMACHINE_H_
#define CSTATEMACHINE_H_

#include "CBoat.h"

class CStateMachine
{
public:
    void initStateMachine();
    int getNextState();
    void forward(int forwardTime);
    void turn(MOTOR::tMovementMode direction, int turnTime);
    void stSending();
    void stWriting();
    void stStoreHomeBase();
    void stGoTo(long latitude, long longitude, boolean isToHome, int forwardTime, int turnTime, int acceptGPSError);
    void resetGoHome();
    static const int m_stStoreHomeBase = 0;
    static const int m_stSending = 1;
    static const int m_stWriting = 2;
    static const int m_stGoTo1 = 3;
    static const int m_stGoTo2 = 4;
    static const int m_stGoTo3 = 5;
    static const int m_stGoTo4 = 6;
    static const int m_stGoTo5 = 7;
    static const int m_stGoingHome = 8;

private:
    boolean m_result;
    boolean m_xbeeResult;
    boolean m_isHomeLat;
    boolean m_isHomeLong;
    int m_nextState;
    int m_tries;
    int m_xbeeTries;
    int m_wayPoint;
    static const int m_maxTries = 50;
    CBoat m_myBoat;
};

#endif /* CSTATEMACHINE_H_ */
