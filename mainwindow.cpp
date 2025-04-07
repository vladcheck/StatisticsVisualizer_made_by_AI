#include "mainwindow.h"

const QString fontName = "Arial";

QWidget* setupHeader(QWidget *parent, const int fontSize) {
    const QFont headerFont(fontName, fontSize);

    QWidget *header = new QWidget(parent);
    header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QLabel *h1 = new QLabel("Statistics Visualizer", header);
    h1->setFont(headerFont);

    QHBoxLayout *headerAlignment = new QHBoxLayout(header);
    headerAlignment->setContentsMargins(0,0,0,0);
    headerAlignment->addWidget(h1, 0, Qt::AlignCenter);
    return header;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    QWidget *header = setupHeader(centralWidget, 20);
    mainLayout->addWidget(header);

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
