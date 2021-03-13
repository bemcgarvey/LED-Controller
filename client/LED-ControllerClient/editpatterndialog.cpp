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
            ui->onCheckBox->setChecked(true);
        } else {
            ui->onTimeSpinBox->setValue(pattern->getOnTime() / 10.0);
        }
        ui->nextPattternComboBox->setCurrentIndex(pattern->getNextPattern());
    }
    connect(ui->patternDisplay, &LEDPatternDisplay::selectionChanged, ui->colorPickerFrame, &ColorPicker::onPatternSelectionChange);
}

EditPatternDialog::~EditPatternDialog()
{
    delete ui;
}

void EditPatternDialog::on_buttonBox_rejected()
{
    close();
}

void EditPatternDialog::on_buttonBox_accepted()
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
