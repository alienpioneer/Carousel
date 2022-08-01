#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

#include <QObject>
#include <QWidget>

class RectangleWidget : public QWidget
{
    Q_OBJECT
public:
    RectangleWidget(QWidget* parent);
    void setIsLast(bool isLast) {m_isLast = isLast;};
    void setSelected( bool selected ) {m_isSelected = selected;};
    void setColor(QColor color, bool instantUpdate = true);
    void setText(QString text, bool instantUpdate = true);
    void setId(int id){m_id = id;};
    int id(){return m_id;};

protected:
    void paintEvent( QPaintEvent* event ) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected:
    int m_id;
    int m_fontSize;
    QColor m_color;
    QColor m_switchColor;
    QString m_styleSheet;
    QString m_text;
    bool m_isSelected;
    bool m_isLast;
    QPoint m_mousePressOffset;
    bool m_isMovable;

signals:
    void click();

};


#endif // RECTANGLEWIDGET_H
