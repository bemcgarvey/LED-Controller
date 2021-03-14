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
    char buffer[64];
    LEDController controller;
    QVector<OutputPanelDisplayManager *> outputDMs;
private slots:
    void updatePortMenu(void);
    void comPortSelected(void);
    void onReadyRead(void);
    void on_actionExit_triggered();
    void onMemoryUsedChanged(void);
    void on_actionAbout_triggered();
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
