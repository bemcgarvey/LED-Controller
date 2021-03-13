#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include "colorpicker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), port(nullptr)
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
    for (int i = 0; i < numOutputs; ++i) {
        outputs.append(new LEDOutputConfig(0, 1));
        connect(outputs[i], &LEDOutputConfig::sizeChanged, this, &MainWindow::onLEDOutputSizeChange);
    }
    outputDMs.append(new OutputPanelDisplayManager(outputs[0], ui->nLEDsSpinBox1, ui->nPatternsSpinBox1, ui->output1PatternLabelsFrame, ui->output1PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(outputs[1], ui->nLEDsSpinBox2, ui->nPatternsSpinBox2, ui->output2PatternLabelsFrame, ui->output2PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(outputs[2], ui->nLEDsSpinBox3, ui->nPatternsSpinBox3, ui->output3PatternLabelsFrame, ui->output3PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(outputs[3], ui->nLEDsSpinBox4, ui->nPatternsSpinBox4, ui->output4PatternLabelsFrame, ui->output4PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(outputs[4], ui->nLEDsSpinBox5, ui->nPatternsSpinBox5, ui->output5PatternLabelsFrame, ui->output5PatternsFrame));
    outputDMs.append(new OutputPanelDisplayManager(outputs[5], ui->nLEDsSpinBox6, ui->nPatternsSpinBox6, ui->output6PatternLabelsFrame, ui->output6PatternsFrame));
    onLEDOutputSizeChange(outputs[0]->sizeInBytes());
    ColorPicker::loadColors();
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < outputs.size(); ++i) {
        delete outputs[i];
    }
    for (int i = 0; i < outputDMs.size(); ++i) {
        delete outputDMs[i];
    }
    delete ui;
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
        connectedLabel->setText("Connected");
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

void MainWindow::onLEDOutputSizeChange(int newSize)
{
    Q_UNUSED(newSize)
    int sizeUsed = 0;
    for (auto &&i : outputs) {
        sizeUsed += i->sizeInBytes();
    }
    memoryLabel->setText(QString("Memory used: %1").arg(sizeUsed));
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    ColorPicker::saveColors();
    event->accept();
}
