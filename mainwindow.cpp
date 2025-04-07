#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    QTableWidget *table = new QTableWidget(1,10,centralWidget);
    layout->addWidget(table);
}

MainWindow::~MainWindow()
{
}
