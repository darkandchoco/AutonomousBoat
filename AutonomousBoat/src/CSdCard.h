#ifndef CSDCARD_H_
#define CSDCARD_H_

#include <Arduino.h>
#include <SdFat.h>

class CSdCard
{
public:
    CSdCard();
    ~CSdCard();
    boolean write(int data);
    boolean writeC(char data);
    boolean writeln();

private:
    static const int m_chipSelect = 4;
    SdFat m_sd;
    SdFile m_myFile;
};

#endif /* CSDCARD_H_ */
