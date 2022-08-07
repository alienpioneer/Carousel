#include "BasicCarousel.h"
#include <QHBoxLayout>
#include <QDebug>

static const int ITERATION_NB = 100;
static const int ITERATION_STEP = 1;

BasicCarousel::BasicCarousel(const QRect geoRect, const int nbBuckets, QWidget *parent)
    : QWidget{parent},
      NB_BUCKETS(nbBuckets),
      m_bcsPosition(1),
      m_previousBcsPosition(0),
      m_timer(new QTimer(this))
{
    this->setGeometry(geoRect);

    connect(m_timer, &QTimer::timeout, this, &BasicCarousel::on_timer);

    CAROUSEL_CURVES_WIDTH = 0.11*width();

    CAROUSEL_LINE_HEIGHT = 0.3*height();

    CAROUSEL_LINES_SPACING = 0.4*height();

    m_synoptic = createSynopticView();

    setInitialState();

    m_timer->setInterval(2000);
    m_timer->start();
}


void BasicCarousel::updateBuckets()
{
    m_buffer.clear();
    BucketPlateData data;

    // WARNING : Each iteration is building up from previous iteration
    // Need offset from previous iteration
    int posOffset = m_bcsPosition - m_previousBcsPosition;
    //qDebug() <<"Pos offset: "<< posOffset;

    for (int i = 0; i < NB_BUCKETS; i++)
    {
        // a[(index+offset)%N] - circular buffer
        data.state = m_buckets[(i+posOffset-1)%NB_BUCKETS]->state();
        data.id = m_buckets[(i+posOffset-1)%NB_BUCKETS]->id();

        m_buffer.append(data);
    }

    for(int i = 0; i< m_buckets.size(); i++)
    {
        m_buckets[i]->setId( m_buffer[i].id);
        m_buckets[i]->setText(QString::number(m_buckets[i]->id()),false,true);
        m_buckets[i]->setState( m_buffer[i].state);
        //m_buckets[i]->repaint();
    }
}


void BasicCarousel::on_timer()
{
    if(m_bcsPosition < ITERATION_NB)
    {
        m_bcsPosition += ITERATION_STEP;
        updateBuckets();
        m_previousBcsPosition = m_bcsPosition;
    }
}

void BasicCarousel::on_plateClicked(int id)
{
    qDebug() <<"Selected id: "<< id;
}


void BasicCarousel::setInitialState()
{
    int index = 0;
    int rnd = 0;

    for(auto bucket: qAsConst(m_buckets))
    {
        rnd = std::rand()%100;

        if (rnd > 60)
            bucket->setState(BucketState::SORTED);
        else if (rnd<10)
            bucket->setState(BucketState::FAILURE);
        else if (rnd>10 && rnd<20)
            bucket->setState(BucketState::REJECTED);

        bucket->setId(index);
        bucket->setText(QString::number(bucket->id()));
        index++;
    }
}


QWidget* BasicCarousel::createSynopticView()
{
    QWidget* synopticView = new QWidget(this);
    synopticView->setStyleSheet("border: 1px solid black");
    synopticView->setMaximumSize(width() , 2*CAROUSEL_LINE_HEIGHT + CAROUSEL_LINES_SPACING);

    QHBoxLayout* synopticViewLayout = new QHBoxLayout(synopticView);
    synopticViewLayout->setMargin(0);
    synopticViewLayout->setSpacing(0);
    synopticView->setLayout(synopticViewLayout);

    // Calculate available space for bucket width
    int upperBackNr, upperFrontNr;

    m_bucketsAvailableWidth =  width()-2*CAROUSEL_CURVES_WIDTH;

    upperFrontNr = NB_BUCKETS/2;

    if (NB_BUCKETS % 2 == 0)
        upperBackNr = NB_BUCKETS/2;
    else
        upperBackNr = NB_BUCKETS/2 + 1;

    // BUCKET_WIDTH is always rounded down
    BUCKET_WIDTH = m_bucketsAvailableWidth/upperBackNr;

    // Create the two lines
    // IMPORTANT : The creation order is important
    m_frontLine = createCarouselLine( upperFrontNr, ConveyorSide::FRONT );
    m_backLine = createCarouselLine( upperBackNr, ConveyorSide::BACK);

    SemicircleWidget* leftCurves = new SemicircleWidget(synopticView, CAROUSEL_LINE_HEIGHT-2, true);
    leftCurves->setFixedSize(CAROUSEL_CURVES_WIDTH , CAROUSEL_LINE_HEIGHT*2 + CAROUSEL_LINES_SPACING);
    leftCurves->setColor(QColor(170,170,170,255));
    SemicircleWidget* rightCurves = new SemicircleWidget(synopticView, CAROUSEL_LINE_HEIGHT-2);
    rightCurves->setFixedSize(CAROUSEL_CURVES_WIDTH , CAROUSEL_LINE_HEIGHT*2 + CAROUSEL_LINES_SPACING);
    rightCurves->setColor(QColor(170,170,170,255));

    QVBoxLayout* linesLayout = new QVBoxLayout();
    linesLayout->setMargin(0);
    linesLayout->setSpacing(0);

    //QSpacerItem* spacer =  new QSpacerItem(m_bucketsAvailableWidth, CAROUSEL_LINES_SPACING);
    linesLayout->addWidget(m_backLine);
    linesLayout->addStretch(10);
    linesLayout->addWidget(m_frontLine);

    synopticViewLayout->addWidget(leftCurves);
    synopticViewLayout->addLayout(linesLayout);
    synopticViewLayout->addWidget(rightCurves);

    return synopticView;
}


// Creates a line composed of buckets
QWidget* BasicCarousel::createCarouselLine(int nb_buckets, ConveyorSide side)
{
    int bucketWidth;

    LINE_WIDTH = 0;

    // BUCKET_WIDTH is always rounded down, so the amount of unused space is less than 1 pixel for each bucket
    // The remaining width from rounding down may be used to increase the width with 1px for a number of buckets
    int nb_oversizeBuckets =  m_bucketsAvailableWidth - BUCKET_WIDTH*nb_buckets;

    QWidget* carouselLineWidget = new QWidget(this);
    carouselLineWidget->setContentsMargins(0,0,0,0);
    carouselLineWidget->setLayout(new QHBoxLayout());
    carouselLineWidget->layout()->setSpacing(0);
    carouselLineWidget->layout()->setMargin(0);

    QVector<BucketPlate*> buckets;

    for (int i=0; i < nb_buckets; i++)
    {
        BucketPlate* bucket = new BucketPlate(this);

        if (nb_oversizeBuckets > 0 || nb_oversizeBuckets > nb_buckets)
        {
            bucketWidth = BUCKET_WIDTH + 1;
            nb_oversizeBuckets--;
        }
        else
            bucketWidth = BUCKET_WIDTH;

        bucket->setFixedHeight(CAROUSEL_LINE_HEIGHT);
        bucket->setFixedWidth(bucketWidth);

        LINE_WIDTH += bucketWidth;

        if(i == nb_buckets-1)
            bucket->setIsLast(true);

        bucket->displayLabel(true);
        bucket->setState(BucketState::EMPTY);

        carouselLineWidget->layout()->addWidget(bucket);

        if(side == ConveyorSide::BACK)
            buckets.push_front(bucket);
        else
            buckets.push_back(bucket);

        connect(bucket, &BucketPlate::clickReleased, this, &BasicCarousel::on_plateClicked);

    }

    m_buckets.append(buckets);

    return carouselLineWidget;
}
