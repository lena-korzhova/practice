#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

QVector<RinexRecord> MainWindow::getVector(QString fileName) //Функция, формирующая вектор записей из заданного файла
{
    QFile file(QCoreApplication::applicationDirPath() + "/" + fileName); //Открываем файл с полученным именем
    QVector<RinexRecord> vector; //Объявляем вектор, котрый буем формировать, исходя из данных в файле

    if(!file.open(QIODevice::ReadOnly)) { //Если не удалось открыть фай
        QMessageBox::information(this, "Error!", file.errorString()); //То выведем сообщение ошибки
        ui->compareWithOne->setEnabled(false); //Убедимся, что кнопки сравнений нельзя нажать(т.к. вектор сформировать не удалось)
        ui->compareWithTwo->setEnabled(false); //Убедимся, что кнопки сравнений нельзя нажать(т.к. вектор сформировать не удалось)
    } else {
        QRegExp exp; //Регулярное выражение
        exp.setPattern("R[0-9][0-9] [0-9][0-9][0-9][0-9]"); //Описали это выражение. Ищем строку вида "R** ****"
        int stringNumber = 0; //Объявляем переменную номера строки в записи
        int recordNumber = 0; //Объявляем переменную номера записи
        while(!file.atEnd()) //Пока в файле есть что читать, читаем
        {
            stringNumber++;//Если было что читать => прочитали новую строку => увеличиваем номер строки
            QString line = file.readLine(); //Читаем строку
            if(line.contains(exp)) //Если она удовлетворила регулярному выражению, то это наша запись
            {
                recordNumber++; //Увеличиваем номер текущей записи
                QString satNumber, epoch, check;
                QStringList list = line.split(" ");

                satNumber = list[0]; //Парсим запись
                check = list[6]; //Парсим запись

                if(check.length() > 2)
                {
                    list[6] = check.mid(0, 2); //Парсим запись
                }

                epoch = list[1] + "." + list[2] + "." + list[3] + "-" + list[4] + ":" + list[5] + ":" + list[6]; //Парсим запись

                RinexRecord record(satNumber, recordNumber, stringNumber, epoch); //Создали запись с заданными полями

                vector.append(record); //Добавили новую запись в вектор
            }
        }
    }
    return vector; //После добавления в вектор всех записей возвращаем его
}

void MainWindow::on_loadOne_clicked() //Нажали на загрузку первого файла
{
    vector1 = getVector(ui->editOne->text()); //Получили из этого файла вектор
    ui->plainOne->clear(); //Очистили текстовое поле, куда записывается список записей

    if(vector1.size() > 0) //Если полученный вектор содержит записи
    {
        ui->plainOne->appendPlainText("Всего записей: " + QString::number(vector1.size())); //Вывод информации о векторе
        ui->plainOne->appendPlainText("---------------------------------------");
        for(int i = 0; i < vector1.size(); i++) //Вывод информации о векторе
        {
            ui->plainOne->appendPlainText("Запись №: " + QString::number(vector1[i].recordNumber)); //Вывод информации о векторе
            ui->plainOne->appendPlainText("Аппарат №: " + vector1[i].satelleteNumber); //Вывод информации о векторе
            ui->plainOne->appendPlainText("Эпоха: " + vector1[i].epoch); //Вывод информации о векторе
            ui->plainOne->appendPlainText("Строка №: " + QString::number(vector1[i].stringNumber)); //Вывод информации о векторе
            ui->plainOne->appendPlainText("---------------------------------------");
        }

        QScrollBar *vScrollBar = ui->plainOne->verticalScrollBar(); //Листаем скролл бар вверх
        vScrollBar->triggerAction(QScrollBar::SliderToMinimum); //Листаем скролл бар вверх

        if(vector2.size() > 0) { //У нас сформирован вектор из файла 1. А если вектор из файла 2 тоже уже сформирован
                                 //то делаем кнопки сравнения файлов доступнми для нажатия, т.к. оба вектора сформированы
            ui->compareWithOne->setEnabled(true);
            ui->compareWithTwo->setEnabled(true);
        }
    } else ui->plainOne->appendPlainText("Вектор пуст"); //Иначе выводим в поле соответстующего вектора, что он пуст
}

void MainWindow::on_loadTwo_clicked() //Аналогично загрузке файла 1
{
    vector2 = getVector(ui->editTwo->text());
    ui->plainTwo->clear();

    if(vector2.size() > 0)
    {
        ui->plainTwo->appendPlainText("Всего записей: " + QString::number(vector2.size()));
        ui->plainTwo->appendPlainText("---------------------------------------");
        for(int i = 0; i < vector2.size(); i++)
        {
            ui->plainTwo->appendPlainText("Запись №: " + QString::number(vector2[i].recordNumber));
            ui->plainTwo->appendPlainText("Аппарат №: " + vector2[i].satelleteNumber);
            ui->plainTwo->appendPlainText("Эпоха: " + vector2[i].epoch);
            ui->plainTwo->appendPlainText("Строка №: " + QString::number(vector2[i].stringNumber));
            ui->plainTwo->appendPlainText("---------------------------------------");
        }

        QScrollBar *vScrollBar = ui->plainTwo->verticalScrollBar();
        vScrollBar->triggerAction(QScrollBar::SliderToMinimum);

        if(vector1.size() > 0) {
            ui->compareWithOne->setEnabled(true);
            ui->compareWithTwo->setEnabled(true);
        }
    } else ui->plainOne->appendPlainText("Вектор пуст");
}

void MainWindow::on_compareWithTwo_clicked() //Нажатие кнопки сравнения первого файла со вторым
{
    differenceWithTwo.clear(); //Очищаем поле, куда будет записываться разница
    for(int i = 0; i < vector1.size(); ++i)
    {
        //Идея такая. Формируем строчки с аппаратоми и эпохами из файла 1, которая присутствовала бы в текстовом поле второго файла.
        //Если бы там тоже была такая запись. Далее смотрим, есть ли такая строчка в текстовом поле второго файла.
        //Если такая строка не найдена, значит мы нашли запись, которая есть в первом файле, но ее нет во втором.
        if(!ui->plainTwo->toPlainText().contains("Аппарат №: " + vector1[i].satelleteNumber + "\nЭпоха: " + vector1[i].epoch)) //Содержит?
        {
            RinexRecord newDifference(vector1[i].satelleteNumber, vector1[i].recordNumber, vector1[i].stringNumber, vector1[i].epoch);
            //Не содержит => формируем запись, которую мы не нашли в текстовом поле второго файла
            differenceWithTwo.append(newDifference); //Добавляем в список другие не найденных во втором файле записей
        }
    }

    ui->differenceWithTwo->clear(); //Очищаем поле, содержащее не найденные во втором файле записи
    if(differenceWithTwo.size() > 0) //Если не найденные записи есть
    {
        ui->differenceWithTwo->appendPlainText("Список записей, которых нет во втором файле, но они есть в первом."); //Вывод информации
        ui->differenceWithTwo->appendPlainText("Всего записей: " + QString::number(differenceWithTwo.size())); //Вывод информации
        ui->differenceWithTwo->appendPlainText("---------------------------------------");
        for(int i = 0; i < differenceWithTwo.size(); i++)
        {
            ui->differenceWithTwo->appendPlainText("Запись №: " + QString::number(differenceWithTwo[i].recordNumber)); //Вывод информации
            ui->differenceWithTwo->appendPlainText("Аппарат №: " + differenceWithTwo[i].satelleteNumber); //Вывод информации
            ui->differenceWithTwo->appendPlainText("Эпоха: " + differenceWithTwo[i].epoch); //Вывод информации
            ui->differenceWithTwo->appendPlainText("Строка №: " + QString::number(differenceWithTwo[i].stringNumber)); //Вывод информации
            ui->differenceWithTwo->appendPlainText("---------------------------------------");
        }

        QScrollBar *vScrollBar = ui->differenceWithTwo->verticalScrollBar(); //Листаем скролл бар вверх
        vScrollBar->triggerAction(QScrollBar::SliderToMinimum); //Листаем скролл бар вверх
    }
}

void MainWindow::on_compareWithOne_clicked() //Аналогично сравнению со вторым файлом
{
    differenceWithOne.clear();
    for(int i = 0; i < vector2.size(); ++i)
    {
        if(!ui->plainOne->toPlainText().contains("Аппарат №: " + vector2[i].satelleteNumber + "\nЭпоха: " + vector2[i].epoch))
        {
            RinexRecord newDifference(vector2[i].satelleteNumber, vector2[i].recordNumber, vector2[i].stringNumber, vector2[i].epoch);
            differenceWithOne.append(newDifference);
        }
    }

    ui->differenceWithOne->clear();
    if(differenceWithOne.size() > 0)
    {
        ui->differenceWithOne->appendPlainText("Список записей, которых нет в первом файле, но они есть во втором.");
        ui->differenceWithOne->appendPlainText("Всего записей: " + QString::number(differenceWithOne.size()));
        ui->differenceWithOne->appendPlainText("---------------------------------------");
        for(int i = 0; i < differenceWithOne.size(); i++)
        {
            ui->differenceWithOne->appendPlainText("Запись №: " + QString::number(differenceWithOne[i].recordNumber));
            ui->differenceWithOne->appendPlainText("Аппарат №: " + differenceWithOne[i].satelleteNumber);
            ui->differenceWithOne->appendPlainText("Эпоха: " + differenceWithOne[i].epoch);
            ui->differenceWithOne->appendPlainText("Строка №: " + QString::number(differenceWithOne[i].stringNumber));
            ui->differenceWithOne->appendPlainText("---------------------------------------");
        }

        QScrollBar *vScrollBar = ui->differenceWithOne->verticalScrollBar();
        vScrollBar->triggerAction(QScrollBar::SliderToMinimum);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
