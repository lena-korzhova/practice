#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rinexrecord.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file("C:/Qt/Projects/build-Lena2-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/Brdc1820.18g");

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    } else {
        qDebug() << "OPENED";
    }

    QVector<RinexRecord> vector;

    QRegExp exp;
    exp.setPattern("R[0-9][0-9] [0-9][0-9][0-9][0-9]");
    int stringNumber = 0;
    int recordNumber = 0;
    while(!file.atEnd())
    {
        stringNumber++;
        QString line = file.readLine();
        if(line.contains(exp))
        {
            recordNumber++;
            QString satNumber, epoch;

            QStringList list = line.split(" ");

            satNumber = list[0];

            QString check = list[6];
            if(check.length() > 2)
            {
                list[6] = check.mid(0, 2);
            }

            epoch = list[1] + "." + list[2] + "." + list[3] + "-" + list[4] + ":" + list[5] + ":" + list[6];

            RinexRecord record(satNumber, recordNumber, stringNumber, epoch);

            vector.append(record);
        }
    }

    for (int i = 0; i < vector.size(); i++)
    {
        qDebug() << vector[i].recordNumber;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
