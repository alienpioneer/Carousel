#ifndef SINGLELEVELCAROUSEL_H
#define SINGLELEVELCAROUSEL_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include "SemicircleWidgetAlt.h"
#include "RectangleWidget.h"

enum class ConveyorSide
{
   FRONT  = 0,
   BACK = 1
};

class SingleLevelCarousel: public QWidget
{
public:
    explicit SingleLevelCarousel(QRect geoRect, int nbBuckets, QWidget *parent);

private:
    QWidget* createCarouselLine     (int nb_buckets);
    QWidget* createLevelContainer   ();
    QWidget* createSynopticContainer();
    QWidget* createSynopticView     ();
    QWidget* createZoomHandle       ();

private slots:
    void on_synopticRightBtnClicked();
    void on_synopticLeftBtnClicked();
    void on_bucketClick();

private:
    int   NB_BUCKETS;
    int   m_lineWidth;
    int   m_bucketWidth;

    int   m_synopticAvailableWidth;
    int   m_bucketsAvailableWidth;

    QVector<RectangleWidget*> m_buckets;
    QWidget*       m_backLine;
    QWidget*       m_frontLine;

    QWidget*       m_synoptic;
    QWidget*       m_synopticContainer;
    QPushButton*   m_synopticRightBtn;
    QPushButton*   m_synopticLeftBtn;

    QScrollArea*   m_scrollArea;

    QWidget*       m_zoomHandle;
};

#endif // SINGLELEVELCAROUSEL_H
