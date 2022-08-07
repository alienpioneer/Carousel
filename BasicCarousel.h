#ifndef BASICCAROUSEL_H
#define BASICCAROUSEL_H

#include <QObject>
#include <QWidget>
#include <Conveyor/Conveyor_T2K.h>
#include <QTimer>

struct BucketPlateData
{
    int id;
    BucketState state;
};

class BasicCarousel : public QWidget
{
    Q_OBJECT
public:
    explicit BasicCarousel(const QRect geoRect,const int nbBuckets, QWidget *parent = nullptr);

    void updateBuckets();

private slots:
    void on_timer();
    void on_plateClicked(int id);

private:
    QWidget* createSynopticView ();
    QWidget* createCarouselLine(int nb_buckets, ConveyorSide side);
    void     setInitialState();

private:
    int   NB_BUCKETS;
    int   LINE_WIDTH;
    int   BUCKET_WIDTH;
    int   CAROUSEL_CURVES_WIDTH;
    int   CAROUSEL_LINE_HEIGHT;
    int   CAROUSEL_LINES_SPACING;

    int   m_bcsPosition;

    int   m_bucketsAvailableWidth;

    QVector<BucketPlate*> m_buckets;
    QVector<BucketPlateData>  m_buffer;
    QWidget*       m_backLine;
    QWidget*       m_frontLine;

    QWidget*       m_synoptic;
    QWidget*       m_synopticContainer;

    QWidget*       m_zoomHandle;
    QTimer*        m_timer;
};

#endif // BASICCAROUSEL_H
