#ifndef BASICCAROUSEL_H
#define BASICCAROUSEL_H

#include <QObject>
#include <QWidget>
#include <Conveyor/Conveyor_T2K.h>

class BasicCarousel : public QWidget
{
    Q_OBJECT
public:
    explicit BasicCarousel(QRect geoRect, int nbBuckets, QWidget *parent = nullptr);


private:
    QWidget* createSynopticView ();

private:
    int   NB_BUCKETS;
    int   m_lineWidth;
    int   m_bucketWidth;

    int   m_synopticAvailableWidth;
    int   m_bucketsAvailableWidth;

    QVector<BucketPlate*> m_buckets;
    QWidget*       m_backLine;
    QWidget*       m_frontLine;

    QWidget*       m_synoptic;
    QWidget*       m_synopticContainer;

    QWidget*       m_zoomHandle;
};

#endif // BASICCAROUSEL_H
