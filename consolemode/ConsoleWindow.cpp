#include "ConsoleWindow.h"

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QTime>

ConsoleWindow::ConsoleWindow(QObject*        parent,
                             SerialForGUI*   Serial,
                             QPlainTextEdit* Workspace,
                             QLineEdit *     Message,
                             QPushButton*    SendButton,
                             QPushButton*    ClearButton) : QObject(parent)
{
    /* Сохраняем указатели на формы и прочие классы внутри */
    ConsoleWindow::Serial      = Serial;
    ConsoleWindow::Workspace   = Workspace;
    ConsoleWindow::Message     = Message;
    ConsoleWindow::SendButton  = SendButton;
    ConsoleWindow::ClearButton = ClearButton;

    /* Настраиваем свойства и внешний вид рабочих областей */
    Workspace->setReadOnly(true);                            /* Запрещаем пользователю редактировать консоль */
    Workspace->setFont(QFont("System", 8, QFont::Normal));   /* Устанавливаем шрифт */
    Workspace->setStyleSheet(                                /* Выбор палитры */
                "QPlainTextEdit{color: #aaaaaa;"
                               "background-color: #000000;"
                               "selection-background-color: #606060;"
                                "selection-color: #ffffff;}");

    /* Привязываем кнопки клавиатуры к кнопкам UI */
    SendButton->setShortcut(Qt::Key_Return);

    /* Выполняем функциональные подключения */
    connect(SendButton,  &QPushButton::clicked,            /* Нажатие кнопки Send  */
            this,        &ConsoleWindow::send);            /* инициирует отправку данных */
    connect(ClearButton, &QPushButton::clicked,            /* Нажатие кнопки Clear */
            this,        &ConsoleWindow::clear);           /* инициирует очистку окна терминала */
    connect(Serial,      &SerialForGUI::receivedNewData,   /* Наличие новых данных в com-порт */
            this,        &ConsoleWindow::receive);         /* инициирует его чтение и отображение принятого */
}

ConsoleWindow::~ConsoleWindow()
{
}

void ConsoleWindow::send (void)
{
    QString Msg(Message->text());
    if(Msg == "" || Serial->getConnectionStatus() == CLOSED)   /* Если в поле ввода пусто,  */
        return;                                                /* или порт закрыт, ничего не делаем */

    Serial->write(Msg.toLocal8Bit());            /* Отправляем данные преобразованные в QByteArray */
    Serial->waitForBytesWritten(100);            /* Ждем 100 мс */
    Message->clear();                            /* Поле ввода очищаем */
    Workspace->moveCursor(QTextCursor::End);     /* Смещаем курсор текста гарантированно в конец */
    Workspace->textCursor().insertText(">> ");
    Workspace->textCursor().insertText(Msg);
    Workspace->textCursor().insertText("\n");
}

void ConsoleWindow::clear (void)
{
    Workspace->clear();   /* Очищаем окно терминала */
}

void ConsoleWindow::receive(void)
{
    QString message = QString::fromLocal8Bit(Serial->getData());  /* Принимаем данные и сразу преобразуем в строку */
    Workspace->moveCursor(QTextCursor::End);                      /* Смещаем курсор текста гарантированно в конец */
    Workspace->textCursor().insertText(message);                  /* Печатаем то, что пришло */
    Workspace->textCursor().insertText("\n");                     /* Переведем курсор на следующую строку */
}