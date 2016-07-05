#include "CSdCard.h"

CSdCard::CSdCard()
{

}

CSdCard::~CSdCard()
{

}

boolean CSdCard::write(int data)
{
    if (!m_sd.init(SPI_QUARTER_SPEED, m_chipSelect))
    {
        //this->sd.initErrorHalt();
        return false;
    }

    // open the file for write at end like the Native SD library
    if (!m_myFile.open("Sensors.txt", O_RDWR | O_CREAT | O_AT_END))
    {
        //this->sd.errorHalt("Opening Sensors.txt for write failed");
        return false;
    }
    // put string to write here
    m_myFile.print(data);
    m_myFile.print(" ");
    m_myFile.close();

    return true;
}

boolean CSdCard::writeC(char data)
{
    if (!m_sd.init(SPI_QUARTER_SPEED, m_chipSelect))
    {
        //this->sd.initErrorHalt();
        return false;
    }

    // open the file for write at end like the Native SD library
    if (!m_myFile.open("Sensors.txt", O_RDWR | O_CREAT | O_AT_END))
    {
        //this->sd.errorHalt("Opening Sensors.txt for write failed");
        return false;
    }
    // put string to write here
    m_myFile.print(data);
    m_myFile.close();

    return true;
}

boolean CSdCard::writeln()
{
    if (!m_sd.init(SPI_QUARTER_SPEED, m_chipSelect))
    {
        //this->sd.initErrorHalt();
        return false;
    }

    // open the file for write at end like the Native SD library
    if (!m_myFile.open("Sensors.txt", O_RDWR | O_CREAT | O_AT_END))
    {
        //this->sd.errorHalt("Opening Sensors.txt for write failed");
        return false;
    }
    // put string to write here
    m_myFile.println("");
    m_myFile.close();

    return true;
}
