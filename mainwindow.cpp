#include "mainwindow.h"
#include "ui_mainwindow.h"




#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаём окно настроек COM
    settingsWindow = new SettingsDialog(this);

    // Выполняем связку класса работы с портом окном настроек
    serialsettings = new SerialSettings(settingsWindow->Ports,                // ComboBox c доступными Com портами
                                        settingsWindow->Baudrate,             // ComboBox с настройками скорости
                                        settingsWindow->Parity,               // ComboBox с настройками паритета
                                        settingsWindow->Databits,             // ComboBox с настройками бит данных
                                        settingsWindow->Stopbits,             // ComboBox с настройками стоп-бит
                                        settingsWindow->Flowcontrol,          // ComboBox с настройками контроля
                                        settingsWindow->ConnectDisconnect);   // Кнопка подключения/отключения

    // Выполняем связку класса работы с консолью с формами
    console = new ConsoleWindow(serialsettings, ui->workspace, ui->message, ui->SendButton, ui->ClearButton);

    tableConsole = new TableConsole(this,
                                    ui->tableView,
                                    ui->tableMessage,
                                    ui->tableSendButton,
                                    ui->tableClearButton);

    ui->tableView->setShowGrid(false);

    // Делает во все окно
//   setCentralWidget(ui->tableView);
//    // Заголовок окна
//    setWindowTitle("Text Edit Delegate");
//    // Изменяет размер самого окна
//    //setGeometry(100, 250, 170, 500);


    QString str = "00 01 02 03 04 05 06 07 08 09 00 01 02 03 04 05 06 07 08 09 00 01 02 03 04 05 06 07 08 09";

    tableConsole->appendData(TableConsole::OUTGOING, &str);
    QByteArray arr = str.toLatin1();
    //tableConsole->appendData(&arr);
    str+=str;
    for(int i=0; i < 1000; i++)
    tableConsole->appendData(TableConsole::INCOMING, &str);


    //ui->tableView->set
            //->item(1,1)->setTextAlignment(Qt::AlignRight);


    // При запуске будем предлагать подключение
    settingsWindow->show();
}

MainWindow::~MainWindow()
{
    delete console;
    delete serialsettings;
    delete settingsWindow;
    delete ui;
}

void MainWindow::on_PortSettings_triggered()
{
    settingsWindow->show();
}
