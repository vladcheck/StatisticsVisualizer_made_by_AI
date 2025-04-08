#include "mainwindow.h"
#include "helperFunctions.h"

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

QWidget* setupTableToolbar(QWidget *parent) {
    QWidget *toolbar = new QWidget(parent);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Спинбоксы
    auto* rowsContainer = Helper::createSpinBoxWithLabel(toolbar, "Строки", 1000);
    auto* columnsContainer = Helper::createSpinBoxWithLabel(toolbar, "Столбцы", 100);

    // Основные действия
    QPushButton *addRowBtn = Helper::createToolButton("Добавить строку", "add_row");
    QPushButton *addColBtn = Helper::createToolButton("Добавить столбец", "add_col");
    QPushButton *delRowBtn = Helper::createToolButton("Удалить строку", "del_row");
    QPushButton *delColBtn = Helper::createToolButton("Удалить столбец", "del_col");

    // Форматирование
    QPushButton *autoSizeBtn = Helper::createToolButton("Авторазмер", "auto_size");

    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(5);

    // Группируем элементы
    toolbarLayout->addLayout(rowsContainer);
    toolbarLayout->addLayout(columnsContainer);
    toolbarLayout->addWidget(Helper::createSeparator());
    toolbarLayout->addWidget(addRowBtn);
    toolbarLayout->addWidget(addColBtn);
    toolbarLayout->addWidget(delRowBtn);
    toolbarLayout->addWidget(delColBtn);
    toolbarLayout->addWidget(Helper::createSeparator());
    toolbarLayout->addWidget(autoSizeBtn);
    toolbarLayout->addWidget(Helper::createSeparator());
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(Helper::createClearButton(parent));

    return toolbar;
}

QTableWidget* setupTable(QWidget *parent) {
    // Правая часть - таблица
    QTableWidget *table = new QTableWidget(3, 20, parent);
    Helper::setSizePolicyExpanding(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Колонки на всю ширину
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);    // Строки на всю высоту
    table->setStyleSheet("border: 1px solid blue;"); // Для визуализации
    table->resizeColumnsToContents();
    return table;
}

QWidget* setupTablePanel(QWidget *parent) {
    QWidget *tableSection = new QWidget(parent);

    auto *tableToolbar = setupTableToolbar(tableSection);
    auto *table = setupTable(tableSection);

    QVBoxLayout *tableSectionLayout = new QVBoxLayout(tableSection);
    tableSectionLayout->addWidget(tableToolbar);
    tableSectionLayout->addWidget(table);

    return tableSection;
}

QWidget *setupStatsPanel(QWidget *parent) {
    QWidget *statsPanel = new QWidget(parent);
    Helper::setSizePolicyExpanding(statsPanel);
    statsPanel->setStyleSheet("border: 1px solid red;"); // Для визуализации

    QVBoxLayout *statsLayout = new QVBoxLayout(statsPanel);
    statsLayout->addWidget(new QLabel("Характеристики", statsPanel));
    return statsPanel;
}

QWidget *setupDataSection(QWidget *parent) {
    QWidget *dataSection = new QWidget(parent);
    dataSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dataSection->setMaximumHeight(600);
    dataSection->setStyleSheet("border: 1px solid green;"); // Для визуализации

    // Горизонтальный layout для разделения на 2 части
    QHBoxLayout *dataSectionLayout = new QHBoxLayout(dataSection);
    dataSectionLayout->setContentsMargins(0, 0, 0, 0); // Убираем внутренние отступы

    auto *statsPanel = setupStatsPanel(dataSection);
    auto *tablePanel = setupTablePanel(dataSection);

    // Распределяем пространство 1:1 между панелью и таблицей
    dataSectionLayout->addWidget(statsPanel, 1); // stretch factor = 1
    dataSectionLayout->addWidget(tablePanel, 1);      // stretch factor = 1
    return dataSection;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Основной контейнер
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    auto *header = setupHeader(mainWidget, 20); // Хедер
    auto *dataSection = setupDataSection(mainWidget);     // Основная секция с данными

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Отступы от краев окна
    mainLayout->addWidget(header);
    mainLayout->addWidget(dataSection, 1); // Занимаем оставшееся пространство

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
}
