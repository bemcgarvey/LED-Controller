#include "editpatterndialog.h"
#include "mainwindow.h"
#include "ui_editpatterndialog.h"

EditPatternDialog::EditPatternDialog(QWidget *parent, LEDPattern *pat) :
    QDialog(parent), ui(new Ui::EditPatternDialog), pattern(nullptr)
{
    ui->setupUi(this);
    if (pat != nullptr) {
        pattern = pat;
        tempPattern = *pattern;
        ui->patternDisplay->setPattern(&tempPattern);
        ui->patternDisplay->setEditable(true);
        if (pattern->getOnTime() == -1) {
            ui->onTimeSpinBox->setEnabled(false);
            ui->onCheckBox->setChecked(true);
        } else {
            ui->onTimeSpinBox->setEnabled(true);
            ui->onCheckBox->setChecked(false);
            ui->onTimeSpinBox->setValue(pattern->getOnTime() / 10.0);
        }
        ui->nextPattternComboBox->setCurrentIndex(pattern->getNextPattern());
    }
    onColorChange(ui->colorPickerFrame->getColor());
    connect(ui->patternDisplay, &PatternDisplay::selectionChanged, ui->colorPickerFrame, &ColorPicker::onPatternSelectionChange);
    connect(ui->patternDisplay, &PatternDisplay::rightClicked, ui->colorPickerFrame, &ColorPicker::onRightClickPattern);
    connect(ui->colorPickerFrame, &ColorPicker::colorChanged, this, &EditPatternDialog::onColorChange);
}

EditPatternDialog::~EditPatternDialog()
{
    delete ui;
}

void EditPatternDialog::onButtonBoxRejected()
{
    close();
}

void EditPatternDialog::onButtonBoxAccepted()
{
    tempPattern.setNextPattern(ui->nextPattternComboBox->currentIndex());
    if (ui->onCheckBox->isChecked()) {
        tempPattern.setOnTime(-1);
    } else {
        tempPattern.setOnTime(ui->onTimeSpinBox->value() * 10);
    }
    if (*pattern != tempPattern) {
        MainWindow *mw = dynamic_cast<MainWindow *>(nativeParentWidget());
        mw->onModified();
    }
    *pattern = tempPattern;
    close();
}

void EditPatternDialog::onColorChange(QColor c)
{
    ui->redLabel->setText(QString("Red:%1").arg(c.red()));
    ui->greenLabel->setText(QString("Green:%1").arg(c.green()));
    ui->blueLabel->setText(QString("Blue:%1").arg(c.blue()));
}


void EditPatternDialog::on_onCheckBox_clicked(bool checked)
{
        ui->onTimeSpinBox->setEnabled(!checked);
}

void EditPatternDialog::on_testPushButton_clicked()
{
    MainWindow *mw = dynamic_cast<MainWindow *>(nativeParentWidget());
    mw->onTestRequest(&tempPattern, ui->testOutputComboBox->currentIndex());
}

void EditPatternDialog::on_resetColorsPushButton_clicked()
{
    ui->colorPickerFrame->resetColors();
    ui->colorPickerFrame->update();
}
