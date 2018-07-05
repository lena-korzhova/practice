#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QStringList>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Connected();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db; //Создали объект подключения
    QTableWidget* Table; //Создали объект таблицы
};

#endif // MAINWINDOW_H
