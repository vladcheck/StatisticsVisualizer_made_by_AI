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

void setupTableActions(const TableActions& actions) {
    // Добавление строки
    QObject::connect(actions.addRowBtn, &QPushButton::clicked, [=](){
        actions.table->setRowCount(actions.table->rowCount() + 1);
        actions.rowSpin->setValue(actions.table->rowCount());
    });

    // Добавление столбца
    QObject::connect(actions.addColBtn, &QPushButton::clicked, [=](){
        actions.table->setColumnCount(actions.table->columnCount() + 1);
        actions.colSpin->setValue(actions.table->columnCount());
    });

    // Удаление строки
    QObject::connect(actions.delRowBtn, &QPushButton::clicked, [=](){
        if(actions.table->rowCount() > 1) {
            actions.table->setRowCount(actions.table->rowCount() - 1);
            actions.rowSpin->setValue(actions.table->rowCount());
        }
    });

    // Удаление столбца
    QObject::connect(actions.delColBtn, &QPushButton::clicked, [=](){
        if(actions.table->columnCount() > 1) {
            actions.table->setColumnCount(actions.table->columnCount() - 1);
            actions.colSpin->setValue(actions.table->columnCount());
        }
    });

    // Очистка таблицы
    QObject::connect(actions.clearButton, &QPushButton::clicked, [=](){
        auto reply = QMessageBox::question(
            actions.table,
            "Очистка таблицы",
            "Удалить все данные?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            actions.table->clearContents();
            actions.rowSpin->setValue(actions.rowSpin->minimum());
            actions.colSpin->setValue(actions.colSpin->minimum());
        }
    });

    // Авторазмер
    QObject::connect(actions.autoSizeBtn, &QPushButton::clicked, [=](){
        actions.table->resizeColumnsToContents();
        actions.table->resizeRowsToContents();
    });
}

QWidget* setupTableToolbar(QWidget *parent, QTableWidget* table) {
    QWidget *toolbar = new QWidget(parent);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(5);

    // Спинбоксы
    auto* rowsContainer = Helper::createSpinBoxWithLabel(toolbar, "Строки", 10);
    auto* columnsContainer = Helper::createSpinBoxWithLabel(toolbar, "Столбцы", 512);
    QSpinBox* rowSpinBox = qobject_cast<QSpinBox*>(rowsContainer->layout()->itemAt(1)->widget());
    QSpinBox* colSpinBox = qobject_cast<QSpinBox*>(columnsContainer->layout()->itemAt(1)->widget());

    // Создаем и настраиваем структуру
    TableActions actions{
        .addRowBtn = Helper::createToolButton("Добавить строку", "add-row"),
        .addColBtn = Helper::createToolButton("Добавить столбец", "add-column"),
        .delRowBtn = Helper::createToolButton("Удалить строку", "delete-row"),
        .delColBtn = Helper::createToolButton("Удалить столбец", "delete-column"),
        .clearButton = Helper::createToolButton("Очистить", "clear-button"),
        .autoSizeBtn = Helper::createToolButton("Авторазмер", "auto-size"),
        .table = table,
        .rowSpin = rowSpinBox,
        .colSpin = colSpinBox
    };

    // Подключение функционала
    setupTableActions(actions);

    // Группируем элементы
    toolbarLayout->addLayout(rowsContainer);
    toolbarLayout->addLayout(columnsContainer);
    toolbarLayout->addWidget(Helper::createSeparator());
    toolbarLayout->addWidget(actions.addRowBtn);
    toolbarLayout->addWidget(actions.addColBtn);
    toolbarLayout->addWidget(actions.delRowBtn);
    toolbarLayout->addWidget(actions.delColBtn);
    toolbarLayout->addWidget(Helper::createSeparator());
    toolbarLayout->addWidget(actions.autoSizeBtn);
    toolbarLayout->addWidget(Helper::createSeparator());
    toolbarLayout->addWidget(actions.clearButton);
    toolbarLayout->addStretch();

    return toolbar;
}

QTableWidget* setupTable(QWidget *parent) {
    // Правая часть - таблица
    QTableWidget *table = new QTableWidget(1, 20, parent);
    Helper::setSizePolicyExpanding(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Колонки на всю ширину
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);    // Строки на всю высоту
    table->resizeColumnsToContents();
    return table;
}

QWidget* setupTablePanel(QWidget *parent) {
    QWidget *tableSection = new QWidget(parent);

    auto *table = setupTable(tableSection);
    auto *tableToolbar = setupTableToolbar(tableSection,table);

    QVBoxLayout *tableSectionLayout = new QVBoxLayout(tableSection);
    tableSectionLayout->addWidget(tableToolbar);
    tableSectionLayout->addWidget(table);

    return tableSection;
}

QWidget* setupStatsPanel(QWidget* parent) {
    QWidget* statsPanel = new QWidget(parent);
    statsPanel->setLayout(new QVBoxLayout(statsPanel));
    Helper::setSizePolicyExpanding(statsPanel);

    QVBoxLayout* statsLayout = qobject_cast<QVBoxLayout*>(statsPanel->layout());
    statsLayout->setContentsMargins(12, 8, 12, 8);
    statsLayout->setSpacing(8);

    QLabel* mainHeader = new QLabel("Анализ данных", statsPanel);
    mainHeader->setStyleSheet("font-size: 16px; font-weight: 600; color: #2c3e50;");
    statsLayout->addWidget(mainHeader);

    // Секция базовой статистики
    QWidget* basicSection = Helper::createStatSection(statsPanel, "Основные метрики");
    QVBoxLayout* basicLayout = qobject_cast<QVBoxLayout*>(basicSection->layout()); // Используем существующий layout
    basicLayout->addWidget(Helper::createStatRow(basicSection, "Элементов", "0"));
    basicLayout->addWidget(Helper::createStatRow(basicSection, "Сумма", "—"));
    basicLayout->addWidget(Helper::createStatRow(basicSection, "Среднее", "—"));
    statsLayout->addWidget(basicSection);

    // Секция распределения
    QWidget* distributionSection = Helper::createStatSection(statsPanel, "Распределение");
    QVBoxLayout* distributionLayout = qobject_cast<QVBoxLayout*>(distributionSection->layout());
    distributionLayout->addWidget(Helper::createStatRow(distributionSection, "Медиана", "—"));
    distributionLayout->addWidget(Helper::createStatRow(distributionSection, "Мода", "—"));
    distributionLayout->addWidget(Helper::createStatRow(distributionSection, "Стандартное отклонение", "—"));
    statsLayout->addWidget(distributionSection);

    // Секция экстремумов
    QWidget* extremesSection = Helper::createStatSection(statsPanel, "Экстремумы");
    QVBoxLayout* extremesLayout = qobject_cast<QVBoxLayout*>(extremesSection->layout());
    extremesLayout->addWidget(Helper::createStatRow(extremesSection, "Минимум", "—"));
    extremesLayout->addWidget(Helper::createStatRow(extremesSection, "Максимум", "—"));
    extremesLayout->addWidget(Helper::createStatRow(extremesSection, "Размах", "—"));
    statsLayout->addWidget(extremesSection);

    statsLayout->addStretch();

    return statsPanel;
}

QWidget *setupDataSection(QWidget *parent) {
    QWidget *dataSection = new QWidget(parent);
    dataSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dataSection->setMaximumHeight(600);

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
