#include "MainWindow.h"
#include "SingleLevelCarousel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setGeometry(QRect(0, 0, 800, 500));

    SingleLevelCarousel* carousel = new SingleLevelCarousel(QRect(20, 20, 740, 182), 400, this);

}

MainWindow::~MainWindow()
{
}

