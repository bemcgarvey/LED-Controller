#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include "colorpicker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), port(nullptr), output1(3, 1)
{
    ui->setupUi(this);
    portLabel = new QLabel();
    portLabel->setText("----");
    connectedLabel = new QLabel();
    connectedLabel->setText("Not Connected");
    memoryLabel = new QLabel("Memory used: 0");
    ui->statusbar->addPermanentWidget(portLabel);
    ui->statusbar->addWidget(connectedLabel);
    ui->statusbar->addWidget(new QLabel("     "));
    ui->statusbar->addWidget(memoryLabel);
    connect(ui->menuPort, &QMenu::aboutToShow, this, &MainWindow::updatePortMenu);
    output1Config = new LEDOutputConfig(1, 1);
    output1DM = new OutputPanelDisplayManager(output1Config, ui->nLEDsSpinBox, ui->nPatternsSpinBox, ui->output1PatternLabelsFrame, ui->output1PatternsFrame);
    connect(output1Config, &LEDOutputConfig::sizeChanged, this, &MainWindow::onLEDOutputSizeChange);
    onLEDOutputSizeChange(output1Config->sizeInBytes());
    ColorPicker::loadColors();
}

MainWindow::~MainWindow()
{
    delete output1DM;
    delete output1Config;
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
    memoryLabel->setText(QString("Memory used: %1").arg(newSize));
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    ColorPicker::saveColors();
    event->accept();
}
