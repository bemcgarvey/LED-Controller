#include "editpatterndialog.h"
#include "ledpatterndisplay.h"
#include <QBrush>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QHBoxLayout>

LEDPatternDisplay::LEDPatternDisplay(QWidget *parent) :
    QFrame(parent), pattern(nullptr), editable(false)
{

    leftButton = new QPushButton("<", this);
    rightButton = new QPushButton(">", this);
    connect(leftButton, &QPushButton::pressed, this, &LEDPatternDisplay::onLeftButton);
    connect(rightButton, &QPushButton::pressed, this, &LEDPatternDisplay::onRightButton);
    QHBoxLayout *hBox = new QHBoxLayout(this);
    hBox->setMargin(0);
    leftButton->setMaximumWidth(20);
    rightButton->setMaximumWidth(20);
    leftButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    rightButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    leftButton->setAutoRepeat(true);
    rightButton->setAutoRepeat(true);
    hBox->addWidget(leftButton);
    hBox->insertStretch(-1, 1);
    hBox->addWidget(rightButton);
    setLayout(hBox);
    leftIndex = 0;
    selection = -1;
}

LEDPatternDisplay::~LEDPatternDisplay() {

}

void LEDPatternDisplay::setPattern(LEDPattern *p)
{
    pattern = p;
}

void LEDPatternDisplay::mousePressEvent(QMouseEvent *event)
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

void LEDPatternDisplay::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!editable) {
        EditPatternDialog *dlg = new EditPatternDialog(this, pattern);
        dlg->exec();
        delete dlg;
        update();
    }
}

void LEDPatternDisplay::paintEvent(QPaintEvent *event)
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
}

bool LEDPatternDisplay::isEditable() const
{
    return editable;
}

void LEDPatternDisplay::setEditable(bool value)
{
    editable = value;
}

void LEDPatternDisplay::setSelectionColor(int r, int g, int b)
{
    (*pattern)[selection] = LEDrgb(r, g, b);
}

void LEDPatternDisplay::setColor(int index, int r, int g, int b)
{
    (*pattern)[index] = LEDrgb(r, g, b);
}



void LEDPatternDisplay::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    length = width();
    vHeight = height();
    ledSize = vHeight / 2;
}

//TODO Scrolling should stop when right most led displayed
//Disable buttons when scrolled to one side
//Scroll in window sized jumps? or add >> and << buttons that do that?

void LEDPatternDisplay::onLeftButton()
{
    if (leftIndex > 0) {
        --leftIndex;
        update();
    }
}

void LEDPatternDisplay::onRightButton()
{
    ++leftIndex;
    update();
}
