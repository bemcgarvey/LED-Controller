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

//TODO implement device read/write

//TODO check all member functions.  Add const where appropriate.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), port(nullptr), modified(true), fileName("")
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
        //connectedLabel->setText("Connected");
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
    int bytesReceived;
    if (port->bytesAvailable() < 3) {
        return;
    }
    bytesReceived = static_cast<int>(port->read(buffer, 64));
    buffer[bytesReceived] = '\0';
    ui->statusbar->showMessage(buffer, 2000);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::onMemoryUsedChanged(void)
{
    memoryLabel->setText(QString("Memory used: %1").arg(controller.sizeInBytes()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
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

}

void MainWindow::on_readPushButton_clicked()
{

}

void MainWindow::onTestRequest(LEDPattern *pat, int output)
{
    //TODO implement test pattern
    qDebug() << "Test Request: " << output << ":" << pat->getNumLEDs();
}
