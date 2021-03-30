#ifndef EDITPATTERNDIALOG_H
#define EDITPATTERNDIALOG_H

#include <QDialog>
#include "ledpattern.h"

namespace Ui {
class EditPatternDialog;
}

class EditPatternDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPatternDialog(QWidget *parent = nullptr, LEDPattern *pat = nullptr);
    ~EditPatternDialog();

private slots:
    void onButtonBoxRejected();
    void onButtonBoxAccepted();
    void onColorChange(QColor c);
    void on_onCheckBox_clicked(bool checked);
    void on_testPushButton_clicked();
    void on_resetColorsPushButton_clicked();

private:
    Ui::EditPatternDialog *ui;
    LEDPattern *pattern;
    LEDPattern tempPattern;
};

#endif // EDITPATTERNDIALOG_H
