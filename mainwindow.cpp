#include "mainwindow.h"
#include "helperFunctions.h"

const QString fontName = "Arial";
const unsigned int initialRowCount = 1;
const unsigned int initialColCount = 20;

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
    Helper::connect(actions.addRowBtn, [=](){
        actions.table->setRowCount(actions.table->rowCount() + 1);
        actions.rowSpin->setValue(actions.table->rowCount());
    });

    // Добавление столбца
    Helper::connect(actions.addColBtn, [=](){
        actions.table->setColumnCount(actions.table->columnCount() + 1);
        actions.colSpin->setValue(actions.table->columnCount());
    });

    // Удаление строки
    Helper::connect(actions.delRowBtn, [=](){
        if(actions.table->rowCount() > 1) {
            actions.table->setRowCount(actions.table->rowCount() - 1);
            actions.rowSpin->setValue(actions.table->rowCount());
        }
    });

    // Удаление столбца
    Helper::connect(actions.delColBtn, [=](){
        if(actions.table->columnCount() > 1) {
            actions.table->setColumnCount(actions.table->columnCount() - 1);
            actions.colSpin->setValue(actions.table->columnCount());
        }
    });

    // Очистка таблицы
    Helper::connect(actions.clearButton, [=](){
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
    Helper::connect(actions.autoSizeBtn, [=](){
        actions.table->resizeColumnsToContents();
        actions.table->resizeRowsToContents();
    });

    // Обработка изменения спинбокса строк
    Helper::connect(actions.rowSpin, [=](int value) {
        if (value >= actions.rowSpin->minimum()) {
            actions.table->setRowCount(value);
        }
    });

    // Обработка изменения спинбокса столбцов
    Helper::connect(actions.colSpin, [=](int value) {
        if (value >= actions.colSpin->minimum()) {
            actions.table->setColumnCount(value);
        }
    });
}

QWidget* setupTableToolbar(QWidget *parent, QTableWidget* table) {
    QWidget *toolbar = new QWidget(parent);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(5);

    // Спинбоксы
    auto* rowsContainer = Helper::createSpinBoxWithLabel(toolbar, "Строки", 10, initialRowCount);
    auto* columnsContainer = Helper::createSpinBoxWithLabel(toolbar, "Столбцы", 512, initialColCount);
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
    for (auto *item : {rowsContainer,columnsContainer}) {
        toolbarLayout->addLayout(item);
    }
    for (auto *item : {actions.addRowBtn,actions.addColBtn,actions.delRowBtn,actions.delColBtn,actions.autoSizeBtn,actions.clearButton}) {
        toolbarLayout->addWidget(item);
    }
    toolbarLayout->addStretch();

    return toolbar;
}

QTableWidget* setupTable(QWidget *parent) {
    // Правая часть - таблица
    QTableWidget *table = new QTableWidget(initialRowCount, initialColCount, parent);
    Helper::setSizePolicyExpanding(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Колонки на всю ширину
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);    // Строки на всю высоту
    table->resizeColumnsToContents();
    return table;
}

QWidget* setupTablePanel(QWidget *parent, QTableWidget** outTable) {
    QWidget *tableSection = new QWidget(parent);

    *outTable = setupTable(tableSection); // Создаем таблицу и возвращаем через outTable
    auto *tableToolbar = setupTableToolbar(tableSection, *outTable);

    QVBoxLayout *tableSectionLayout = new QVBoxLayout(tableSection);
    tableSectionLayout->addWidget(tableToolbar);
    tableSectionLayout->addWidget(*outTable);

    return tableSection;
}

QWidget* MainWindow::setupStatsPanel(QWidget* parent, QLabel** elementCountLabel,
                                     QLabel** sumLabel, QLabel** averageLabel) {
    QWidget* statsPanel = new QWidget(parent);
    statsPanel->setLayout(new QVBoxLayout(statsPanel));
    Helper::setSizePolicyExpanding(statsPanel);

    QVBoxLayout* statsLayout = qobject_cast<QVBoxLayout*>(statsPanel->layout());
    statsLayout->setContentsMargins(12, 8, 12, 8);
    statsLayout->setSpacing(8);

    QLabel* mainHeader = new QLabel("Анализ данных", statsPanel);
    mainHeader->setStyleSheet("font-size: 16px; font-weight: 600; color: #2c3e50;");
    statsLayout->addWidget(mainHeader);

    QWidget* basicSection = Helper::createStatSection(statsPanel, "Основные метрики");
    QVBoxLayout* basicLayout = qobject_cast<QVBoxLayout*>(basicSection->layout());
    statsLayout->addWidget(basicSection);

    // Секция базовой статистики
    *elementCountLabel = Helper::createAndRegisterStatRow(basicSection, basicLayout, "Элементов", "0", "elementCountLabel");
    *sumLabel = Helper::createAndRegisterStatRow(basicSection, basicLayout, "Сумма", "0", "sumLabel");
    *averageLabel = Helper::createAndRegisterStatRow(basicSection, basicLayout, "Среднее", "—", "averageLabel");

    // Секция распределения
    QWidget* distributionSection = Helper::createStatSection(statsPanel, "Распределение");
    QVBoxLayout* distributionLayout = qobject_cast<QVBoxLayout*>(distributionSection->layout());
    Helper::addStatRows(distributionSection, distributionLayout,
    {
        {"Медиана", "—"},
        {"Мода", "—"},
        {"Стандартное отклонение", "—"}
    });
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

QWidget* MainWindow::setupDataSection(QWidget *parent) {
    QWidget *dataSection = new QWidget(parent);
    dataSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Убрана фиксированная высота

    QHBoxLayout *dataSectionLayout = new QHBoxLayout(dataSection);
    dataSectionLayout->setContentsMargins(0, 0, 0, 0);

    auto *statsPanel = setupStatsPanel(dataSection, &m_elementCountLabel,
                                       &m_sumLabel, &m_averageLabel);

    // Создаем tablePanel и получаем таблицу
    QTableWidget* table = nullptr;
    auto *tablePanel = setupTablePanel(dataSection, &table);
    m_table = table; // Сохраняем таблицу

    dataSectionLayout->addWidget(statsPanel, 1);
    dataSectionLayout->addWidget(tablePanel, 1);

    return dataSection;
}

void MainWindow::updateStatistics() {
    if (!m_table || !m_elementCountLabel || !m_sumLabel || !m_averageLabel) return;

    // Сбор данных
    QVector<double> values;
    int count = 0;
    double sum = 0.0;
    bool conversionOK;

    for (int row = 0; row < m_table->rowCount(); ++row) {
        for (int col = 0; col < m_table->columnCount(); ++col) {
            QTableWidgetItem* item = m_table->item(row, col);
            if (item && !item->text().isEmpty()) {
                double value = item->text().toDouble(&conversionOK);
                if (conversionOK) {
                    values.append(value);
                    sum += value;
                    count++;
                }
            }
        }
    }

    // Расчёты
    const bool hasData = count > 0;
    m_elementCountLabel->setText(QString::number(count));
    m_sumLabel->setText(hasData ? QString::number(sum, 'f', 2) : "—");
    m_averageLabel->setText(hasData ? QString::number(sum/count, 'f', 2) : "—");
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    auto *header = setupHeader(mainWidget, 20);
    auto *dataSection = setupDataSection(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(header);
    mainLayout->addWidget(dataSection, 1);

    // Проверка инициализации перед подключением сигналов
    if (m_table) {
        connect(m_table, &QTableWidget::itemChanged, this, &MainWindow::updateStatistics);
        connect(m_table, &QTableWidget::cellChanged, this, &MainWindow::updateStatistics);
        updateStatistics();
    } else {
        qFatal("Table initialization failed!");
    }

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
}
