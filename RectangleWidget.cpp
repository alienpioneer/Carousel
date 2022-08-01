#include "RectangleWidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

RectangleWidget::RectangleWidget(QWidget *parent):
    m_id(-1),
    m_fontSize(7),
    m_color(QColor( 0, 0, 0, 0 )),
    m_styleSheet(""),
    m_text(""),
    m_isSelected(false),
    m_isLast(false),
    m_isMovable(false)
{
    setParent(parent);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void RectangleWidget::setColor(QColor color, bool instantUpdate)
{
    m_color = color;
    if (isVisible() && instantUpdate) update();
}

void RectangleWidget::setText(QString text, bool instantUpdate)
{
    m_text = text;
    if (isVisible() && instantUpdate) update();
}

void RectangleWidget::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing);

    QPen pen( QColor( 0, 0, 0, 1 ), 1, Qt::SolidLine );
    QFont font( "Arial", m_fontSize, QFont::Light );
    painter.setPen( pen );
    painter.setFont( font );
    QRect textRect = QRect(0, height()/2-m_fontSize/2-1, width(), m_fontSize+2);
    painter.drawText(textRect,Qt::AlignCenter, m_text );

    QRect geo(0, 0, width(), height());
    painter.fillRect(geo, m_color);

    QStyleOption styleOpt;
    styleOpt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void RectangleWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED( event );
    m_mousePressOffset = event->pos();
    m_switchColor = m_color;
    setColor(QColor("#5EA9F3"));
}

void RectangleWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED( event );
    setColor(m_switchColor);
    emit click();
}

void RectangleWidget::mouseMoveEvent(QMouseEvent *event)
{
    //Q_UNUSED(event);
//    setUpdatesEnabled(false);
    if (m_isMovable)
        move(mapToParent(event->pos() - m_mousePressOffset));
     qDebug() << "RectangleWidget moved" << m_id;
}

