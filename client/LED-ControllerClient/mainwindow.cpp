#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>
#include "colorpicker.h"
#include "aboutdialog.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), port(nullptr), pRxBuffer(nullptr)
    , fileName(""), state(IDLE), bytesNeeded(0)
    , bufferPos(nullptr)
{
    ui->setupUi(this);
    portLabel = new QLabel("----");
    connectedLabel = new QLabel("Not Connected");
    memoryLabel = new QLabel("Memory used: 0");
    ui->statusbar->addPermanentWidget(portLabel);
    ui->statusbar->addWidget(connectedLabel);
    ui->statusbar->addWidget(new QLabel("     "));
    ui->statusbar->addWidget(memoryLabel);
    connect(ui->menuPort, &QMenu::aboutToShow, this, &MainWindow::updatePortMenu);
    outputDMs.append(new OutputPanelDisplayManager(&(controller[0]), ui->nLEDsSpinBox1, ui->nPatternsSpinBox1, ui->output1PatternLabelsFrame, ui->output1PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(&(controller[1]), ui->nLEDsSpinBox2, ui->nPatternsSpinBox2, ui->output2PatternLabelsFrame, ui->output2PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(&(controller[2]), ui->nLEDsSpinBox3, ui->nPatternsSpinBox3, ui->output3PatternLabelsFrame, ui->output3PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(&(controller[3]), ui->nLEDsSpinBox4, ui->nPatternsSpinBox4, ui->output4PatternLabelsFrame, ui->output4PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(&(controller[4]), ui->nLEDsSpinBox5, ui->nPatternsSpinBox5, ui->output5PatternLabelsFrame, ui->output5PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(&(controller[5]), ui->nLEDsSpinBox6, ui->nPatternsSpinBox6, ui->output6PatternLabelsFrame, ui->output6PatternsFrame));
    connect(outputDMs[0], &OutputPanelDisplayManager::sizeChanged, this, &MainWindow::onMemoryUsedChanged);
    connect(outputDMs[1], &OutputPanelDisplayManager::sizeChanged, this, &MainWindow::onMemoryUsedChanged);
    connect(outputDMs[2], &OutputPanelDisplayManager::sizeChanged, this, &MainWindow::onMemoryUsedChanged);
    connect(outputDMs[3], &OutputPanelDisplayManager::sizeChanged, this, &MainWindow::onMemoryUsedChanged);
    connect(outputDMs[4], &OutputPanelDisplayManager::sizeChanged, this, &MainWindow::onMemoryUsedChanged);
    connect(outputDMs[5], &OutputPanelDisplayManager::sizeChanged, this, &MainWindow::onMemoryUsedChanged);
    onMemoryUsedChanged();
    ColorPicker::loadColors();
    updateControls();
    modified = false;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimeout);
    ui->rcInComboBox1->setStyleSheet("background-color: lightgreen");
    QStringList c = QCoreApplication::arguments();
    if (c.size() > 1) {
        open(c.at(1));
    }
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < outputDMs.size(); ++i) {
        delete outputDMs[i];
    }
    delete ui;
}

bool MainWindow::save()
{
    getActionControls();
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QVector<uint8_t> bytes;
        controller.toByteVector(bytes);
        char *writeBuffer;
        writeBuffer = new char[bytes.size()];
        for (int i = 0; i < bytes.size(); ++i) {
            writeBuffer[i] = bytes[i];
        }
        uint16_t size = bytes.size();
        const uint32_t magic = 0x4D630302;
        file.write(reinterpret_cast<const char *>(&magic), 4);
        char majorVersion = Version::clientMajorVersion;
        char minorVersion = Version::clientMinorVersion;
        file.write(&majorVersion, 1);
        file.write(&minorVersion, 1);
        file.write(reinterpret_cast<char *>(&size), 2);
        file.write(writeBuffer, bytes.size());
        file.flush();
        file.close();
        delete[] writeBuffer;
        modified = false;
        setWindowTitle("RC LED Editor  [" + file.fileName() + "]");
        return true;
    } else {
        return false;
    }
}

bool MainWindow::open(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        uint32_t magic;
        file.read(reinterpret_cast<char *>(&magic), 4);
        if (magic != 0x4D630302) {
            QMessageBox::critical(this, "LED-Controller", "Invalid file format");
            file.close();
            return false;
        }
        char majorVersion;
        char minorVersion;
        file.read(&majorVersion, 1);
        file.read(&minorVersion, 1);  //Can use version if format changes later
        uint16_t size;
        file.read(reinterpret_cast<char *>(&size), 2);
        QVector<uint8_t> data;
        char *readBuffer;
        readBuffer = new char[size];
        file.read(readBuffer, size);
        for (int i = 0; i < size; ++i) {
            data.append(readBuffer[i]);
        }
        controller.fromByteVector(data);
        file.close();
        delete[] readBuffer;
        updateControls();
        modified = false;
        setWindowTitle("RC LED Editor  [" + file.fileName() + "]");
        return true;
    } else {
        setWindowTitle("RC LED Editor");
        return false;
    }
}

void MainWindow::updateControls()
{
    ui->rcInComboBox1->setCurrentIndex(controller.getRCAction(0));
    ui->rcInComboBox2->setCurrentIndex(controller.getRCAction(1));
    ui->rcInComboBox3->setCurrentIndex(controller.getRCAction(2));
    ui->rcInComboBox4->setCurrentIndex(controller.getRCAction(3));
    ui->rcInComboBox5->setCurrentIndex(controller.getRCAction(4));
    ui->rcInComboBox6->setCurrentIndex(controller.getRCAction(5));
    for (auto&& i : outputDMs) {
        i->updateControls();
    }
}

void MainWindow::getActionControls()
{
    controller.setRCAction(0, ui->rcInComboBox1->currentIndex());
    controller.setRCAction(1, ui->rcInComboBox2->currentIndex());
    controller.setRCAction(2, ui->rcInComboBox3->currentIndex());
    controller.setRCAction(3, ui->rcInComboBox4->currentIndex());
    controller.setRCAction(4, ui->rcInComboBox5->currentIndex());
    controller.setRCAction(5, ui->rcInComboBox6->currentIndex());
}

void MainWindow::clearRCComboBoxStyle()
{
    ui->rcInComboBox1->setStyleSheet("");
    ui->rcInComboBox2->setStyleSheet("");
    ui->rcInComboBox3->setStyleSheet("");
    ui->rcInComboBox4->setStyleSheet("");
    ui->rcInComboBox5->setStyleSheet("");
    ui->rcInComboBox6->setStyleSheet("");
}

void MainWindow::updatePortMenu()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    ui->menuPort->clear();
    if (portList.size() == 0) {
        ui->menuPort->addAction("No ports found");
        if (port != nullptr) {
            port->close();
            delete port;
        }
        port = nullptr;
        portLabel->setText("----");
        connectedLabel->setText("Not Connected");
        return;
    }
    for (auto&& i : portList) {
        QAction * action = ui->menuPort->addAction(i.portName(), this, &MainWindow::comPortSelected);
        action->setCheckable(true);
        if (i.portName() == portLabel->text()) {
            action->setChecked(true);
        }
    }
}

void MainWindow::comPortSelected()
{
    QAction *action = dynamic_cast<QAction *>(sender());
    if (port != nullptr) {
        port->close();
        delete port;
        port = nullptr;
    }
    port = new QSerialPort(action->text(), this);
    if (port->open(QIODevice::ReadWrite)) {
        port->setBaudRate(QSerialPort::Baud115200);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        portLabel->setText(action->text());
        ui->connectPushButton->setEnabled(true);
        port->clear(QSerialPort::AllDirections);  //clear out extra bytes from device power on
        connect(port, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
    } else {
        delete port;
        port = nullptr;
        portLabel->setText("----");
        connectedLabel->setText("Not Connected");
    }
}

void MainWindow::onReadyRead(void) {
    int received;
    while (port->bytesAvailable() > 0) {
        switch (state) {
        case IDLE:
            port->clear(QSerialPort::Input);
            break;
        case WAIT_VERSION:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                if (tempBuffer[0] != CMD_START1 || tempBuffer[1] != CMD_START2) {
                    state = IDLE;
                    port->clear(QSerialPort::AllDirections);
                    ui->statusbar->showMessage("Unable to connect", 1000);
                    return;
                }
                Version::firmwareMajorVersion = tempBuffer[2];
                Version::firmwareMinorVersion = tempBuffer[3];
                uint16_t mem = *(reinterpret_cast<uint16_t*>(&tempBuffer[4]));
                controller.setMaxMemory(mem);
                bytesNeeded = 0;
                bufferPos = nullptr;
                connectedLabel->setText("Connected");
                ui->readPushButton->setEnabled(true);
                ui->writePushButton->setEnabled(true);
                ui->resetPushButton->setEnabled(true);
                ui->monitorRCPushButton->setEnabled(true);
                onMemoryUsedChanged();
                state = IDLE;
            }
            break;
        case WAIT_CONFIG_SIZE:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                rxSize = *(reinterpret_cast<uint16_t *>(tempBuffer));
                bytesNeeded = rxSize + 1;  //extra byte for checksum
                pRxBuffer = new char[bytesNeeded];
                bufferPos = pRxBuffer;
                state = WAIT_CONFIG;
            }
            break;
        case WAIT_CONFIG:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                //Checksum algorithm - add bytes with carry
                uint8_t sum = 0;
                for (int i = 0; i < rxSize; ++i) {
                    uint8_t carry = ((sum + (uint8_t)pRxBuffer[i]) >> 8) & 1;
                    sum += (uint8_t)pRxBuffer[i] + carry;
                }
                sum += (rxSize / 256);  //No idea why this is needed
                sum += pRxBuffer[rxSize];
                if (sum == 0) {
                    QVector<uint8_t> vec;
                    for (int i = 0; i < rxSize; ++i) {
                        vec.append(pRxBuffer[i]);
                    }
                    controller.fromByteVector(vec);
                    updateControls();
                    ui->statusbar->showMessage("Read successful", 1000);
                    for (auto&& i : findChildren<PatternDisplay *>()) {
                        i->update();
                    }
                    modified = false;
                } else {
                    QMessageBox::critical(this, "LED-Controller", "Error reading device");
                }
                delete[] pRxBuffer;
                state = IDLE;
            }
            break;
        case WAIT_ACK:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                if (tempBuffer[0] == ACK) {
                    ui->statusbar->showMessage("Write to device successful", 2000);
                } else {
                    QMessageBox::critical(this, "LED-Controller", "Write to device failed");
                }
                state = IDLE;
            }
            break;
        case WAIT_RC_US:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                int rcMicroSeconds = *(reinterpret_cast<int16_t*>(tempBuffer));
                showRCValue(rcMicroSeconds);
            }
            break;
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::onMemoryUsedChanged(void)
{
    if (controller.getMaxMemory() > 0) {
        memoryLabel->setText(QString("Memory used: %1 of %2").arg(controller.sizeInBytes())
                             .arg(controller.getMaxMemory()));
    } else {
        memoryLabel->setText(QString("Memory used: %1 of ...")
                             .arg(controller.sizeInBytes()));
    }
    modified = true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (modified) {
        if (QMessageBox::warning(this, "LED-Controller"
                                 , "You have unsaved changes. Are you sure you want to quite?"
                                 , QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {

            ColorPicker::saveColors();
            event->accept();
        } else {
            event->ignore();
            return;
        }
    }
    if (ColorPicker::getColorsChanged()) {
        if (QMessageBox::warning(this, "LED-Controller"
                                 , "Color pallette has been modified.  Save it?"
                                 , QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {

            ColorPicker::saveColors();
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *dlg = new AboutDialog(this);
    dlg->exec();
    delete dlg;
}

void MainWindow::on_actionSave_As_triggered()
{
    QSettings settings;
    QString lastFileName = settings.value("lastfile", "").toString();
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save As", lastFileName, "LED files (*.led)");
    if (saveFileName.length() > 0) {
        fileName = saveFileName;
        if (!save()) {
            QMessageBox::critical(this, "LED Controller", "Error saving file.  Changes are not saved.");
        } else {
            modified = false;
            QSettings settings;
            settings.setValue("lastfile", fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (fileName.length() > 0) {
        if (!save()) {
            QMessageBox::critical(this, "LED Controller", "Error saving file.  Changes are not saved.");
        } else {
            modified = false;
        }
    } else {
        on_actionSave_As_triggered();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QSettings settings;
    QString lastFileName = settings.value("lastfile", "").toString();
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", lastFileName, "LED files (*.led)");
    if (openFileName.length() > 0) {
        if (open(openFileName)) {
            modified = false;
            fileName = openFileName;
            settings.setValue("lastfile", fileName);
        } else {
            QMessageBox::critical(this, "LED Controller", "Could not open file.");
        }
    }
}

void MainWindow::on_writePushButton_clicked()
{
    getActionControls();
    QVector<uint8_t> vec;
    controller.toByteVector(vec);
    uint16_t size = vec.size();
    char cmd = CMD_WRITE;
    state = WAIT_ACK;
    bytesNeeded = 1;
    bufferPos = tempBuffer;
    port->write(&cmd, 1);
    port->write(reinterpret_cast<char *>(&size), 2);
    uint8_t sum = 0;
    for (int i = 0; i < vec.size(); ++i) {
        uint8_t carry = (sum + vec[i]) >> 8;
        sum += vec[i] + carry;
        port->write(reinterpret_cast<char *>(&vec[i]), 1);
    }
    sum = -sum;
    port->write(reinterpret_cast<char *>(&sum), 1);
}

void MainWindow::on_readPushButton_clicked()
{
    bytesNeeded = 2;
    bufferPos = tempBuffer;
    state = WAIT_CONFIG_SIZE;
    char cmd = CMD_READ;
    port->write(&cmd, 1);
}

void MainWindow::onTestRequest(LEDPattern *pat, int output)
{
    if (!ui->writePushButton->isEnabled()) {
        QMessageBox::critical(this, "LED-Controller", "Not connected!");
        return;
    }
    QVector<uint8_t> vec;
    pat->toByteVector(vec);
    char cmd = CMD_TEST;
    state = WAIT_ACK;
    bytesNeeded = 1;
    bufferPos = tempBuffer;
    port->write(&cmd, 1);
    uint16_t size = vec.size();
    port->write(reinterpret_cast<char *>(&size), 2);
    char out = output;
    port->write(&out, 1);
    uint8_t sum = 0;
    for (int i = 0; i < vec.size(); ++i) {
        uint8_t carry = (sum + vec[i]) >> 8;
        sum += vec[i] + carry;
        port->write(reinterpret_cast<char *>(&vec[i]), 1);
    }
    sum = -sum;
    port->write(reinterpret_cast<char *>(&sum), 1);
}

void MainWindow::onModified()
{
    modified = true;
}

void MainWindow::on_connectPushButton_clicked()
{
    ui->readPushButton->setEnabled(false);
    ui->writePushButton->setEnabled(false);
    ui->resetPushButton->setEnabled(false);
    ui->monitorRCPushButton->setEnabled(false);
    connectedLabel->setText("Not Connected");
    port->clear();
    bytesNeeded = 6;
    tempBuffer[0] = tempBuffer[1] = 0;
    bufferPos = tempBuffer;
    state = WAIT_VERSION;
    char cmd[] = {CMD_START1, CMD_START2};
    port->write(cmd, 2);
}

void MainWindow::on_resetPushButton_clicked()
{
    ui->readPushButton->setEnabled(false);
    ui->writePushButton->setEnabled(false);
    ui->resetPushButton->setEnabled(false);
    ui->monitorRCPushButton->setEnabled(false);
    connectedLabel->setText("Not Connected");
    char cmd = CMD_RESET;
    port->write(&cmd, 1);
}

void MainWindow::on_monitorRCPushButton_toggled(bool checked)
{
    if (checked) {
        ui->connectPushButton->setEnabled(false);
        ui->readPushButton->setEnabled(false);
        ui->writePushButton->setEnabled(false);
        ui->resetPushButton->setEnabled(false);
        onTimeout();
        timer->start(100);
    } else {
        timer->stop();
        clearRCComboBoxStyle();
        ui->connectPushButton->setEnabled(true);
        ui->readPushButton->setEnabled(true);
        ui->writePushButton->setEnabled(true);
        ui->resetPushButton->setEnabled(true);
    }
}

void MainWindow::showRCValue(int us)
{
    clearRCComboBoxStyle();
    if (us < 1151) {
        ui->rcInComboBox1->setStyleSheet("background-color: lightgreen");
    } else if (us < 1351) {
        ui->rcInComboBox2->setStyleSheet("background-color: lightgreen");
    } else if (us < 1551) {
        ui->rcInComboBox3->setStyleSheet("background-color: lightgreen");
    } else if (us < 1751) {
        ui->rcInComboBox4->setStyleSheet("background-color: lightgreen");
    } else if (us < 1951) {
        ui->rcInComboBox5->setStyleSheet("background-color: lightgreen");
    } else {
        ui->rcInComboBox6->setStyleSheet("background-color: lightgreen");
    }
}

void MainWindow::onTimeout()
{
    bytesNeeded = 2;
    bufferPos = tempBuffer;
    state = WAIT_RC_US;
    char cmd = CMD_MONITOR_RC;
    port->write(&cmd, 1);
}
