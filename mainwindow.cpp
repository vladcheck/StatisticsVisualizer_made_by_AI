#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    QWidget *tableGroup = new QWidget(centralWidget);
    mainLayout->addWidget(tableGroup);

    QTableWidget *table = new QTableWidget(1,20,tableGroup);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    QHBoxLayout *tableLayout = new QHBoxLayout(tableGroup);
    tableLayout->addWidget(table);

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
}
