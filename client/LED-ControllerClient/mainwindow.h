#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>
#include "ledoutputconfig.h"
#include "outputpaneldisplaymanager.h"

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
    char buffer[64];
    LEDOutputConfig output1;
    OutputPanelDisplayManager *output1DM;
    LEDOutputConfig *output1Config;
private slots:
    void updatePortMenu(void);
    void comPortSelected(void);
    void onReadyRead(void);
    void on_actionExit_triggered();
    void onLEDOutputSizeChange(int newSize);
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
