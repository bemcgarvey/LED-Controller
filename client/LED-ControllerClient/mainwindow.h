#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>
#include "ledoutput.h"
#include "outputpaneldisplaymanager.h"
#include "ledcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *portLabel;
    QLabel *connectedLabel;
    QLabel *memoryLabel;
    QSerialPort *port;
    char *pRxBuffer;
    uint16_t rxSize;
    LEDController controller;
    QVector<OutputPanelDisplayManager *> outputDMs;
    bool modified;
    QString fileName;
    bool save(void);
    bool open(QString fileName);
    void updateControls(void);
    enum RxState {IDLE, WAIT_VERSION, WAIT_MEM_SIZE, WAIT_CONFIG_SIZE, WAIT_CONFIG, WAIT_ACK};
    RxState state;
    int bytesNeeded;
    char *bufferPos;
    char tempBuffer[4];
    enum DeviceCommands {CMD_READ = 0x80, CMD_WRITE = 0x81, CMD_TEST = 0x82};
    enum DeviceResponse {ACK = 0x06, NACK = 0x15};
private slots:
    void updatePortMenu(void);
    void comPortSelected(void);
    void onReadyRead(void);
    void on_actionExit_triggered();
    void onMemoryUsedChanged(void);
    void on_actionAbout_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_writePushButton_clicked();
    void on_readPushButton_clicked();
    void on_connectPushButton_clicked();
public slots:
    void onTestRequest(LEDPattern *pat, int output);
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
