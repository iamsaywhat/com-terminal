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
    connectQuickMenu();
    connectSettingsMenu();
    connectSectionConnection();
    connectSectionGeneral();
    connectSectionConsole();
    connectSectionTable();
    connectSectionLog();
    retranstate();
}
GuiController::~GuiController(){
    delete hexValidator;
}
void GuiController::connectQuickMenu(void){
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
}
void GuiController::connectSettingsMenu(void){
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
}
void GuiController::connectSectionConnection(void){

}
void GuiController::connectSectionGeneral(void){
    connect(gui->comboBoxTheme, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GuiController::generalSettingsChanged);
    connect(gui->comboBoxCodec, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GuiController::generalSettingsChanged);
    connect(gui->comboBoxLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GuiController::generalSettingsChanged);
    connect(gui->spinBoxCaptureTime, QOverload<int>::of(&QSpinBox::valueChanged), this, &GuiController::generalSettingsChanged);
    connect(gui->spinBoxCaptureBytes, QOverload<int>::of(&QSpinBox::valueChanged), this, &GuiController::generalSettingsChanged);
    connect(gui->buttonGeneralApply, &QPushButton::pressed, this, &GuiController::generalSettingsChanged);
}
void GuiController::connectSectionConsole(void){
    connect(gui->checkboxConsoleEcho, &QCheckBox::stateChanged,
            [=](int state){emit this->consoleEchoChanged(state == Qt::CheckState::Checked ? true:false);});
    connect(gui->buttonConsoleHotKey1, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey2, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey3, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey4, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
}
void GuiController::connectSectionTable(void){
    connect(gui->checkboxTableEcho, &QCheckBox::stateChanged,
            [=](int state){emit this->tableEchoChanged(state == Qt::CheckState::Checked ? true:false);});
    connect(gui->buttonTableHotKey1, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey2, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey3, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey4, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->lineEditTableHotKey1, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey2, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey3, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey4, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
}
void GuiController::connectSectionLog(void){
    connect(gui->checkBoxLogEnable, &QCheckBox::stateChanged, [=](int state){emit this->logEnableChanged(state == Qt::CheckState::Checked ? true:false);});
    connect(gui->buttonLogPath, &QToolButton::clicked, this, &GuiController::selectLogPath);
    connect(gui->buttonLogApply, &QPushButton::pressed, [=](){
            emit logColumnSizeChanged(gui->spinBoxLogColumnSize->value());
            emit logColumnSpacing(gui->spinBoxLogSpace->value());});
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
    gui->maximazeButton->setCheckable(true);
    gui->minimizeButton->setText("");
    gui->appName->setText("");
    gui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
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
    gui->labelSectionConnection->setFont(QFont("Arial", 14, QFont::Bold));
}
void GuiController::setPropertiesSectionGeneral (void){
    gui->labelSectionGeneral->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->spinBoxCaptureTime->setRange(0, 2147483647);
    gui->spinBoxCaptureBytes->setRange(0, 2147483647);
}
void GuiController::setPropertiesSectionConsole (void){
    gui->labelSectionConsole->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->checkboxConsoleEcho->setText("");
    gui->checkboxConsoleCyclic->setText("");
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
    gui->labelSectionTable->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->checkboxTableEcho->setText("");
    gui->checkboxTableCyclic->setText("");
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
    gui->labelSectionLog->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->lineEditLogPath->setReadOnly(true);
    gui->checkBoxLogEnable->setText("");
    gui->spinBoxLogSpace->setRange(1, 60);
    gui->spinBoxLogColumnSize->setRange(1, 60);
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
void GuiController::setTextSettingsMenu(void){
    gui->connectionContentsButton->setText(tr("Connection"));
    gui->generalContentButton->setText(tr("General"));
    gui->consoleContentButton->setText(tr("Console"));
    gui->tableContentButton->setText(tr("Table"));
    gui->logsContentButton->setText(tr("Logs"));
    gui->bindsContentButton->setText(tr("Binds"));
    gui->infoLabel->setText(tr("Version: 0.0.0"));
}
void GuiController::setTextSectionConnection(void){
    gui->labelSectionConnection->setText(tr("Connection"));
    gui->labelPorts->setText(tr("Port:"));
    gui->labelData->setText(tr("Data:"));
    gui->labelStopBits->setText(tr("Stop bits:"));
    gui->labelParity->setText(tr("Parity:"));
    gui->labelBaudrate->setText(tr("Baudrate:"));
    gui->labelFlowControl->setText(tr("Flow control:"));
}
void GuiController::setTextSectionGeneral(void){
    gui->labelSectionGeneral->setText(tr("General"));
    gui->labelTheme->setText(tr("Theme:"));
    gui->labelLanguage->setText(tr("Language:"));
    gui->labelCodec->setText(tr("Text codec:"));
    gui->labelCaptureTime->setText(tr("Capture time, ms:"));
    gui->labelCaptureBytes->setText(tr("Capture bytes:"));
    gui->buttonGeneralApply->setText(tr("Apply"));
}
void GuiController::setTextSectionConsole(void){
    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelConsoleEcho->setText(tr("Echo:"));
    gui->labelConsoleCyclic->setText(tr("Cyclic mode:"));
    gui->labelConsoleCyclicInterval->setText(tr("Interval, ms:"));
    gui->labelConsoleHotKey1->setText(tr("Hot key 1:"));
    gui->labelConsoleHotKey2->setText(tr("Hot key 2:"));
    gui->labelConsoleHotKey3->setText(tr("Hot key 3:"));
    gui->labelConsoleHotKey4->setText(tr("Hot key 4:"));
}
void GuiController::setTextSectionTable(void){
    gui->labelSectionTable->setText(tr("Table"));
    gui->labelTableEcho->setText(tr("Echo:"));
    gui->labelTableCyclicMode->setText(tr("Cyclic mode:"));
    gui->labelTableCyclicInterval->setText(tr("Interval, ms:"));
    gui->labelTableHotKey1->setText(tr("Hot key 1:"));
    gui->labelTableHotKey2->setText(tr("Hot key 2:"));
    gui->labelTableHotKey3->setText(tr("Hot key 3:"));
    gui->labelTableHotKey4->setText(tr("Hot key 4:"));
}
void GuiController::setTextSectionLogs(void){
    gui->labelSectionLog->setText(tr("Logs"));
    gui->labelLogEnable->setText(tr("Enable:"));
    gui->labelLogPath->setText(tr("Path:"));
    gui->labelLogSpace->setText(tr("Сolumn spacing:"));
    gui->labelLogColomnSize->setText(tr("Сolumn size, byte:"));
    gui->buttonLogApply->setText(tr("Apply"));
}
void GuiController::setTextSectionBinds(void){

}
void GuiController::retranstate(void){
    setTextSettingsMenu();
    setTextSectionConnection();
    setTextSectionGeneral();
    setTextSectionConsole();
    setTextSectionTable();
    setTextSectionLogs();
    setTextSectionBinds();
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
    QToolButton* button = static_cast<QToolButton*>(QObject::sender());
    button->setChecked(false);
//    gui->buttonTableHotKey2->setChecked(false);
//    gui->buttonTableHotKey3->setChecked(false);
 //   gui->buttonTableHotKey4->setChecked(false);
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
void GuiController::generalSettingsChanged(void){
    QComboBox* comboBoxes = static_cast<QComboBox*>(QObject::sender());
    QSpinBox*  spinBoxes = static_cast<QSpinBox*>(QObject::sender());
    QPushButton*  button = static_cast<QPushButton*>(QObject::sender());
    static bool themeChanged = false;
    static bool codecChanged = false;
    static bool languageChanged = false;
    static bool captureTimeChanged = false;
    static bool captureBytesChanged = false;

    static int themeIndex = gui->comboBoxTheme->currentIndex();
    static int codecIndex = gui->comboBoxCodec->currentIndex();
    static int languageIndex = gui->comboBoxLanguage->currentIndex();
    static int captureTime = gui->spinBoxCaptureTime->value();
    static int captureBytes = gui->spinBoxCaptureBytes->value();

    if(comboBoxes == gui->comboBoxTheme){
        if(themeIndex != gui->comboBoxTheme->currentIndex())
            themeChanged = true;
        else
            themeChanged = false;
    }
    else if(comboBoxes == gui->comboBoxCodec){
        if(codecIndex != gui->comboBoxCodec->currentIndex())
            codecChanged = true;
        else
            codecChanged = false;
    }
    else if(comboBoxes == gui->comboBoxLanguage){
        if(languageIndex != gui->comboBoxLanguage->currentIndex())
            languageChanged = true;
        else
            languageChanged = false;
    }
    else if(spinBoxes == gui->spinBoxCaptureTime){
        if(captureTime !=  gui->spinBoxCaptureTime->value())
            captureTimeChanged = true;
        else
            captureTimeChanged = false;
    }
    else if(spinBoxes == gui->spinBoxCaptureBytes){
        if(captureBytes !=  gui->spinBoxCaptureBytes->value())
            captureBytes = true;
        else
            captureBytes = false;
    }
    else if(button == gui->buttonGeneralApply){
        if(themeChanged){
            themeIndex = gui->comboBoxTheme->currentIndex();
            emit currentThemeChanged(themeIndex);
            themeChanged = false;
        }
        if(codecChanged){
            codecIndex = gui->comboBoxCodec->currentIndex();
            emit currentTextCodecChanged(codecIndex);
            codecChanged = false;
        }
        if(languageChanged){
            languageIndex = gui->comboBoxLanguage->currentIndex();
            emit currentLanguageChanged(languageIndex);
            languageChanged = false;
        }
        if(captureTimeChanged){
            captureTime = gui->spinBoxCaptureTime->value();
            emit captureTimeChanges(captureTime);
            captureTimeChanged = false;
        }
        if(captureBytesChanged){
            captureBytes = gui->spinBoxCaptureBytes->value();
            emit captureBytesChanges(captureBytes);
            captureBytes = false;
        }
    }
    if(themeChanged||codecChanged||languageChanged||captureTimeChanged||captureBytesChanged)
        gui->buttonGeneralApply->show();
    else
         gui->buttonGeneralApply->hide();
}
