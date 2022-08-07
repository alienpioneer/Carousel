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
#include "Conveyor_T2K.h"
#include <QDebug>

//---------------------------------------------------------------------------------------
// class TrayBase
// Serves as base class for object display
//---------------------------------------------------------------------------------------

TrayBase::TrayBase(QWidget *parent):
    m_id(-1),
    m_fontSize(7),
    m_gradient(QLinearGradient(0,0,0,100)),
    m_color(QColor( 0, 0, 0, 0 )),
    m_styleSheet(""),
    m_text(""),
    m_isSelected(false),
    m_isLast(false),
    m_displayLabel(false),
    m_boldText(false),
    m_useGradient(false)
{
    m_gradient.setColorAt(0.2, QColor(255,255,255,255));
    m_previousColor = m_color;
    setParent(parent);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void TrayBase::setColor(QColor color)
{
    m_color = color;
    redraw();
}

void TrayBase::setText(QString text, bool boldText, bool instantUpdate)
{
    m_text = text;
    m_boldText = boldText;
    if (instantUpdate)
        redraw();
}

void TrayBase:: redraw()
{
    // edit styleSheet only if changed for performance
    // outside isVisible because of update issues with text
    if ( this->styleSheet().compare( m_styleSheet ) != 0)
        setStyleSheet( m_styleSheet );

    if ( isVisible() && m_color != m_previousColor)
    {
        update();
        m_previousColor = m_color;
    }
    else if (isVisible() && m_text != "")
    {
        update();
    }
}

void TrayBase::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    QPainter painter( this );

    painter.setRenderHint( QPainter::Antialiasing);

    QPen pen( QColor( "#000" ), 1, Qt::SolidLine );
    painter.setPen( pen );

    QRect geo(0, 0, width(), height());

    if (m_useGradient)
    {
        m_gradient.setColorAt(0.0, m_color);
        painter.fillRect(geo, m_gradient);
    }
    else
    {
        painter.fillRect(geo, m_color);
    }

    if (m_text != "" && m_displayLabel)
    {
        QRect textRect;
        QFont font;

        if (m_boldText)
            font = QFont( "Arial", m_fontSize, QFont::Bold);
        else
            font = QFont( "Arial", m_fontSize, QFont::Normal);

        painter.setFont( font );

        if( m_text.contains("\n") ) // If text has 2 lines
        {
            textRect = QRect(0, height()/2-m_fontSize-6, width(), 2*(m_fontSize + 6));
        }
        else
        {
            textRect = QRect(0, height()/2-m_fontSize/2-1, width(), m_fontSize + 4);
        }

        painter.drawText(textRect,Qt::AlignCenter, m_text );
    }

    QStyleOption styleOpt;
    styleOpt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);

    painter.end();
}

void TrayBase::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED( event );
    //   qDebug() << "Click Position : " << mapToGlobal(event->pos());
    m_switchColor = m_color;
    setColor(QColor("#5EA9F3"));
    emit clickPushed(m_id);
}

void TrayBase::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED( event );
    setColor(m_switchColor);
    emit clickReleased(m_id);
}

//---------------------------------------------------------------------------------------
// class BucketPlate
//---------------------------------------------------------------------------------------

BucketPlate::BucketPlate(QWidget* parent):
    TrayBase(parent),
    m_side(ConveyorSide::FRONT),
    m_level(ConveyorLevel::UPPER),
    m_isDisabled(false)
{
    setState(BucketState::EMPTY);
}

void BucketPlate::setState(BucketState state)
{

    m_styleSheet = "border:1px solid black; ";

    switch ( state)
    {
    case BucketState::UNKNOWN:
        m_color = QColor(0,0,0,0);
        m_styleSheet = "background:url(qrc:/layout/hatchedNoAlpha.png);"
                       " border:1px solid black;";
        break;
    case BucketState::EMPTY:
        m_color = QColor(0xFF,0xFF,0xFF);
        break;
    case BucketState::INJECTED:
        m_color = QColor(0x99,0xCC,0xFF);
        break;
    case BucketState::SORTED:
        m_color = QColor(0x73,0xE6,0x00);
        break;
    case BucketState::REJECTED:
        m_color = QColor(0xFF,0x9D,0x3B);
        break;
    case BucketState::FAILURE:
        m_color = QColor(0xE4,0x34,0x34);
        break;
    case BucketState::DISABLED:
        m_color = QColor(0x80,0x80,0x80);
        m_previousState = m_state;
        setDisabled(true);
        break;
    default:;
    }

    m_state = state;

    if( !m_isLast )
        m_styleSheet += "border-right:0;";

    redraw();
}


//---------------------------------------------------------------------------------------
// class OutputTray
//---------------------------------------------------------------------------------------

OutputTray::OutputTray(QWidget *parent, int id):
    TrayBase(parent)
{
    m_displayLabel = true;
    m_isSelected = false;
    setId(id);
    setState(OutputTrayState::ENABLED);
}

void OutputTray::deselect()
{
    if (isSelected())
    {
        setSelected(false);
        restorePreviousState();
        redraw();
    }
}

void OutputTray::select()
{
    if (!isSelected())
    {
        setSelected(true);
        m_previousState = m_state;
        m_color = QColor("#5E96EB");
        redraw();
    }
}


void OutputTray::setState(OutputTrayState state)
{
    m_state = state;

    if(m_isSelected)
        m_color = QColor("#5E96EB");
    else
    {
        switch ( m_state )
        {
        case OutputTrayState::ENABLED:
            m_color = QColor("#E1F4FF");
            break;
        case OutputTrayState::INHIBITED_U:
            m_color = QColor("#8282C4");
            break;
        case OutputTrayState::INHIBITED_PATD:
            m_color = QColor("#6DADAD");
            break;
        case OutputTrayState::INHIBITED_SD:
            m_color = QColor("#B88C5A");
            break;
        default:;
        }
    }

    m_styleSheet = "border:1px solid black; ";

    if( !m_isLast )
        m_styleSheet += "border-right:0;";

    redraw();
}

void OutputTray::setLevel(ConveyorLevel level, ConveyorSide side)
{
    m_side = side;
    m_level = level;

    if (level == ConveyorLevel::UPPER)
    {
        if (side == ConveyorSide::FRONT)
        {
            m_label = "UF\n" + QString::number(m_id);
            m_outputId = QString::number(21001 + m_id);
        }
        else
        {
            m_label = "UB\n" + QString::number(m_id);
            m_outputId = QString::number(22001 + m_id);
        }
    }
    else
    {
        if (side == ConveyorSide::FRONT)
        {
            m_label = "LF\n" + QString::number(m_id);
            m_outputId = QString::number(11001 + m_id);
        }
        else
        {
            m_label = "LB\n" + QString::number(m_id);
            m_outputId = QString::number(12001 + m_id);
        }
    }

    m_text = m_label;
}

//---------------------------------------------------------------------------------------
// class BcsIndicator
//---------------------------------------------------------------------------------------

BcsIndicator::BcsIndicator( QWidget* parent )
    : QWidget( parent ),
      m_color(QColor(0,0,0,255)),
      m_borderThickness(1)
{
    setAttribute( Qt::WA_TransparentForMouseEvents );
}

void BcsIndicator::setPosition( int x, int y, bool flip )
{
    m_pos  = QPoint( x, y );
    m_flip = flip;
    move(m_pos.x() - width()/2, m_pos.y());
    if (isVisible()) update();
}

void BcsIndicator::paintEvent( QPaintEvent* evt )
{
    Q_UNUSED( evt );
    QPainter painter( this );
    painter.setPen( QPen( QColor( 0,0,0,255 ), m_borderThickness, Qt::SolidLine ) );
    painter.setBrush( QBrush( m_color ) );
    //painter.setBrush( QBrush( QColor( 0,0,0,0 ) ) );
    painter.setRenderHint( QPainter::Antialiasing);

    painter.drawPolygon( QPolygonF( { QPointF( m_borderThickness, height()-m_borderThickness ),
                                      QPointF( width()-2*m_borderThickness, height()-m_borderThickness),
                                      QPointF( width()/2-m_borderThickness, m_borderThickness ) } ) );
    painter.save();
    if ( m_flip )
        painter.rotate(180);
    painter.restore();
}

//---------------------------------------------------------------------------------------
// class TrayReserve
//---------------------------------------------------------------------------------------

TrayReserve::TrayReserve(QWidget *parent, int id):
    TrayBase(parent)
{
    setId(id);
    m_styleSheet = "border:1px solid black;";
    setStyleSheet(m_styleSheet);
    setColor( QColor(255,255,255,255) );
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void TrayReserve::disable()
{
    setColor(QColor(255,255,255,0));
    m_styleSheet = "border:0px";
    setStyleSheet(m_styleSheet);
    m_isActive = false;
}

void TrayReserve::enable()
{
    m_styleSheet = "border:1px solid black;";
    setColor( QColor(255,255,255,255) );
    setStyleSheet(m_styleSheet);
    m_isActive = true;
}

//---------------------------------------------------------------------------------------
// class TrayContainer - Containers
//---------------------------------------------------------------------------------------

TrayContainer::TrayContainer(QWidget *parent, int id):
    TrayBase(parent),
    m_reserve(nullptr)
{
    m_displayLabel = false;
    m_isSelected = false;
    setId(id);
    setState(ContainerTrayState::UNKNOWN);
}

void TrayContainer::select()
{
    if(!isSelected())
    {
        setSelected(true);
        m_previousState = m_state;
        m_styleSheet = "border:3px solid #128AE6; background-color: rgba(18, 138, 230, 0.2);";
        redraw();
    }
}

void TrayContainer::deselect()
{
    if(isSelected())
    {
        setSelected(false);
        restorePreviousState();
    }
}

void TrayContainer::setState(ContainerTrayState state)
{
    m_state = state;

    if(isSelected())
        m_styleSheet = "border:3px solid #128AE6; background-color: rgba(18, 138, 230, 0.2);";
    else
        m_styleSheet = "border:1px solid black; ";

    switch ( m_state )
    {
    case ContainerTrayState::UNKNOWN:
        m_color = QColor("#F44242");
        break;
    case ContainerTrayState::EMPTY:
        m_color = QColor("#F9F6E8");
        break;
    case ContainerTrayState::NOT_EMPTY:
        m_color = QColor("#FAC191");
        break;
    case ContainerTrayState::EJECTED:
        setSelected(false);
        m_styleSheet = "background:url(qrc:/layout/hatchedNoAlpha.png);"
                       "border:1px solid black;";
        break;
    default:;
    }

    if( !m_isLast && !isSelected())
        m_styleSheet += "border-right:0;";

    redraw();
}

void TrayContainer::setLevel(ConveyorLevel level, ConveyorSide side)
{
    m_side = side;
    m_level = level;

    QString txt = "";

    if (level == ConveyorLevel::UPPER)
        txt = "U";
    else
        txt = "L";

    if (side == ConveyorSide::FRONT)
        txt += "F";
    else
        txt += "B";
}


//---------------------------------------------------------------------------------------
// class Semicircle Widget
//---------------------------------------------------------------------------------------

SemicircleWidget::SemicircleWidget(QWidget *parent, int distBetweenCircles, bool flip)
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

void SemicircleWidget::paintEvent(QPaintEvent *event)
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

//---------------------------------------------------------------------------------------
// class Rounded Widget
//---------------------------------------------------------------------------------------

RoundedWidget::RoundedWidget(QWidget *parent, int circleWidth)
    : QWidget{parent},
      m_lineThickness(1),
      m_lineStyle(Qt::SolidLine),
      m_color(QColor(200,200,200,255)),
      m_borderColor(QColor(0, 0, 0, 255))
{
    m_circleWidth = circleWidth;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void RoundedWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter( this );
    QPainterPath path;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_borderColor, m_lineThickness, m_lineStyle, Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(m_color);

    path.moveTo( width() - m_circleWidth - m_lineThickness, m_lineThickness );
    path.lineTo(m_circleWidth, m_lineThickness);
    path.quadTo(QPoint( m_lineThickness , m_lineThickness), QPoint(m_lineThickness , height()/2));
    path.quadTo(QPoint(m_lineThickness , height()-m_lineThickness),  QPoint(m_circleWidth, height()-m_lineThickness));
    path.lineTo(width()-m_circleWidth-m_lineThickness, height()-m_lineThickness);
    path.quadTo(QPoint(width()-m_lineThickness, height()-m_lineThickness), QPoint(width()-m_lineThickness, height()/2));
    path.quadTo(QPoint(width()-m_lineThickness, m_lineThickness), QPoint(width()-m_circleWidth-m_lineThickness, m_lineThickness));

    painter.drawPath(path);
    painter.end();
}

//---------------------------------------------------------------------------------------
// class MiniLineWidget Widget
//---------------------------------------------------------------------------------------

MiniLineWidget::MiniLineWidget(QWidget *parent, int line_width, int nb_buckets, QString levelLabel, int spacing)
    : QWidget{parent},
      m_levelFontSize(10),
      m_fontSize(8),
      m_lineThickness(1),
      m_lineStyle(Qt::SolidLine),
      m_color(QColor(125,187,247,255)),
      m_borderColor(QColor(0, 0, 0, 255))

{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setFixedSize(line_width, parent->height());

    m_separatorLabelHeight  = m_fontSize*2;
    m_levelLabelHeight      = m_levelFontSize*2;
    m_labelsHeight          = m_levelLabelHeight + m_separatorLabelHeight;

    m_spacing = spacing;
    m_labelsNb = nb_buckets/100;

    //use rule of three simple for width of a "hundredth" -> if width()...nb_buckets then x ...100
    m_labelWidth    = (width()*100)/nb_buckets+1;
    m_height        = parent->height() - m_labelsHeight - m_spacing - 2*m_lineThickness;

    m_levelLabel = levelLabel;
}

void MiniLineWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter( this );
    QPainterPath path;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_borderColor, m_lineThickness, m_lineStyle, Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(m_color);

    // Draw contour
    path.moveTo( m_lineThickness, m_lineThickness + m_labelsHeight );
    path.lineTo( width()-m_lineThickness, m_lineThickness + m_labelsHeight );
    path.lineTo( width()-m_lineThickness, m_lineThickness + m_labelsHeight + m_height );
    path.lineTo( m_lineThickness, m_lineThickness + m_labelsHeight + m_height );
    path.lineTo( m_lineThickness, m_lineThickness + m_labelsHeight );

    // Draw separators
    for(int i = 1; i <= m_labelsNb; i++)
    {
        path.moveTo(m_labelWidth*i-m_lineThickness, m_lineThickness + m_labelsHeight + m_height);
        path.lineTo(m_labelWidth*i-m_lineThickness, m_lineThickness + m_labelsHeight);
    }

    painter.drawPath(path);

    QRect textRect;

    // Draw level label
    painter.setFont( QFont( "Arial", m_levelFontSize, QFont::Bold));
    // Width()/2 seems sufficient for this label
    textRect = QRect(0, 0, width()/2, m_levelLabelHeight);
    painter.drawText(textRect, Qt::AlignLeft, m_levelLabel );

    // Draw separators labels
    painter.setFont( QFont( "Arial", m_fontSize, QFont::Normal));

    for(int i = 1; i <= m_labelsNb; i++)
    {
        textRect = QRect(m_labelWidth*i-m_fontSize*2, m_lineThickness + m_levelLabelHeight, m_fontSize*4, m_separatorLabelHeight);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(100*i) );
    }

    painter.end();
}

//---------------------------------------------------------------------------------------
// class ZoomLineWidget Widget
//---------------------------------------------------------------------------------------

ZoomLineWidget::ZoomLineWidget(QWidget *parent, int id, bool isStart, bool isEnd)
    :TrayBase(parent)
{
    setId(id);

    // Not used, keep it in case
//    QString label;
//    if (level == ConveyorLevel::UPPER)
//        label = "UC\n" + QString::number(id);
//    else
//        label = "LC\n" + QString::number(id);

    setText(QString::number(id));

    setFontSize(10);
    m_displayLabel = true;

    m_isSelected = false;

    m_selectedColor = QColor(94, 150, 235, 255);
    m_defaultColor   = QColor(109, 224, 230 ,255);
    setColor(m_defaultColor);

    m_isStart = isStart;
    setIsLast(isEnd);

    m_styleSheet = "border:2px solid black; ";

    if( !m_isLast )
        m_styleSheet += "border-right:0;";

    if( !m_isStart )
        m_styleSheet += "border-left-width:1;";

    redraw();
}

void ZoomLineWidget::deselect()
{
    if (isSelected() && m_isEnabled)
    {
        setSelected(false);
        setColor(m_defaultColor);
        redraw();
    }
}

void ZoomLineWidget::select()
{
    if (!isSelected() && m_isEnabled)
    {
        setSelected(true);
        setColor(m_selectedColor);
        redraw();
    }
}

void ZoomLineWidget::setDefaultColor(QColor color)
{
    m_defaultColor = color;
    setColor(m_defaultColor);
}

