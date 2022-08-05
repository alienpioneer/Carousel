#include "MainWindow.h"
#include "SingleLevelCarousel.h"
#include "BasicCarousel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setGeometry(QRect(0, 0, 800, 500));

    //SingleLevelCarousel* singleLevelCarousel = new SingleLevelCarousel(QRect(20, 20, 740, 182), 400, this);

    BasicCarousel* carousel = new BasicCarousel(QRect(20, 20, 740, 182), 400, this);

}

MainWindow::~MainWindow()
{
}

