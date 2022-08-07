#include "SingleLevelCarousel.h"
#include "SemicircleWidgetAlt.h"

static const int CAROUSEL_LINE_HEIGHT              = 40;
static const int CAROUSEL_CURVES_WIDTH             = 80;
static const int CAROUSEL_LINES_SPACING            = 100;
static const int SYNOPTIC_BUTTON_WIDTH             = 40;

SingleLevelCarousel::SingleLevelCarousel(QRect geoRect, int nbBuckets, QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(geoRect);

    NB_BUCKETS = nbBuckets;

    m_synopticAvailableWidth =  width() - SYNOPTIC_BUTTON_WIDTH;

    m_synoptic = createSynopticView();
}


// The main widget, composed of the scroll area and the switch buttons
QWidget* SingleLevelCarousel::createSynopticView()
{
    QWidget* synopticView = new QWidget(this);

    synopticView->setFixedSize(m_synopticAvailableWidth + SYNOPTIC_BUTTON_WIDTH, height());
    synopticView->setLayout(new QHBoxLayout());
    synopticView->layout()->setSpacing(0);
    synopticView->layout()->setMargin(0);
    synopticView->layout()->setAlignment(Qt::AlignCenter);
    //synopticView->setStyleSheet("background-color: red;");

    m_synopticRightBtn = new QPushButton();
    m_synopticRightBtn->setFixedSize(SYNOPTIC_BUTTON_WIDTH, height());
    m_synopticRightBtn->setIcon(QIcon(":/carousel_move_right.png"));
    m_synopticRightBtn->setIconSize(QSize(40,40));
    m_synopticRightBtn->setStyleSheet("background-color: transparent; border: 1px");
    connect(m_synopticRightBtn, &QPushButton::clicked, this, &SingleLevelCarousel::on_synopticRightBtnClicked);

    m_synopticLeftBtn = new QPushButton();
    m_synopticLeftBtn->setFixedSize(SYNOPTIC_BUTTON_WIDTH, height());
    m_synopticLeftBtn->setIcon(QIcon(":/carousel_move_left.png"));
    m_synopticLeftBtn->setIconSize(QSize(40,40));
    m_synopticLeftBtn->setStyleSheet("background-color: transparent; border: 1px");
    connect(m_synopticLeftBtn, &QPushButton::clicked, this, &SingleLevelCarousel::on_synopticLeftBtnClicked);
    m_synopticLeftBtn->hide();

    m_synopticContainer = createSynopticContainer();

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setFixedSize(m_synopticAvailableWidth, height());
    //m_scrollArea->setStyleSheet("background-color: transparent; border: 2px");
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidget(m_synopticContainer);

    synopticView->layout()->addWidget(m_synopticLeftBtn);
    synopticView->layout()->addWidget(m_scrollArea);
    synopticView->layout()->addWidget(m_synopticRightBtn);

    return synopticView;
}


// Creates the container to be placed inside the scrool area from the main synoptic
QWidget* SingleLevelCarousel::createSynopticContainer()
{
   int upperBackNr, upperFrontNr, maxBucketsPerLine;

   m_bucketsAvailableWidth =  2*m_synopticAvailableWidth - 2*CAROUSEL_CURVES_WIDTH;

   upperFrontNr = NB_BUCKETS/2;

   if (NB_BUCKETS % 2 == 0)
      upperBackNr = NB_BUCKETS/2;
   else
      upperBackNr = NB_BUCKETS/2 + 1;

   maxBucketsPerLine = upperBackNr;

   m_bucketWidth = m_bucketsAvailableWidth/maxBucketsPerLine;

   // Create the two lines
   // IMPORTANT : The creation order is important
   m_frontLine = createCarouselLine( upperFrontNr );
   m_backLine = createCarouselLine( upperBackNr );

   QWidget* synopticContainer = new QWidget(this);
   synopticContainer->setFixedSize(2*m_synopticAvailableWidth, height());

   QWidget* level = createLevelContainer();
   level->setParent(synopticContainer);
   level->move(0,0);

   m_zoomHandle = createZoomHandle();
   m_zoomHandle->setParent(synopticContainer);

   return synopticContainer;
}


// Creates a line composed of buckets
QWidget *SingleLevelCarousel::createCarouselLine(int nb_buckets)
{
    int bucketWidth;
    m_lineWidth = 0;
    int nb_oversizeBuckets =  m_bucketsAvailableWidth - m_bucketWidth*nb_buckets;

    QWidget* conveyorLineWidget = new QWidget(this);
    conveyorLineWidget->setContentsMargins(0,0,0,0);
    conveyorLineWidget->setLayout(new QHBoxLayout());
    conveyorLineWidget->layout()->setSpacing(0);
    conveyorLineWidget->layout()->setMargin(0);

    for (int i=0; i < nb_buckets; i++)
    {
        RectangleWidget* bucket = new RectangleWidget(this);

        nb_oversizeBuckets--;
        if (nb_oversizeBuckets > 0 || nb_oversizeBuckets > nb_buckets)
            bucketWidth = m_bucketWidth + 1;
        else
            bucketWidth = m_bucketWidth;

        bucket->setFixedHeight(CAROUSEL_LINE_HEIGHT);
        bucket->setFixedWidth(bucketWidth);
        m_lineWidth += bucketWidth;

        if(i == nb_buckets-1)
        {
            bucket->setIsLast(true);
            bucket->setStyleSheet("border:1px solid black;");
        }
        else
             bucket->setStyleSheet("border:1px solid black; border-right:0px;");

        if (i%2)
            bucket->setColor(QColor(255,70,80,255));
        else
           bucket->setColor(QColor(122, 163, 39, 255));

        bucket->setId(i);

        conveyorLineWidget->layout()->addWidget(bucket);

        connect(bucket, &RectangleWidget::click, this, &SingleLevelCarousel::on_bucketClick);

        m_buckets.append(bucket);
    }

    return conveyorLineWidget;
}


// Creates 2 curbed widgets and place them at the end of the 2 lines and a space in the middle
QWidget* SingleLevelCarousel::createLevelContainer()
{
    const int levelWidgetWidth = 2 * m_synopticAvailableWidth - 1;
    const int levelWidgetHeight = 2 * CAROUSEL_LINE_HEIGHT + CAROUSEL_LINES_SPACING;

    QWidget* levelWidget = new QWidget(this);
    levelWidget->setMaximumSize(levelWidgetWidth , levelWidgetHeight);

    QHBoxLayout* levelWidgetLayout = new QHBoxLayout(levelWidget);
    levelWidget->setLayout(levelWidgetLayout);
    levelWidgetLayout->setMargin(0);
    levelWidgetLayout->setSpacing(0);

    // Creates te container for the 2 lines and a middle separator
    QWidget* linesContainer = new QWidget(levelWidget);
    linesContainer->setFixedSize( m_lineWidth , CAROUSEL_LINE_HEIGHT*2 + CAROUSEL_LINES_SPACING);

    QWidget* linesSeparator = new QWidget(linesContainer);
    linesSeparator->setFixedSize( m_lineWidth , CAROUSEL_LINES_SPACING);
    linesSeparator->setAttribute(Qt::WA_TransparentForMouseEvents);
    linesSeparator->setAttribute(Qt::WA_TranslucentBackground);

    linesContainer->setLayout(new QVBoxLayout());
    linesContainer->layout()->setMargin(0);
    linesContainer->layout()->setSpacing(0);
    linesContainer->layout()->addWidget(m_backLine);
    linesContainer->layout()->addWidget(linesSeparator);
    linesContainer->layout()->addWidget(m_frontLine);

    SemicircleWidgetAlt* leftCurves = new SemicircleWidgetAlt(levelWidget, CAROUSEL_LINE_HEIGHT-2, true);
    leftCurves->setFixedSize(CAROUSEL_CURVES_WIDTH , CAROUSEL_LINE_HEIGHT*2 + CAROUSEL_LINES_SPACING);
    SemicircleWidgetAlt* rightCurves = new SemicircleWidgetAlt(levelWidget, CAROUSEL_LINE_HEIGHT-2);
    rightCurves->setFixedSize(CAROUSEL_CURVES_WIDTH , CAROUSEL_LINE_HEIGHT*2 + CAROUSEL_LINES_SPACING);

    leftCurves->setColor(QColor(170,170,170,255));
    rightCurves->setColor(QColor(170,170,170,255));

    levelWidgetLayout->addWidget(leftCurves);
    levelWidgetLayout->addWidget(rightCurves);
    levelWidgetLayout->insertWidget(1,linesContainer);
    linesContainer->raise();

    int spacingWidth = levelWidgetWidth - m_lineWidth - 2*CAROUSEL_CURVES_WIDTH;

    if ( spacingWidth > 0 )
        levelWidgetLayout->insertSpacing(0, spacingWidth);

    // Mask the interior
    QRegion levelWidgetRegion = QRegion(QRect(0, 0, levelWidgetWidth , height()));
    QRegion regionToMask      = QRegion(QRect(CAROUSEL_CURVES_WIDTH,CAROUSEL_LINE_HEIGHT,
                                              levelWidgetWidth - 2*CAROUSEL_CURVES_WIDTH , CAROUSEL_LINES_SPACING));
    QRegion levelWidgetMaskRegion = levelWidgetRegion.subtracted(regionToMask);
    levelWidget->setMask(levelWidgetMaskRegion);

    return levelWidget;
}


QWidget *SingleLevelCarousel::createZoomHandle()
{
    QWidget* widget = new QWidget(this);
    widget->setAttribute(Qt::WA_TransparentForMouseEvents);
    widget->setStyleSheet( "border: 2px solid #7E00FF; background: rgba(126,25,227,0.125)");
    widget->resize(55, 40);
    widget->setVisible(false);
    return widget;
}


void SingleLevelCarousel::on_synopticRightBtnClicked()
{
   m_scrollArea->horizontalScrollBar()->setValue(m_scrollArea->horizontalScrollBar()->value() + m_synopticAvailableWidth);
   m_synopticRightBtn->hide();
   m_synopticLeftBtn->show();
}


void SingleLevelCarousel::on_synopticLeftBtnClicked()
{
   m_scrollArea->horizontalScrollBar()->setValue(m_scrollArea->horizontalScrollBar()->value() - m_synopticAvailableWidth);
   m_synopticLeftBtn->hide();
   m_synopticRightBtn->show();
}

void SingleLevelCarousel::on_bucketClick()
{

}
