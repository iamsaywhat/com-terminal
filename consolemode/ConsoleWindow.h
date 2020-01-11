#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "SerialForGUI.h"

class ConsoleWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit ConsoleWindow(SerialForGUI*   Serial,
                           QPlainTextEdit* Workspace,
                           QLineEdit *     Message,
                           QPushButton*    SendButton,
                           QPushButton*    ClearButton);
private:
    SerialForGUI*   Serial;
    QPlainTextEdit* Workspace;
    QLineEdit *     Message;
    QPushButton*    SendButton;
    QPushButton*    ClearButton;

public slots:
   void readMessage (void);

private slots:
    void sendMessage (void);
    void clearWorkspace (void);
    //void readMessage (void);

};

#endif // CONSOLEWINDOW_H
