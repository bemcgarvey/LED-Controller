#include "editpatterndialog.h"
#include "patterndisplay.h"
#include <QBrush>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"

//Implement drag and drop

PatternDisplay::PatternDisplay(QWidget *parent) :
    QFrame(parent), pattern(nullptr), editable(false)
{

    leftButton = new QPushButton("<", this);
    leftSpeedButton = new QPushButton("<<", this);
    rightButton = new QPushButton(">", this);
    rightSpeedButton = new QPushButton(">>", this);
    connect(leftButton, &QPushButton::pressed, this, &PatternDisplay::onLeftButton);
    connect(leftSpeedButton, &QPushButton::pressed, this, &PatternDisplay::onLeftSpeedButton);
    connect(rightSpeedButton, &QPushButton::pressed, this, &PatternDisplay::onRightSpeedButton);
    connect(rightButton, &QPushButton::pressed, this, &PatternDisplay::onRightButton);
    QHBoxLayout *hBox = new QHBoxLayout(this);
    QVBoxLayout *vBoxLeft = new QVBoxLayout();
    QVBoxLayout *vBoxRight = new QVBoxLayout();
    hBox->setMargin(0);
    leftButton->setMaximumWidth(20);
    leftSpeedButton->setMaximumWidth(20);
    rightButton->setMaximumWidth(20);
    rightSpeedButton->setMaximumWidth(20);
    leftButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    leftSpeedButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    rightButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    rightSpeedButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    leftButton->setAutoRepeat(true);
    rightButton->setAutoRepeat(true);
    leftSpeedButton->setAutoRepeat(true);
    rightSpeedButton->setAutoRepeat(true);
    vBoxLeft->addWidget(leftButton);
    vBoxLeft->addWidget(leftSpeedButton);
    vBoxLeft->setSpacing(0);
    vBoxLeft->setMargin(0);
    vBoxRight->addWidget(rightButton);
    vBoxRight->addWidget(rightSpeedButton);
    vBoxRight->setSpacing(0);
    vBoxRight->setMargin(0);
    hBox->addLayout(vBoxLeft);
    hBox->insertStretch(-1, 1);
    hBox->addLayout(vBoxRight);
    setLayout(hBox);
    leftIndex = 0;
    selection = -1;
}

PatternDisplay::~PatternDisplay() {

}

void PatternDisplay::setPattern(LEDPattern *p)
{
    pattern = p;
}

void PatternDisplay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && editable) {
        int newSelection = ((event->x() - 20) / (ledSize + 5)) + leftIndex;
        if (newSelection < pattern->getNumLEDs()) {
            selection = newSelection;
            update();
            emit selectionChanged(selection);
        }
    }
}

void PatternDisplay::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!editable) {
        EditPatternDialog *dlg = new EditPatternDialog(this, pattern);
        dlg->exec();
        delete dlg;
        update();
    }
}

void PatternDisplay::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    if (pattern == nullptr) {
        return;
    }
    QPainter painter(this);
    QFont font;
    font.setPixelSize(vHeight / 2 - 5);
    painter.setFont(font);
    QBrush brush(Qt::black);
    for (int i = 0; (i + leftIndex) < pattern->getNumLEDs(); ++i) {
        if (editable && (i + leftIndex) == selection) {
            brush.setColor(Qt::lightGray);
            painter.setBrush(brush);
            painter.drawRect(18 + (ledSize + 5) * i, 2, ledSize + 2, vHeight-2);
        }
        brush.setColor((*pattern)[i + leftIndex].rgb());
        painter.setBrush(brush);
        painter.drawEllipse(20 + (ledSize + 5) * i, 2, ledSize, ledSize);
        painter.drawText(20 + (ledSize + 5) * i, vHeight - 2, QString::number(leftIndex + i + 1));
    }
    if (leftIndex == 0) {
        leftButton->setEnabled(false);
        leftSpeedButton->setEnabled(false);
    } else {
        leftButton->setEnabled(true);
        leftSpeedButton->setEnabled(true);
    }
    if (pattern->getNumLEDs() - leftIndex > ledsAcross) {
        rightButton->setEnabled(true);
        rightSpeedButton->setEnabled(true);
    } else {
        rightButton->setEnabled(false);
        rightSpeedButton->setEnabled(false);
    }
}

bool PatternDisplay::isEditable() const
{
    return editable;
}

void PatternDisplay::setEditable(bool value)
{
    editable = value;
}

void PatternDisplay::setSelectionColor(int r, int g, int b)
{
    (*pattern)[selection] = LEDrgb(r, g, b);
}

void PatternDisplay::setColor(int index, int r, int g, int b)
{
    (*pattern)[index] = LEDrgb(r, g, b);
}



void PatternDisplay::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    length = width();
    vHeight = height();
    ledSize = vHeight / 2;
    ledsAcross = (length - 40) / (ledSize + 5);
}

//TODO make some nice button icons

void PatternDisplay::onLeftButton()
{  
    --leftIndex;
    update();

}

void PatternDisplay::onRightButton()
{
    ++leftIndex;
    update();
}

void PatternDisplay::onLeftSpeedButton()
{
    leftIndex -= (ledsAcross - 1);
    if (leftIndex < 0) {
        leftIndex = 0;
    }
    update();
}

void PatternDisplay::onRightSpeedButton()
{
    leftIndex += (ledsAcross - 1);
    if (pattern->getNumLEDs() - leftIndex <= ledsAcross) {
        leftIndex = pattern->getNumLEDs() - ledsAcross;
    }
    update();
}
