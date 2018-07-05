#ifndef RINEXRECORD_H
#define RINEXRECORD_H
#include <QString>

class RinexRecord
{

public:
    RinexRecord();
    RinexRecord(QString satelNumber, int recNumber, int strNumber, QString ep);

    QString satelleteNumber;
    int recordNumber;
    int stringNumber;
    QString epoch;

private:
};

#endif // RINEXRECORD_H
