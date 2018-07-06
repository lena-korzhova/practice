#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QRegExp>
#include <QVector>
#include <QScrollBar>
#include <QDateTime>

#include <rinexrecord.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QVector<RinexRecord> getVector(QString fileName); //Функция, формирующая вектор записей из заданного файла
    ~MainWindow();

private slots:
    void on_loadOne_clicked(); //Нажатие кнопки "Загрузить первый файл"

    void on_loadTwo_clicked(); //Нажатие кнопки "Загрузить второй файл"

    void on_compareWithTwo_clicked(); //Нажатие кнопки "Сравнить со вторым файлом"

    void on_compareWithOne_clicked(); //Нажатие кнопки "Сравнить с первым файлом"

private:
    QVector<RinexRecord> vector1; //Вектор, который формируется из первого файла
    QVector<RinexRecord> vector2; //Вектор, который формируется из второго файла
    QVector<RinexRecord> differenceWithTwo; //Вектор записей, которые есть в первом, но нет во втором
    QVector<RinexRecord> differenceWithOne; //Вектор записей, которые есть во втором, но нет в первом
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
