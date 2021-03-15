#include "editpatterndialog.h"
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
            //FIXME need to work on how the ontime interacts with the checkbox.
            //Maybe disable spinbox when checked? and set to -1 (need to change limits to do this)
            ui->onCheckBox->setChecked(true);
        } else {
            ui->onTimeSpinBox->setValue(pattern->getOnTime() / 10.0);
        }
        ui->nextPattternComboBox->setCurrentIndex(pattern->getNextPattern());
    }
    onColorChange(ui->colorPickerFrame->getColor());
    connect(ui->patternDisplay, &PatternDisplay::selectionChanged, ui->colorPickerFrame, &ColorPicker::onPatternSelectionChange);
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
    *pattern = tempPattern;
    pattern->setNextPattern(ui->nextPattternComboBox->currentIndex());
    if (ui->onCheckBox->isChecked()) {
        pattern->setOnTime(-1);
    } else {
        pattern->setOnTime(ui->onTimeSpinBox->value() * 10);
    }
    close();
}

void EditPatternDialog::onColorChange(QColor c)
{
    ui->redLabel->setText(QString("Red:%1").arg(c.red()));
    ui->greenLabel->setText(QString("Green:%1").arg(c.green()));
    ui->blueLabel->setText(QString("Blue:%1").arg(c.blue()));
}

