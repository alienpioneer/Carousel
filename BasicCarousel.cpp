#include "BasicCarousel.h"

static const int CAROUSEL_LINE_HEIGHT              = 40;
static const int CAROUSEL_CURVES_WIDTH             = 80;
static const int CAROUSEL_LINES_SPACING            = 100;
static const int SYNOPTIC_BUTTON_WIDTH             = 40;


BasicCarousel::BasicCarousel(QRect geoRect, int nbBuckets, QWidget *parent)
    : QWidget{parent}
{
    this->setGeometry(geoRect);

    NB_BUCKETS = nbBuckets;

    m_synopticAvailableWidth =  width() - SYNOPTIC_BUTTON_WIDTH;

    m_synoptic = createSynopticView();
}


QWidget *BasicCarousel::createSynopticView()
{
    QWidget* synopticView = new QWidget(this);
    synopticView->setStyleSheet("border: 1px solid black");

    return synopticView;
}
