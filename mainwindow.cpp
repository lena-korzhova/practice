#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QStringList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Table = new QTableWidget();
    setCentralWidget(Table);
    QStringList headers;
    headers << "ID" << "Имя" << "Возраст";
    Table->setColumnCount(3);
    Table->setHorizontalHeaderLabels(headers);
    Table->verticalHeader()->setVisible(false);

    QHeaderView* tableHeader = Table->horizontalHeader();
    tableHeader->setSectionResizeMode(QHeaderView::Stretch);

    db = QSqlDatabase::addDatabase("QMYSQL", "connection"); // Настройка
    db.setHostName("localhost"); // Настройка
    db.setUserName("root"); // Настройка
    db.setPort(3306);// Настройка
    db.setDatabaseName("lenadb"); // Настройка
    db.setPassword("110698gtaiv"); // Настройка

    if(!db.open())
    {
        qDebug() << db.lastError(); //Если открыть не удалось - вывести ошибку
    } else {
        qDebug() << "Connected!";
        Connected();
    }
}

void MainWindow::Connected()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM Persons");

    while(query.next())
    {
        QString ID, name, age;

        ID = query.value(0).toString();
        name = query.value(1).toString();
        age = query.value(2).toString();

        Table->insertRow(Table->rowCount());
        QTableWidgetItem* itemID;
        QTableWidgetItem* itemName;
        QTableWidgetItem* itemAge;

        itemID = new QTableWidgetItem(ID);
        itemName = new QTableWidgetItem(name);
        itemAge = new QTableWidgetItem(age);

        Table->setItem(Table->rowCount() - 1, 0, itemID);
        Table->setItem(Table->rowCount() - 1, 1, itemName);
        Table->setItem(Table->rowCount() - 1, 2, itemAge);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
