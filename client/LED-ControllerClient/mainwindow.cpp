#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>
#include "colorpicker.h"
#include "aboutdialog.h"
#include "version.h"
#include <QDebug>

//TODO check all member functions.  Add const where appropriate.
//TODO Application icons

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), port(nullptr), pRxBuffer(nullptr)
    , modified(true), fileName(""), state(IDLE), bytesNeeded(0)
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
    ui->rcInComboBox1->setCurrentIndex(controller.getRCAction(0));
    ui->rcInComboBox2->setCurrentIndex(controller.getRCAction(1));
    ui->rcInComboBox3->setCurrentIndex(controller.getRCAction(2));
    ui->rcInComboBox4->setCurrentIndex(controller.getRCAction(3));
    ui->rcInComboBox5->setCurrentIndex(controller.getRCAction(4));
    ui->rcInComboBox6->setCurrentIndex(controller.getRCAction(5));
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
    controller.setRCAction(0, ui->rcInComboBox1->currentIndex());
    controller.setRCAction(1, ui->rcInComboBox2->currentIndex());
    controller.setRCAction(2, ui->rcInComboBox3->currentIndex());
    controller.setRCAction(3, ui->rcInComboBox4->currentIndex());
    controller.setRCAction(4, ui->rcInComboBox5->currentIndex());
    controller.setRCAction(5, ui->rcInComboBox6->currentIndex());
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
        ui->rcInComboBox1->setCurrentIndex(controller.getRCAction(0));
        ui->rcInComboBox2->setCurrentIndex(controller.getRCAction(1));
        ui->rcInComboBox3->setCurrentIndex(controller.getRCAction(2));
        ui->rcInComboBox4->setCurrentIndex(controller.getRCAction(3));
        ui->rcInComboBox5->setCurrentIndex(controller.getRCAction(4));
        ui->rcInComboBox6->setCurrentIndex(controller.getRCAction(5));
        for (auto&& i : outputDMs) {
            i->updateControls();
        }
        return true;
    } else {
        return false;
    }
}

void MainWindow::updatePortMenu()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    ui->menuPort->clear();
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
    }
    port = new QSerialPort(action->text(), this);
    if (port->open(QIODevice::ReadWrite)) {
        port->setBaudRate(QSerialPort::Baud115200);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        portLabel->setText(action->text());
        ui->connectPushButton->setEnabled(true);
        port->clear();  //clear out extra bytes from device power on
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
        //qDebug() << port->bytesAvailable() << ":" << bytesNeeded;
        switch (state) {
        case IDLE:
            port->clear(QSerialPort::Input);
            break;
        case WAIT_VERSION:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                Version::firmwareMajorVersion = tempBuffer[0];
                Version::firmwareMinorVersion = tempBuffer[1];
                bytesNeeded = 2;
                state = WAIT_MEM_SIZE;
            }
            break;
        case WAIT_MEM_SIZE:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                uint16_t mem = tempBuffer[2];
                mem |= (static_cast<uint16_t>(tempBuffer[3])) << 8;
                controller.setMaxMemory(mem);
                bytesNeeded = 0;
                bufferPos = nullptr;
                connectedLabel->setText("Connected");
                ui->readPushButton->setEnabled(true);
                ui->writePushButton->setEnabled(true);
                onMemoryUsedChanged();
                state = IDLE;   //This will change to WAIT_CONFIG_SIZE
            }
            break;
        case WAIT_CONFIG_SIZE:
            received = port->read(bufferPos, bytesNeeded);
            bytesNeeded -= received;
            bufferPos += received;
            if (bytesNeeded == 0) {
                rxSize = tempBuffer[0];
                rxSize |= (static_cast<uint16_t>(tempBuffer[1])) << 8;
                bytesNeeded = rxSize; // + 1;  //extra byte for checksum
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
                uint8_t sum = 0;
                for (int i = 0; i < rxSize; ++i) {
                    sum += pRxBuffer[i];
                }
                if (true) {  //TODO implement checksum //(sum ^ pRxBuffer[rxSize]) == 0) {
                    QVector<uint8_t> vec;
                    for (int i = 0; i < rxSize; ++i) {
                        vec.append(pRxBuffer[i]);
                    }
                    //TODO - roll this code and code from open() and constructor?? into a function
                    controller.fromByteVector(vec);
                    ui->rcInComboBox1->setCurrentIndex(controller.getRCAction(0));
                    ui->rcInComboBox2->setCurrentIndex(controller.getRCAction(1));
                    ui->rcInComboBox3->setCurrentIndex(controller.getRCAction(2));
                    ui->rcInComboBox4->setCurrentIndex(controller.getRCAction(3));
                    ui->rcInComboBox5->setCurrentIndex(controller.getRCAction(4));
                    ui->rcInComboBox6->setCurrentIndex(controller.getRCAction(5));
                    for (auto&& i : outputDMs) {
                        i->updateControls();
                    }
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
                    QMessageBox::information(this, "LED-Controller", "Operation completed successfully");
                } else {
                    QMessageBox::critical(this, "LED-Controller", "Operation failed");
                }
                state = IDLE;
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
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //TODO check for modified
    ColorPicker::saveColors();
    event->accept();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *dlg = new AboutDialog(this);
    dlg->exec();
    delete dlg;
}

void MainWindow::on_actionSave_As_triggered()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save As", fileName, "LED files (*.led)");
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
    QVector<uint8_t> vec;
    controller.toByteVector(vec);
    uint16_t size = vec.size();
    char cmd = CMD_WRITE;
    state = WAIT_ACK;
    bytesNeeded = 1;
    bufferPos = tempBuffer;
    port->write(&cmd, 1);
    port->write(reinterpret_cast<char *>(&size), 2);
    char sum = 0;
    for (int i = 0; i < vec.size(); ++i) {
        char byte;
        byte = vec[i];
        port->write(&byte, 1);
        sum += byte;
    }
    port->write(&sum, 1);
}

void MainWindow::on_readPushButton_clicked()
{
    bytesNeeded = 2;
    bufferPos = tempBuffer;
    state = WAIT_CONFIG_SIZE;
    char txBuff = CMD_READ;
    port->write(&txBuff, 1);
}

void MainWindow::onTestRequest(LEDPattern *pat, int output)
{
    //TODO implement test pattern
    qDebug() << "Test Request: " << output << ":" << pat->getNumLEDs();
}

void MainWindow::on_connectPushButton_clicked()
{
    bytesNeeded = 2;
    bufferPos = tempBuffer;
    state = WAIT_VERSION;
    char txBuff[] = {0x4d, 0x63};
    port->write(txBuff, 2);
}
