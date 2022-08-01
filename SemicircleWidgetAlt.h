#ifndef SEMICIRCLEWIDGETALT_H
#define SEMICIRCLEWIDGETALT_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

//---------------------------------------------------------------------------------------
// class Semicircle Widget
//---------------------------------------------------------------------------------------

class SemicircleWidgetAlt : public QWidget
{
    Q_OBJECT
public:
    explicit SemicircleWidgetAlt(QWidget *parent = nullptr, int distBetweenCircles = 10, bool flip = false);
    void setThickness(int thickness) { m_lineThickness = thickness; };
    void setDistBetweenCircles(int dist) { m_circlesDist = dist; };
    void setLineStyle(Qt::PenStyle style){m_lineStyle = style;};
    void setColor(QColor color){ m_color = color; };
    void setBorderColor(QColor color) { m_borderColor = color; };

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool          m_flip;
    int           m_circlesDist;
    int           m_lineThickness;
    Qt::PenStyle  m_lineStyle;
    QColor        m_color;
    QColor        m_borderColor;
};

#endif // SEMICIRCLEWIDGETALT_H
