#include "guicontroller.h"
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QDebug>

Ui::MainWidget* GuiController::gui = nullptr;

GuiController::GuiController(QObject *parent, Ui::MainWidget* gui) : QObject(parent)
{
    GuiController::gui = gui;
    parent->installEventFilter(this);
    setProperties();

    // Обслуживание основного меню
    connect(gui->showConnectionButton,    &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexSettings);
                     gui->rightStackedPanel->setCurrentIndex(GuiController::settingsMenu::settingsIndexConnection);});
    connect(gui->switchToConsoleButton,   &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexConsole);});
    connect(gui->switchToTableButton,     &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexTable);});
    connect(gui->switchToConverterButton, &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexConverter);});
    connect(gui->showSettingsButton,      &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexSettings);});
    // Переключение между вкладками
    connect(gui->connectionContentsButton, &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexConnection);});
    connect(gui->generalContentButton,     &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexGeneral);});
    connect(gui->consoleContentButton,     &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexConsole);});
    connect(gui->tableContentButton,       &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexTable);});
    connect(gui->logsContentButton,        &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexLogs);});
    connect(gui->bindsContentButton,       &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexBinds);});
    // Вкладка General
    connect(gui->buttonGeneralApply, &QPushButton::pressed, [=](){
            emit currentThemeChanged(gui->comboBoxTheme->currentIndex());
            emit currentTextCodecChanged(gui->comboBoxCodec->currentIndex());
            emit currentLanguageChanged(gui->comboBoxLanguage->currentIndex());
            emit captureTimeChanges(gui->spinBoxCaptureTime->value());
            emit captureBytesChanges(gui->spinBoxCaptureBytes->value());
    });
    // Вкладка Console
    connect(gui->checkboxConsoleEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->consoleEchoChanged(true);
                            else
                                emit this->consoleEchoChanged(false);});
    connect(gui->buttonConsoleHotKey1, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey2, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey3, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey4, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    // Вкладка Table
    connect(gui->checkboxTableEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->tableEchoChanged(true);
                            else
                                emit this->tableEchoChanged(false);});
    connect(gui->buttonTableHotKey1, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey2, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey3, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey4, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->lineEditTableHotKey1, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey2, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey3, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey4, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    // Вкладка Log
    connect(gui->checkBoxLogEnable, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->logEnableChanged(true);
                            else
                                emit this->logEnableChanged(false);});
    connect(gui->buttonLogPath, &QToolButton::clicked, this, &GuiController::selectLogPath);
    connect(gui->buttonLogApply, &QPushButton::pressed, [=](){
            emit logColumnSizeChanged(gui->spinBoxLogColumnSize->value());
            emit logColumnSpacing(gui->spinBoxLogSpace->value());
    });

    gui->checkboxConsoleEcho->setCheckState(Qt::CheckState::Checked);
    gui->checkboxTableEcho->setCheckState(Qt::CheckState::Checked);
}
GuiController::~GuiController(){
    delete hexValidator;
}
void GuiController::setProperties (void){
    setPropertiesMainWidget();
    setPropertiesMenu();
    setPropertiesSectionConnection();
    setPropertiesSectionGeneral();
    setPropertiesSectionConsole();
    setPropertiesSectionTable();
    setPropertiesSectionLogs();
    setPropertiesSectionBinds();
}
void GuiController::setPropertiesMainWidget(void){
    gui->centralWidget->layout()->setMargin(0);
    gui->closeButton->setText("");
    gui->maximazeButton->setText("");
    gui->minimizeButton->setText("");
    gui->appName->setText("");
    gui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    gui->showConnectionButton->setMinimumSize(50,50);
    gui->showConnectionButton->setMinimumSize(50,50);
    gui->showSettingsButton->setMinimumSize(50,50);
    gui->switchToConsoleButton->setMinimumSize(50,50);
    gui->switchToTableButton->setMinimumSize(50,50);
    gui->switchToConverterButton->setMinimumSize(50,50);
    gui->showConnectionButton->setText("");
    gui->showConnectionButton->setText("");
    gui->showSettingsButton->setText("");
    gui->switchToConsoleButton->setText("");
    gui->switchToTableButton->setText("");
    gui->switchToConverterButton->setText("");
}
void GuiController::setPropertiesMenu(void){
    gui->connectionContentsButton->setText(tr("Connection"));
    gui->generalContentButton->setText(tr("General"));
    gui->consoleContentButton->setText(tr("Console"));
    gui->tableContentButton->setText(tr("Table"));
    gui->logsContentButton->setText(tr("Logs"));
    gui->bindsContentButton->setText(tr("Binds"));
    gui->infoLabel->setText(tr("Version: 0.0.0"));
    gui->infoLabel->setFont(QFont("Arial", 8, QFont::Bold));
    gui->infoLabel->setAlignment(Qt::AlignRight);
    gui->settingsPage->layout()->setMargin(0);
    gui->pageDelimiterLayout->setMargin(0);
    gui->leftPanelContents->layout()->setMargin(0);
    gui->rightPanelContents->layout()->setMargin(0);
    gui->rightStackedPanel->layout()->setMargin(0);
    gui->pageDelimiterLayout->setSpacing(0);
}
void GuiController::setPropertiesSectionConnection (void){
    gui->labelSectionConnection->setText(tr("Connection"));
    gui->labelSectionConnection->setFont(QFont("Arial", 14, QFont::Bold));
    gui->labelPorts->setText(tr("Port:"));
    gui->labelData->setText(tr("Data:"));
    gui->labelStopBits->setText(tr("Stop bits:"));
    gui->labelParity->setText(tr("Parity:"));
    gui->labelBaudrate->setText(tr("Baudrate:"));
    gui->labelFlowControl->setText(tr("Flow control:"));
}
void GuiController::setPropertiesSectionGeneral (void){
    gui->labelSectionGeneral->setText(tr("General"));
    gui->labelSectionGeneral->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTheme->setText(tr("Theme:"));
    gui->labelLanguage->setText(tr("Language:"));
    gui->labelCodec->setText(tr("Text codec:"));
    gui->labelCaptureTime->setText(tr("Capture time, ms:"));
    gui->labelCaptureBytes->setText(tr("Capture bytes:"));
    gui->buttonGeneralApply->setText(tr("Apply"));
    gui->spinBoxCaptureTime->setRange(0, 2147483647);
    gui->spinBoxCaptureBytes->setRange(0, 2147483647);
}
void GuiController::setPropertiesSectionConsole (void){
    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelSectionConsole->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelConsoleEcho->setText(tr("Echo:"));
    gui->checkboxConsoleEcho->setText("");

    gui->labelConsoleCyclic->setText(tr("Cyclic mode:"));
    gui->labelConsoleCyclicInterval->setText(tr("Interval, ms:"));
    gui->checkboxConsoleCyclic->setText("");
    gui->labelConsoleHotKey1->setText("1:");
    gui->labelConsoleHotKey2->setText("2:");
    gui->labelConsoleHotKey3->setText("3:");
    gui->labelConsoleHotKey4->setText("4:");
    gui->buttonConsoleHotKey1->setText(" ");
    gui->buttonConsoleHotKey2->setText(" ");
    gui->buttonConsoleHotKey3->setText(" ");
    gui->buttonConsoleHotKey4->setText(" ");
    gui->buttonConsoleHotKey1->setCheckable(true);
    gui->buttonConsoleHotKey2->setCheckable(true);
    gui->buttonConsoleHotKey3->setCheckable(true);
    gui->buttonConsoleHotKey4->setCheckable(true);
    gui->spinboxConsoleCyclicInterval->setRange(5, 10000);
}
void GuiController::setPropertiesSectionTable (void){
    gui->labelSectionTable->setText(tr("Table"));
    gui->labelSectionTable->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTableEcho->setText(tr("Echo:"));
    gui->checkboxTableEcho->setText("");

    gui->labelTableCyclicMode->setText(tr("Cyclic mode:"));
    gui->checkboxTableCyclic->setText("");
    gui->labelTableCyclicInterval->setText(tr("Interval, ms:"));
    gui->labelTableHotKey1->setText(tr("1:"));
    gui->labelTableHotKey2->setText(tr("2:"));
    gui->labelTableHotKey3->setText(tr("3:"));
    gui->labelTableHotKey4->setText(tr("4:"));
    gui->buttonTableHotKey1->setText(" ");
    gui->buttonTableHotKey2->setText(" ");
    gui->buttonTableHotKey3->setText(" ");
    gui->buttonTableHotKey4->setText(" ");
    gui->buttonTableHotKey1->setCheckable(true);
    gui->buttonTableHotKey2->setCheckable(true);
    gui->buttonTableHotKey3->setCheckable(true);
    gui->buttonTableHotKey4->setCheckable(true);

    gui->spinboxTableCyclicInterval->setRange(5, 10000);

    gui->lineEditTableHotKey1->setValidator(hexValidator);
    gui->lineEditTableHotKey2->setValidator(hexValidator);
    gui->lineEditTableHotKey3->setValidator(hexValidator);
    gui->lineEditTableHotKey4->setValidator(hexValidator);
}
void GuiController::setPropertiesSectionLogs (void){
    gui->labelSectionLog->setText(tr("Logs"));
    gui->labelSectionLog->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelLogEnable->setText(tr("Enable:"));
    gui->labelLogPath->setText(tr("Path:"));
    gui->lineEditLogPath->setReadOnly(true);
    gui->checkBoxLogEnable->setText("");
    gui->labelLogSpace->setText("Сolumn spacing:");
    gui->labelLogColomnSize->setText("Сolumn size, byte:");
    gui->buttonLogApply->setText(tr("Apply"));
    gui->spinBoxLogSpace->setRange(1, 50);
    gui->spinBoxLogColumnSize->setRange(1, 50);
}
void GuiController::setPropertiesSectionBinds(void){
    gui->bindsContentButton->hide();
}
void GuiController::setLanguageList(QStringList* list){
    gui->comboBoxLanguage->clear();
    gui->comboBoxLanguage->addItems(*list);
}
void GuiController::setCodecList(QStringList* list){
    gui->comboBoxCodec->clear();
    gui->comboBoxCodec->addItems(*list);
}
void GuiController::setThemeList(QStringList* list){
    gui->comboBoxTheme->clear();
    gui->comboBoxTheme->addItems(*list);
}
void GuiController::retranstate(void){
    gui->connectionContentsButton->setText(tr("Connection"));
    gui->generalContentButton->setText(tr("General"));
    gui->consoleContentButton->setText(tr("Console"));
    gui->tableContentButton->setText(tr("Table"));
    gui->logsContentButton->setText(tr("Logs"));
    gui->bindsContentButton->setText(tr("Binds"));
    gui->infoLabel->setText(tr("Version: 0.0.0"));

    gui->labelSectionConnection->setText(tr("Connection"));
    gui->labelPorts->setText(tr("Port:"));
    gui->labelData->setText(tr("Data:"));
    gui->labelStopBits->setText(tr("Stop bits:"));
    gui->labelParity->setText(tr("Parity:"));
    gui->labelBaudrate->setText(tr("Baudrate:"));
    gui->labelFlowControl->setText(tr("Flow control:"));

    gui->labelSectionGeneral->setText(tr("General"));
    gui->labelTheme->setText(tr("Theme:"));
    gui->labelLanguage->setText(tr("Language:"));
    gui->labelCodec->setText(tr("Text codec:"));

    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelConsoleEcho->setText(tr("Echo:"));

    gui->labelSectionTable->setText(tr("Table"));
    gui->labelTableEcho->setText(tr("Echo:"));
}
bool GuiController::eventFilter(QObject *target, QEvent *event){
    if(event->type() != QKeyEvent::KeyPress || static_cast<QKeyEvent*>(event)->isAutoRepeat())
        return QObject::eventFilter(target, event);

    switch (static_cast<QKeyEvent*>(event)->key()) {
    case Qt::Key_Return:
        qDebug() << "Enter";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            emit gui->sendConsoleButton->clicked();
            return true;
        case quickIndexTable:
            emit gui->sendTableButton->clicked();
            return true;
        case quickIndexConverter:
            emit gui->converterConvertButton->clicked();
            return true;
        }
    case Qt::Key_1:
        qDebug() << "Key_1";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey1->setChecked(gui->buttonConsoleHotKey1->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey1->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey1->setChecked(gui->buttonTableHotKey1->isChecked() ? false:true);
            emit gui->buttonTableHotKey1->clicked();
            return true;
        }
        break;
    case Qt::Key_2:
        qDebug() << "Key_2";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey2->setChecked(gui->buttonConsoleHotKey2->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey2->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey2->setChecked(gui->buttonTableHotKey2->isChecked() ? false:true);
            emit gui->buttonTableHotKey2->clicked();
            return true;
        }
        break;
    case Qt::Key_3:
        qDebug() << "Key_3";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey3->setChecked(gui->buttonConsoleHotKey3->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey3->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey3->setChecked(gui->buttonTableHotKey3->isChecked() ? false:true);
            emit gui->buttonTableHotKey3->clicked();
            return true;
        }
        break;
    case Qt::Key_4:
        qDebug() << "Key_4";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey4->setChecked(gui->buttonConsoleHotKey4->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey4->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey4->setChecked(gui->buttonTableHotKey4->isChecked() ? false:true);
            emit gui->buttonTableHotKey4->clicked();
            return true;
        }
        break;
    }
    return QObject::eventFilter(target, event);
}
void GuiController::consoleHotkeys(void){
    QString data;
    static QToolButton *button = nullptr;
    if(button != static_cast<QToolButton*>(QObject::sender())){
        if(button != nullptr)
            button->setChecked(false);
        button = static_cast<QToolButton*>(QObject::sender());
        if(button == gui->buttonConsoleHotKey1)
            data = gui->lineEditConsoleHotKey1->text();
        else if(button == gui->buttonConsoleHotKey2)
            data = gui->lineEditConsoleHotKey2->text();
        else if(button == gui->buttonConsoleHotKey3)
            data = gui->lineEditConsoleHotKey3->text();
        else if(button == gui->buttonConsoleHotKey4)
            data = gui->lineEditConsoleHotKey4->text();
        else
            return;
        emit consoleCyclicChanged(gui->checkboxConsoleCyclic->checkState() == Qt::CheckState::Checked ? true : false);
        emit consoleCyclicIntervalChanged(gui->spinboxConsoleCyclicInterval->value());
        emit consoleCyclicDataChanged(data);
        emit consoleStartCycle();
    }
    else {
        button = nullptr;
        emit consoleStopCycle();
    }
}
void GuiController::consoleCyclicStoped(void){
    gui->buttonConsoleHotKey1->setChecked(false);
    gui->buttonConsoleHotKey2->setChecked(false);
    gui->buttonConsoleHotKey3->setChecked(false);
    gui->buttonConsoleHotKey4->setChecked(false);
}
void GuiController::tableHotkeys(void){
    QString data;
    static QToolButton *button = nullptr;
    if(button != static_cast<QToolButton*>(QObject::sender())){
        if(button != nullptr)
            button->setChecked(false);
        button = static_cast<QToolButton*>(QObject::sender());
        if(button == gui->buttonTableHotKey1)
            data = gui->lineEditTableHotKey1->text();
        else if(button == gui->buttonTableHotKey2)
            data = gui->lineEditTableHotKey2->text();
        else if(button == gui->buttonTableHotKey3)
            data = gui->lineEditTableHotKey3->text();
        else if(button == gui->buttonTableHotKey4)
            data = gui->lineEditTableHotKey4->text();
        else
            return;
        emit tableCyclicChanged(gui->checkboxTableCyclic->checkState() == Qt::CheckState::Checked ? true : false);
        emit tableCyclicIntervalChanged(gui->spinboxTableCyclicInterval->value());
        emit tableCyclicDataChanged(data);
        emit tableStartCycle();
    }
    else {
        button = nullptr;
        emit tableStopCycle();
    }
}
void GuiController::tableCyclicStoped(void){
    gui->buttonTableHotKey1->setChecked(false);
    gui->buttonTableHotKey2->setChecked(false);
    gui->buttonTableHotKey3->setChecked(false);
    gui->buttonTableHotKey4->setChecked(false);
}
void GuiController::hexDelimiters(const QString& text){
    QLineEdit* lineEdit = static_cast<QLineEdit*>(QObject::sender());
    QString data = text;
    Converter::setDelimitersInHexString(data, 2, ' ');
    lineEdit->setText(data);
}
void GuiController::selectLogPath(void){
    QString previousPath = gui->lineEditLogPath->text();
    QString path = QFileDialog::getExistingDirectory(nullptr, tr("Select path"),
                                                     previousPath,
                                                     QFileDialog::ShowDirsOnly |
                                                     QFileDialog::DontResolveSymlinks);
    if(path == "")
        gui->lineEditLogPath->setText(previousPath);
    else
        gui->lineEditLogPath->setText(path);
    emit logPathChanged(gui->lineEditLogPath->text());
}
