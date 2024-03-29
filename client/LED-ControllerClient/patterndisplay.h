#ifndef LEDPATTERNDISPLAY_H
#define LEDPATTERNDISPLAY_H

#include <QFrame>
#include <QPoint>
#include <QPushButton>
#include "ledpattern.h"

class OutputPanelDisplayManager;

class PatternDisplay : public QFrame
{
    Q_OBJECT
public:
    PatternDisplay(QWidget *parent = nullptr);
    ~PatternDisplay();
    void setPattern(LEDPattern *p);
    bool isEditable() const;
    void setEditable(bool value);
    void setSelectionColor(int r, int g, int b);
    void setColor(int index, int r, int g, int b);
    QColor getColor(int index);
    enum RightClickActions {FILL, GRADIENT};
    void setDisplayManager(OutputPanelDisplayManager *value);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    LEDPattern *pattern;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *leftSpeedButton;
    QPushButton *rightSpeedButton;
    bool editable;
    int length;
    int vHeight;
    int ledSize;
    int leftIndex;
    int selection;
    int ledsAcross;
    QPoint dragStartPos;
    bool dragSource;
    QPixmap *dragPixmap;
    QMenu *popupMenu;
    OutputPanelDisplayManager *displayManager;
private slots:
    void onLeftButton(void);
    void onRightButton(void);
    void onLeftSpeedButton(void);
    void onRightSpeedButton(void);
signals:
    void selectionChanged(int value);
    void rightClicked(int selection1, int selection2, RightClickActions action);
};

#endif // LEDPATTERNDISPLAY_H
