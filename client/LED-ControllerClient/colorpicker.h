#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QFrame>

class ColorPicker : public QFrame
{
    Q_OBJECT
public:
    ColorPicker(QWidget *parent = nullptr);
    static void loadColors();
    static void saveColors();
    QVector<QColor> getColors() const;
    void setColors(const QVector<QColor> &value);
    QColor getColor(void);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    static QVector<QColor> colors;
    static bool colorsLoaded;
    static bool colorsChanged;
    int selectedColor;
    const int space = 2;
    int rWidth;
    int rHeight;
    int border;
public slots:
    void onPatternSelectionChange(int value);
signals:
    void colorChanged(QColor c);
};

#endif // COLORPICKER_H
