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

void setupTableActions(QPushButton* addRowBtn,
                       QPushButton* addColBtn,
                       QPushButton* delRowBtn,
                       QPushButton* delColBtn,
                       QPushButton* clearButton,
                       QPushButton* autoSizeBtn,
                       QTableWidget* table,
                       QSpinBox* rowSpin,
                       QSpinBox* colSpin)
{
    // Добавление строки
    QObject::connect(addRowBtn, &QPushButton::clicked, [=](){
        table->setRowCount(table->rowCount() + 1);
        rowSpin->setValue(table->rowCount());
    });

    // Добавление столбца
    QObject::connect(addColBtn, &QPushButton::clicked, [=](){
        table->setColumnCount(table->columnCount() + 1);
        colSpin->setValue(table->columnCount());
    });

    // Удаление строки
    QObject::connect(delRowBtn, &QPushButton::clicked, [=](){
        if(table->rowCount() > 1) {
            table->setRowCount(table->rowCount() - 1);
            rowSpin->setValue(table->rowCount());
        }
    });

    // Удаление столбца
    QObject::connect(delColBtn, &QPushButton::clicked, [=](){
        if(table->columnCount() > 1) {
            table->setColumnCount(table->columnCount() - 1);
            colSpin->setValue(table->columnCount());
        }
    });

    // Очистка таблицы
    QObject::connect(clearButton, &QPushButton::clicked, [=](){
        auto reply = QMessageBox::question(
            table,
            "Очистка таблицы",
            "Удалить все данные?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            table->clearContents();
            rowSpin->setValue(rowSpin->minimum());
            colSpin->setValue(colSpin->minimum());
        }
    });


    // Автоматическое изменение размера
    QObject::connect(autoSizeBtn, &QPushButton::clicked, [=](){
        table->resizeColumnsToContents();
        table->resizeRowsToContents();
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

    // Основные действия
    QPushButton *addRowBtn = Helper::createToolButton("Добавить строку", "add-row");
    QPushButton *addColBtn = Helper::createToolButton("Добавить столбец", "add-column");
    QPushButton *delRowBtn = Helper::createToolButton("Удалить строку", "delete-row");
    QPushButton *delColBtn = Helper::createToolButton("Удалить столбец", "delete-column");
    QPushButton *clearButton = Helper::createToolButton("Очистить", "clear-button");

    // Форматирование
    QPushButton *autoSizeBtn = Helper::createToolButton("Авторазмер", "auto-size");

    // Подключение функционала кнопок
    setupTableActions(addRowBtn, addColBtn, delRowBtn, delColBtn, clearButton, autoSizeBtn, table, rowSpinBox, colSpinBox);

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
    toolbarLayout->addWidget(clearButton);
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
