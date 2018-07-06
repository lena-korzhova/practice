#include "rinexrecord.h"

RinexRecord::RinexRecord(QString satelNumber, int recNumber, int strNumber, QString ep)
{
    satelleteNumber = satelNumber;
    recordNumber = recNumber;
    stringNumber = strNumber;
    epoch = ep;
}

RinexRecord::RinexRecord()
{
    satelleteNumber = "";
    recordNumber = 0;
    stringNumber = 0;
    epoch = "";
}
