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
    // Основной контейнер
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Отступы от краев окна
    setCentralWidget(mainWidget);

    // Хедер
    QWidget *header = setupHeader(mainWidget, 20);
    mainLayout->addWidget(header);

    // Основная секция с данными
    QWidget *dataSection = new QWidget(mainWidget);
    dataSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    dataSection->setMaximumHeight(600);
    dataSection->setStyleSheet("border: 1px solid green;"); // Для визуализации
    mainLayout->addWidget(dataSection, 1); // Занимаем оставшееся пространство

    // Горизонтальный layout для разделения на 2 части
    QHBoxLayout *dataSectionLayout = new QHBoxLayout(dataSection);
    dataSectionLayout->setContentsMargins(0, 0, 0, 0); // Убираем внутренние отступы

    // Левая часть - характеристики
    QWidget *statsPanel = new QWidget(dataSection);
    statsPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    statsPanel->setStyleSheet("border: 1px solid red;"); // Для визуализации

    QVBoxLayout *statsLayout = new QVBoxLayout(statsPanel);
    statsLayout->addWidget(new QLabel("Характеристики", statsPanel));
    // Здесь можно добавить другие элементы характеристик

    // Правая часть - таблица
    QTableWidget *table = new QTableWidget(1, 20, dataSection);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setStyleSheet("border: 1px solid blue;"); // Для визуализации

    // Распределяем пространство 1:1 между панелью и таблицей
    dataSectionLayout->addWidget(statsPanel, 1); // stretch factor = 1
    dataSectionLayout->addWidget(table, 1);      // stretch factor = 1

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
}
