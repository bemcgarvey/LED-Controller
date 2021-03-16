#include "editpatterndialog.h"
#include "patterndisplay.h"
#include <QBrush>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QHBoxLayout>
#include "mainwindow.h"

//Implement drag and drop

PatternDisplay::PatternDisplay(QWidget *parent) :
    QFrame(parent), pattern(nullptr), editable(false)
{

    leftButton = new QPushButton("<", this);
    rightButton = new QPushButton(">", this);
    connect(leftButton, &QPushButton::pressed, this, &PatternDisplay::onLeftButton);
    connect(rightButton, &QPushButton::pressed, this, &PatternDisplay::onRightButton);
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
        MainWindow *mw = dynamic_cast<MainWindow *>(nativeParentWidget());
        connect(dlg, &EditPatternDialog::testPattern, mw, &MainWindow::onTestRequest);
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
}

//TODO Scrolling should stop when right most led displayed
//Disable buttons when scrolled to one side
//Scroll in window sized jumps? or add >> and << buttons that do that?

void PatternDisplay::onLeftButton()
{
    if (leftIndex > 0) {
        --leftIndex;
        update();
    }
}

void PatternDisplay::onRightButton()
{
    ++leftIndex;
    update();
}
