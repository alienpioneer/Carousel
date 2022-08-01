/*====================================== Solystic =======================================
|
| File: Browser.h
|
| Description: Helper objects for the representation of different parts of the conveyor
|              - used inside the windows Carousel, Outputs, Containers, Maintenance
|
|  Adapté au TOP2000 Italy-> 2022
|
+=======================================================================================*/
#ifndef CONVEYOR_T2K_H
#define CONVEYOR_T2K_H

#include <QObject>
#include <QPushButton>

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QStyleOption>


enum class BucketState
{
   EMPTY    = 0,
   INJECTED = 1,
   SORTED   = 2,
   REJECTED = 3,
   FAILURE  = 4,
   UNKNOWN  = 5,
   DISABLED = 6
};


enum class OutputTrayState
{
   ENABLED        = 0,
   INHIBITED_U    = 1,
   INHIBITED_PATD = 2,
   INHIBITED_SD   = 3
};


enum class ContainerTrayState
{
   EMPTY     = 0,
   NOT_EMPTY = 1,
   UNKNOWN   = 2,
   EJECTED   = 3
};


enum class ConveyorSide
{
   FRONT  = 0,
   BACK = 1
};


enum class ConveyorLevel
{
   LOWER = 0,
   UPPER = 1,
   BOTH  = 2
};


class TrayBase : public QWidget
{
   Q_OBJECT

public:
   TrayBase(QWidget* parent);
   int  id() { return m_id; };
   bool isSelected(){return m_isSelected;}

   void setId(int id) { m_id = id; }
   void setIsLast(bool isLast) {m_isLast = isLast;};
   void setSelected( bool selected ) {m_isSelected = selected;};
   void setColor(QColor color);
   void setText(QString text, bool boldText = false, bool instantUpdate = true);
   void setBold(bool useBoldFont) {m_boldText = useBoldFont;};
   void displayLabel( bool display ) {m_displayLabel = display;}
   void setFontSize(int fontSize){m_fontSize = fontSize;};
   void setUseGradient(bool useGradient){ m_useGradient = useGradient;};

   void redraw();

protected:
   void paintEvent( QPaintEvent* event ) override;
   void mousePressEvent(QMouseEvent *event) override;
   void mouseReleaseEvent(QMouseEvent *event) override;

protected:
   int m_id;
   int m_fontSize;
   QLinearGradient m_gradient;
   QColor m_color;
   QColor m_previousColor;
   QColor m_switchColor;
   QString m_styleSheet;
   QString m_text;
   bool m_isSelected;
   bool m_isLast;
   bool m_displayLabel;
   bool m_boldText;
   bool m_useGradient;

signals:
   void clickReleased(int id);
   void clickPushed(int id);
};

//---------------------------------------------------------------------------------------
// class BucketPlate -> Carousel
//---------------------------------------------------------------------------------------

class BucketPlate : public TrayBase
{
   Q_OBJECT

public:
   BucketPlate(QWidget* parent);
   BucketState state() { return m_state; }
   ConveyorSide side() {return m_side;};
   ConveyorLevel level() {return m_level;};
   QStringList attributes() { return m_attributes; };
   bool selected() const { return m_selected; }
   bool isDisabled() const {return m_isDisabled;};

   void setDisabled(bool disabled){m_isDisabled = disabled;};
   void setState(BucketState state);
   void setSide(ConveyorSide side) { m_side = side; }
   void setLevel(ConveyorLevel level) { m_level = level; }
   void setAttributes(QStringList attributes) { m_attributes = attributes; }

   void restorePreviousState(){ setState(m_previousState); };

private:
   ConveyorSide m_side;
   ConveyorLevel m_level;

   BucketState m_state;
   BucketState m_previousState;
   QStringList m_attributes;
   bool m_isRecycled;
   bool m_selected;
   bool m_isDisabled;
};

//---------------------------------------------------------------------------------------
// class OutputTray -> Outputs
//---------------------------------------------------------------------------------------

class OutputTray : public TrayBase
{
   Q_OBJECT

public:
   OutputTray(QWidget* parent, int id);

   OutputTrayState   state() { return m_state; }
   OutputTrayState   previousState() { return m_previousState; }
   ConveyorSide      side() {return m_side;};
   ConveyorLevel     level() {return m_level;};
   QString           outputId(){return m_outputId;};

   void deselect();
   void select();
   void setState(OutputTrayState state);
   void setLevel(ConveyorLevel level, ConveyorSide side);

   void restorePreviousState(){ setState(m_previousState); };

private:
   ConveyorSide      m_side;
   ConveyorLevel     m_level;
   OutputTrayState   m_state;
   OutputTrayState   m_previousState;
   QString           m_label;
   QString           m_outputId;
};

//---------------------------------------------------------------------------------------
// class TrayReserve - Containers
//---------------------------------------------------------------------------------------

class TrayReserve : public TrayBase
{
   Q_OBJECT

public:
   TrayReserve(QWidget* parent, int id);

   ConveyorSide   side() {return m_side;};
   ConveyorLevel  level() {return m_level;};
   QString        trayId(){return m_trayId;};
   bool           isPresent(){return m_isActive;};

   void           setLevel(ConveyorLevel level, ConveyorSide side){m_level=level;m_side=side;};
   void           setTrayId(QString trayId){m_trayId = trayId;};

   void           disable();
   void           enable();

private:
   ConveyorSide      m_side;
   ConveyorLevel     m_level;
   QString           m_trayId;
   bool              m_isActive;
};

//---------------------------------------------------------------------------------------
// class TrayContainer - Containers
//---------------------------------------------------------------------------------------

class TrayContainer : public TrayBase
{
   Q_OBJECT

public:
   TrayContainer(QWidget* parent, int id);

   ContainerTrayState   state() { return m_state; }
   ConveyorSide         side() {return m_side;};
   ConveyorLevel        level() {return m_level;};
   TrayReserve*         reserve() const {return m_reserve;};
   QString              output() const {return m_output;};
   QString              sortingProduct() const {return m_sortingProduct;};
   QString              trayId() const {return m_trayId;};

   void select();
   void deselect();

   void setState(ContainerTrayState state);
   void setLevel(ConveyorLevel level, ConveyorSide side);
   void setReserve(TrayReserve* reserve){m_reserve = reserve;};
   void restorePreviousState(){ setState(m_previousState); };
   void setOutput(QString output){m_output = output;};
   void setSortingProduct(QString sortingProduct){m_sortingProduct = sortingProduct;};
   void setTrayId(QString trayId){m_trayId = trayId;};

private:
   ConveyorSide         m_side;
   ConveyorLevel        m_level;
   ContainerTrayState   m_state;
   ContainerTrayState   m_previousState;
   TrayReserve*         m_reserve;
   QString              m_trayId;
   QString              m_sortingProduct;
   QString              m_output;
};

//---------------------------------------------------------------------------------------
// class BcsIndicator
//---------------------------------------------------------------------------------------

class BcsIndicator : public QWidget
{
public:
    BcsIndicator(QWidget* parent);
    void setPosition( int x, int y, bool flip );

protected:
    void paintEvent( QPaintEvent* evt ) override;

private:
    QPoint m_pos;
    QColor m_color;
    bool m_flip;
    int m_borderThickness;
};

//---------------------------------------------------------------------------------------
// class Semicircle Widget
//---------------------------------------------------------------------------------------

class SemicircleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SemicircleWidget(QWidget *parent = nullptr, int distBetweenCircles = 10, bool flip = false);
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

//---------------------------------------------------------------------------------------
// class RoundedWidget Widget
//---------------------------------------------------------------------------------------

class RoundedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoundedWidget(QWidget *parent, int circleWidth);
    void setThickness(int thickness) { m_lineThickness = thickness; };
    void setLineStyle(Qt::PenStyle style){m_lineStyle = style;};
    void setColor(QColor color){ m_color = color; };
    void setCircleWidth(int circleWidth){ m_circleWidth = circleWidth; };
    void setBorderColor(QColor color) { m_borderColor = color; };

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int           m_circleWidth;
    int           m_lineThickness;
    Qt::PenStyle  m_lineStyle;
    QColor        m_color;
    QColor        m_borderColor;
};

//---------------------------------------------------------------------------------------
// class MiniLineWidget Widget
//---------------------------------------------------------------------------------------

class MiniLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MiniLineWidget(QWidget *parent, int line_width, int nb_buckets, QString levelLabel, int spacing);
    void setThickness(int thickness) { m_lineThickness = thickness; };
    void setLineStyle(Qt::PenStyle style){m_lineStyle = style;};
    void setColor(QColor color){ m_color = color; };
    void setBorderColor(QColor color) { m_borderColor = color; };

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    int             m_height;
    int             m_width;
    int             m_labelsHeight;
    int             m_levelLabelHeight;
    int             m_separatorLabelHeight;
    int             m_spacing;

    int             m_labelWidth;
    int             m_labelsNb;

    int             m_levelFontSize;
    int             m_fontSize;
    int             m_lineThickness;

    Qt::PenStyle    m_lineStyle;
    QColor          m_color;
    QColor          m_borderColor;

    QString         m_levelLabel;
};

//---------------------------------------------------------------------------------------
// class ZoomLineWidget Widget
//---------------------------------------------------------------------------------------

class ZoomLineWidget : public TrayBase
{
   Q_OBJECT

public:
   ZoomLineWidget(QWidget *parent, int id, bool isStart, bool isEnd);
   void deselect();
   void select();
   void enable(bool enable){m_isEnabled = enable;};
   void setDefaultColor(QColor color);
private:
   bool     m_isEnabled;
   bool     m_isStart;
   QColor   m_selectedColor;
   QColor   m_defaultColor;
};


#endif // CAROUSELHELPERS_H
