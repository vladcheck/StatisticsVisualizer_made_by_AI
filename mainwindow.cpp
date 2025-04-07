#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    QTableWidget *table = new QTableWidget(1,20,centralWidget);
    table->resizeColumnsToContents();
    layout->addWidget(table);

    this->setWindowState(Qt::WindowMaximized);
    centralWidget->adjustSize(); // Меняет размер окна под размер содержимого
}

MainWindow::~MainWindow()
{
}
