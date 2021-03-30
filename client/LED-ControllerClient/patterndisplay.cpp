#include "editpatterndialog.h"
#include "patterndisplay.h"
#include <QBrush>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include "mainwindow.h"

PatternDisplay::PatternDisplay(QWidget *parent) :
    QFrame(parent), pattern(nullptr), editable(false), dragPixmap(nullptr)
{
    QIcon lIcon(":/images/leftButton.png");
    leftButton = new QPushButton(lIcon, "", this);
    QIcon lsIcon(":/images/leftSpeedButton.png");
    leftSpeedButton = new QPushButton(lsIcon, "", this);
    QIcon rIcon(":/images/rightButton.png");
    rightButton = new QPushButton(rIcon, "", this);
    QIcon rsIcon(":/images/rightSpeedButton.png");
    rightSpeedButton = new QPushButton(rsIcon, "", this);
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
    leftButton->setFocusPolicy(Qt::NoFocus);
    rightButton->setFocusPolicy(Qt::NoFocus);
    leftSpeedButton->setFocusPolicy(Qt::NoFocus);
    rightSpeedButton->setFocusPolicy(Qt::NoFocus);
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
    setAcceptDrops(true);
    dragSource = false;
    setMaximumHeight(30);
    popupMenu = new QMenu(this);
    popupMenu->addAction("Fill");
    popupMenu->addAction("Gradient Fill");
}

PatternDisplay::~PatternDisplay() {
    delete popupMenu;
}

void PatternDisplay::setPattern(LEDPattern *p)
{
    pattern = p;
}

void PatternDisplay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (editable) {
            int newSelection = ((event->x() - 20) / (ledSize + 5)) + leftIndex;
            if (newSelection < pattern->getNumLEDs()) {
                selection = newSelection;
                update();
                emit selectionChanged(selection);
            }
        }
        dragStartPos = event->pos();
    } else if (event->button() == Qt::RightButton) {
        if (editable) {
            int newSelection = ((event->x() - 20) / (ledSize + 5)) + leftIndex;
            int currentSelection = selection;
            if (currentSelection < 0) {
                currentSelection = 0;
            }
            if (newSelection < pattern->getNumLEDs()) {
                QAction *action = popupMenu->exec(QCursor::pos());
                if (action == nullptr) {
                    return;
                }
                if (action->text() == "Fill") {
                    emit rightClicked(currentSelection, newSelection, FILL);
                } else if (action->text() == "Gradient Fill") {
                    emit rightClicked(currentSelection, newSelection, GRADIENT);
                }
            }
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
    if (dragPixmap) {
        delete dragPixmap;
    }
    dragPixmap = new QPixmap(length, vHeight);
    dragPixmap->fill(Qt::transparent);
    QPainter dpPainter(dragPixmap);
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
        dpPainter.setBrush(brush);
        painter.drawEllipse(20 + (ledSize + 5) * i, 2, ledSize, ledSize);
        dpPainter.drawEllipse(20 + (ledSize + 5) * i, 2, ledSize, ledSize);
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
    if (editable) {
        setAcceptDrops(false);
    } else {
        setAcceptDrops(true);
    }
}

void PatternDisplay::setSelectionColor(int r, int g, int b)
{
    (*pattern)[selection] = LEDrgb(r, g, b);
}

void PatternDisplay::setColor(int index, int r, int g, int b)
{
    (*pattern)[index] = LEDrgb(r, g, b);
}

QColor PatternDisplay::getColor(int index)
{
    if (index < pattern->getNumLEDs()) {
        return (*pattern)[index].rgb();
    } else {
        return Qt::black;
    }
}



void PatternDisplay::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    length = width();
    vHeight = height();
    ledSize = vHeight / 2;
    ledsAcross = (length - 40) / (ledSize + 5);
}

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

void PatternDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if (editable) {
        return;
    }
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    QByteArray bytes;
    pattern->toByteArray(bytes);
    mimeData->setData("application/x-led_pattern", bytes);
    drag->setMimeData(mimeData);
    drag->setPixmap(*dragPixmap);
    drag->setHotSpot(QPoint(20, 0));
    dragSource = true;
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
    if (dropAction != Qt::IgnoreAction) {
        MainWindow *mw = dynamic_cast<MainWindow *>(nativeParentWidget());
        mw->onModified();
    }
    dragSource = false;
}


void PatternDisplay::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-led_pattern") && !dragSource) {
        event->acceptProposedAction();
    }
}

void PatternDisplay::dropEvent(QDropEvent *event)
{
    pattern->fromByteArray(event->mimeData()->data("application/x-led_pattern"));
    event->acceptProposedAction();
    update();
}
