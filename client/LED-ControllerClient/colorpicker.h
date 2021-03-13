#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QFrame>

class ColorPicker : public QFrame
{
    Q_OBJECT
public:
    ColorPicker(QWidget *parent = nullptr);
    ~ColorPicker();
    QVector<QColor> getColors() const;
    void setColors(const QVector<QColor> &value);
    int getSelectedColor() const;
    void setSelectedColor(int value);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    static QVector<QColor> colors;
    static bool colorsLoaded;
    int selectedColor;
    const int space = 2;
    int rWidth;
    int rHeight;
    int border;
    bool colorsChanged;
public slots:
    void onPatternSelectionChange(int value);
};

#endif // COLORPICKER_H
