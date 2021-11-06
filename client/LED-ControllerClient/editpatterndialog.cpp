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
        setPatternComboBox();
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
    tempPattern.setNextPattern(getSelectedPattern());
    if (ui->onCheckBox->isChecked()) {
        tempPattern.setOnTime(-1);
    } else {
        tempPattern.setOnTime(qRound(ui->onTimeSpinBox->value() * 10.0));
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

void EditPatternDialog::setPatternComboBox()
{
    uint8_t pat = pattern->getNextPattern() & 0xc0;
    uint8_t step = pattern->getNextPattern() & 0x3f;
    if (pat == LEDPattern::BOUNCE) {
        ui->nextPatternComboBox->setCurrentIndex(ui->nextPatternComboBox->findText("Bounce"));
        ui->stepSpinBox->setValue(step);
    } else if (pat == LEDPattern::ROTATE_OUT) {
        ui->nextPatternComboBox->setCurrentIndex(ui->nextPatternComboBox->findText("Rotate Out"));
        ui->stepSpinBox->setValue(step);
    } else if (pat == LEDPattern::ROTATE_IN) {
        ui->nextPatternComboBox->setCurrentIndex(ui->nextPatternComboBox->findText("Rotate In"));
        ui->stepSpinBox->setValue(step);
    } else {
        ui->nextPatternComboBox->setCurrentIndex(pattern->getNextPattern());
        ui->stepSpinBox->setValue(0);
    }
}

int EditPatternDialog::getSelectedPattern()
{
    if (ui->nextPatternComboBox->currentText() == "Bounce") {
        return LEDPattern::BOUNCE + ui->stepSpinBox->value();
    } else if (ui->nextPatternComboBox->currentText() == "Rotate Out") {
        return LEDPattern::ROTATE_OUT + ui->stepSpinBox->value();
    } else if (ui->nextPatternComboBox->currentText() == "Rotate In") {
        return LEDPattern::ROTATE_IN + ui->stepSpinBox->value();
    } else {
        return ui->nextPatternComboBox->currentIndex();
    }
}

void EditPatternDialog::on_nextPatternComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Bounce" || arg1 == "Rotate Out" || arg1 == "Rotate In") {
        ui->stepSpinBox->setEnabled(true);
    } else {
        ui->stepSpinBox->setEnabled(false);
    }
}

