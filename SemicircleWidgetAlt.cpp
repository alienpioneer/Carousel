#include "SemicircleWidgetAlt.h"

//---------------------------------------------------------------------------------------
// class Semicircle Widget
//---------------------------------------------------------------------------------------

// Same version as in Conveyor.h

SemicircleWidgetAlt::SemicircleWidgetAlt(QWidget *parent, int distBetweenCircles, bool flip)
    : QWidget{parent},
      m_lineThickness(1),
      m_lineStyle(Qt::SolidLine),
      m_color(QColor(200,200,200,255)),
      m_borderColor(QColor(0, 0, 0, 255))
{
    m_circlesDist = distBetweenCircles;
    m_flip = flip;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void SemicircleWidgetAlt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter( this );
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_borderColor, m_lineThickness, m_lineStyle, Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(m_color);

    int R = width()-m_lineThickness;
    int r = R - m_circlesDist;

    QPoint startA, handleAUp, midA, handleADown, endA;
    QPoint startB, handleBUp, midB, handleBDown, endB;

    if (m_flip)
    {
        startA       = QPoint(width()         , m_lineThickness);
        handleAUp    = QPoint(m_lineThickness , m_lineThickness);
        midA         = QPoint(m_lineThickness , height()/2);
        handleADown  = QPoint(m_lineThickness , height()-m_lineThickness);
        endA         = QPoint(width()         , height()-m_lineThickness);

        startB       = QPoint(startA.x()                     , startA.y() + m_circlesDist);
        handleBUp    = QPoint(handleAUp.x() + m_circlesDist  , handleAUp.y() + m_circlesDist);
        midB         = QPoint(handleAUp.x() + m_circlesDist  , midA.y());
        handleBDown  = QPoint(handleAUp.x() + m_circlesDist  , handleADown.y() - m_circlesDist);
        endB         = QPoint(endA.x()                       , endA.y() - m_circlesDist);
    }
    else
    {
        startA       = QPoint(0   , m_lineThickness);
        handleAUp    = QPoint(R   , m_lineThickness);
        midA         = QPoint(R   , height()/2);
        handleADown  = QPoint(R   , height()-m_lineThickness);
        endA         = QPoint(0   , height()-m_lineThickness);

        startB       = QPoint(0   , startA.y() + m_circlesDist);
        handleBUp    = QPoint(r   , handleAUp.y() + m_circlesDist);
        midB         = QPoint(r   , midA.y());
        handleBDown  = QPoint(r   , handleADown.y() - m_circlesDist);
        endB         = QPoint(0   , endA.y() - m_circlesDist);
    }

    QPainterPath path;
    path.moveTo(startA);
    path.quadTo(handleAUp , midA);
    path.quadTo(handleADown, endA);
    path.moveTo(startB);
    path.quadTo(handleBUp , midB);
    path.quadTo(handleBDown, endB);

    painter.drawPath(path);
    painter.end();
}
