#include "mainwindow.h"
#include "helperFunctions.h"
#include "calculate.h"

const QString fontName = "Arial";
const unsigned int initialRowCount = 1;
const unsigned int initialColCount = 20;
const int statsPrecision = 3;

QWidget *setupHeader(QWidget *parent, const int fontSize)
{
    const QFont headerFont(fontName, fontSize);

    QWidget *header = new QWidget(parent);
    header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QLabel *h1 = new QLabel("Statistics Visualizer", header);
    h1->setFont(headerFont);

    QHBoxLayout *headerAlignment = new QHBoxLayout(header);
    headerAlignment->setContentsMargins(0, 0, 0, 0);
    headerAlignment->addWidget(h1, 0, Qt::AlignCenter);
    return header;
}

void setupTableActions(const TableActions &actions)
{
    // Добавление строки
    Helper::connect(actions.addRowBtn, [=]()
                    {
                        actions.table->setRowCount(actions.table->rowCount() + 1);
                        actions.rowSpin->setValue(actions.table->rowCount()); });

    // Добавление столбца
    Helper::connect(actions.addColBtn, [=]()
                    {
                        actions.table->setColumnCount(actions.table->columnCount() + 1);
                        actions.colSpin->setValue(actions.table->columnCount()); });

    // Удаление строки
    Helper::connect(actions.delRowBtn, [=]()
                    {
                        if(actions.table->rowCount() > 1) {
                            actions.table->setRowCount(actions.table->rowCount() - 1);
                            actions.rowSpin->setValue(actions.table->rowCount());
                        } });

    // Удаление столбца
    Helper::connect(actions.delColBtn, [=]()
                    {
                        if(actions.table->columnCount() > 1) {
                            actions.table->setColumnCount(actions.table->columnCount() - 1);
                            actions.colSpin->setValue(actions.table->columnCount());
                        } });

    // Очистка таблицы
    Helper::connect(actions.clearButton, [=]()
                    {
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
                        } });

    // Авторазмер
    Helper::connect(actions.autoSizeBtn, [=]()
                    {
                        actions.table->resizeColumnsToContents();
                        actions.table->resizeRowsToContents(); });

    // Обработка изменения спинбокса строк
    Helper::connect(actions.rowSpin, [=](int value)
                    {
                        if (value >= actions.rowSpin->minimum()) {
                            actions.table->setRowCount(value);
                        } });

    // Обработка изменения спинбокса столбцов
    Helper::connect(actions.colSpin, [=](int value)
                    {
                        if (value >= actions.colSpin->minimum()) {
                            actions.table->setColumnCount(value);
                        } });
}

QWidget *setupTableToolbar(QWidget *parent, QTableWidget *table)
{
    QWidget *toolbar = new QWidget(parent);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(5);

    // Спинбоксы
    auto *rowsContainer = Helper::createSpinBoxWithLabel(toolbar, "Строки", 10, initialRowCount);
    auto *columnsContainer = Helper::createSpinBoxWithLabel(toolbar, "Столбцы", 512, initialColCount);
    QSpinBox *rowSpinBox = qobject_cast<QSpinBox *>(rowsContainer->layout()->itemAt(1)->widget());
    QSpinBox *colSpinBox = qobject_cast<QSpinBox *>(columnsContainer->layout()->itemAt(1)->widget());

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
                         .colSpin = colSpinBox};

    // Подключение функционала
    setupTableActions(actions);

    // Группируем элементы
    for (auto *item : {rowsContainer, columnsContainer})
    {
        toolbarLayout->addLayout(item);
    }
    for (auto *item : {actions.addRowBtn, actions.addColBtn, actions.delRowBtn, actions.delColBtn, actions.autoSizeBtn, actions.clearButton})
    {
        toolbarLayout->addWidget(item);
    }
    toolbarLayout->addStretch();

    return toolbar;
}

QTableWidget *setupTable(QWidget *parent)
{
    // Правая часть - таблица
    QTableWidget *table = new QTableWidget(initialRowCount, initialColCount, parent);
    Helper::setSizePolicyExpanding(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Колонки на всю ширину
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);   // Строки на всю высоту
    table->resizeColumnsToContents();
    return table;
}

QWidget *setupTablePanel(QWidget *parent, QTableWidget **outTable)
{
    QWidget *tableSection = new QWidget(parent);

    *outTable = setupTable(tableSection); // Создаем таблицу и возвращаем через outTable
    auto *tableToolbar = setupTableToolbar(tableSection, *outTable);

    QVBoxLayout *tableSectionLayout = new QVBoxLayout(tableSection);
    tableSectionLayout->addWidget(tableToolbar);
    tableSectionLayout->addWidget(*outTable);

    return tableSection;
}

QWidget *MainWindow::setupStatsPanel(QWidget *parent, QLabel **elementCountLabel,
                                     QLabel **sumLabel, QLabel **averageLabel)
{
    QWidget *statsPanel = new QWidget(parent);
    statsPanel->setLayout(new QVBoxLayout(statsPanel));
    Helper::setSizePolicyExpanding(statsPanel);

    QVBoxLayout *statsLayout = qobject_cast<QVBoxLayout *>(statsPanel->layout());
    statsLayout->setContentsMargins(12, 8, 12, 8);
    statsLayout->setSpacing(8);

    QLabel *mainHeader = new QLabel("Анализ данных", statsPanel);
    mainHeader->setStyleSheet("font-size: 16px; font-weight: 600; color: #2c3e50;");
    statsLayout->addWidget(mainHeader);

    // Секция базовой статистики
    QWidget *basicSection = Helper::createStatSection(statsPanel, "Основные метрики");
    QVBoxLayout *basicLayout = qobject_cast<QVBoxLayout *>(basicSection->layout());
    *elementCountLabel = Helper::createAndRegisterStatRow(basicSection, basicLayout, "Количество элементов", "0", "elementCountLabel");
    *sumLabel = Helper::createAndRegisterStatRow(basicSection, basicLayout, "Сумма", "0", "sumLabel");
    *averageLabel = Helper::createAndRegisterStatRow(basicSection, basicLayout, "Среднее арифметическое", "—", "averageLabel");
    statsLayout->addWidget(basicSection);

    // Секция средних
    QWidget *meansSection = Helper::createStatSection(statsPanel, "Средние");
    QVBoxLayout *meansLayout = qobject_cast<QVBoxLayout *>(meansSection->layout());
    m_geometricMeanLabel = Helper::createAndRegisterStatRow(meansSection, meansLayout, "Геом. среднее", "—", "geometricMeanLabel");
    m_harmonicMeanLabel = Helper::createAndRegisterStatRow(meansSection, meansLayout, "Гарм. среднее", "—", "harmonicMeanLabel");
    m_weightedMeanLabel = Helper::createAndRegisterStatRow(meansSection, meansLayout, "Взвеш. среднее", "—", "weightedMeanLabel");
    statsLayout->addWidget(meansSection);

    // Секция распределения
    QWidget *distributionSection = Helper::createStatSection(statsPanel, "Распределение");
    QVBoxLayout *distributionLayout = qobject_cast<QVBoxLayout *>(distributionSection->layout());
    m_medianLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Медиана", "—", "medianLabel");
    m_modeLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Мода", "—", "modeLabel");
    m_stdDevLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Стандартное отклонение", "—", "stdDevLabel");
    m_skewnessLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Асимметрия", "—", "skewnessLabel");
    m_kurtosisLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Эксцесс", "—", "kurtosisLabel");
    statsLayout->addWidget(distributionSection);

    // Секция экстремумов
    QWidget *extremesSection = Helper::createStatSection(statsPanel, "Экстремумы");
    QVBoxLayout *extremesLayout = qobject_cast<QVBoxLayout *>(extremesSection->layout());
    m_minLabel = Helper::createAndRegisterStatRow(extremesSection, extremesLayout, "Минимум", "—", "minLabel");
    m_maxLabel = Helper::createAndRegisterStatRow(extremesSection, extremesLayout, "Максимум", "—", "maxLabel");
    m_rangeLabel = Helper::createAndRegisterStatRow(extremesSection, extremesLayout, "Размах", "—", "rangeLabel");
    statsLayout->addWidget(extremesSection);

    statsLayout->addStretch();
    return statsPanel;
}

QWidget *MainWindow::setupDataSection(QWidget *parent)
{
    QWidget *dataSection = new QWidget(parent);
    dataSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Убрана фиксированная высота

    QHBoxLayout *dataSectionLayout = new QHBoxLayout(dataSection);
    dataSectionLayout->setContentsMargins(0, 0, 0, 0);

    auto *statsPanel = setupStatsPanel(dataSection, &m_elementCountLabel,
                                       &m_sumLabel, &m_averageLabel);

    // Создаем tablePanel и получаем таблицу
    QTableWidget *table = nullptr;
    auto *tablePanel = setupTablePanel(dataSection, &table);
    m_table = table; // Сохраняем таблицу

    dataSectionLayout->addWidget(statsPanel, 1);
    dataSectionLayout->addWidget(tablePanel, 1);

    return dataSection;
}

void MainWindow::updateStatistics()
{
    if (!m_table || !m_elementCountLabel || !m_sumLabel || !m_averageLabel ||
        !m_geometricMeanLabel || !m_medianLabel || !m_modeLabel || !m_stdDevLabel ||
        !m_minLabel || !m_maxLabel || !m_rangeLabel || !m_skewnessLabel ||
        !m_kurtosisLabel || !m_harmonicMeanLabel || !m_weightedMeanLabel)
        return;

    // Сбор данных
    QVector<double> values;
    int count = 0;
    double sum = 0.0;
    bool conversionOK;

    for (int row = 0; row < m_table->rowCount(); ++row)
    {
        for (int col = 0; col < m_table->columnCount(); ++col)
        {
            QTableWidgetItem *item = m_table->item(row, col);
            if (item && !item->text().isEmpty())
            {
                double value = item->text().toDouble(&conversionOK);
                if (conversionOK)
                {
                    values.append(value);
                    sum += value;
                    count++;
                }
            }
        }
    }

    // Расчёты
    const QVector<double> weights = Calculate::findWeights(m_table); // Автопоиск столбца

    const bool hasData = count > 0;
    const double mean = hasData ? Calculate::getMean(sum, count) : 0.0;
    const double geomMean = Calculate::geometricMean(values);
    const double harmonicMean = Calculate::harmonicMean(values);
    const double median = hasData ? Calculate::getMedian(values) : 0.0;
    const double mode = hasData ? Calculate::getMode(values) : 0.0;
    const double stdDev = hasData ? Calculate::getStandardDeviation(values, mean) : 0.0;
    const double skew = Calculate::skewness(values, mean, stdDev);
    const double kurt = Calculate::kurtosis(values, mean, stdDev);
    const double wMean = Calculate::weightedMean(values, weights);

    const bool validCalculation = !std::isnan(wMean) &&
                                  (values.size() == weights.size()) &&
                                  !values.isEmpty();

    double minValue = std::numeric_limits<double>::quiet_NaN();
    double maxValue = std::numeric_limits<double>::quiet_NaN();
    double range = std::numeric_limits<double>::quiet_NaN();
    if (hasData)
    {
        auto [minIt, maxIt] = std::minmax_element(values.begin(), values.end());
        minValue = *minIt;
        maxValue = *maxIt;
        range = maxValue - minValue;
    }

    // Обновление интерфейса
    m_elementCountLabel->setText(QString::number(count));
    m_sumLabel->setText(hasData ? QString::number(sum, 'f', statsPrecision) : "—");
    m_averageLabel->setText(hasData ? QString::number(mean, 'f', statsPrecision) : "—");
    m_geometricMeanLabel->setText((hasData && !std::isnan(geomMean)) ? QString::number(geomMean, 'f', statsPrecision) : "—");
    m_harmonicMeanLabel->setText((hasData && !std::isnan(harmonicMean)) ? QString::number(harmonicMean, 'f', statsPrecision) : "—");
    m_weightedMeanLabel->setText(validCalculation ? QString::number(wMean, 'f', statsPrecision) : "—");
    m_medianLabel->setText(hasData ? QString::number(median) : "-");
    m_modeLabel->setText(hasData && !std::isnan(mode) ? QString::number(mode, 'f', statsPrecision) : "—");
    m_stdDevLabel->setText(hasData && !std::isnan(stdDev) ? QString::number(stdDev, 'f', statsPrecision) : "—");
    m_skewnessLabel->setText((hasData && !std::isnan(skew)) ? QString::number(skew, 'f', statsPrecision) : "—");
    m_kurtosisLabel->setText((hasData && !std::isnan(kurt)) ? QString::number(kurt, 'f', statsPrecision) : "—");
    m_minLabel->setText(hasData ? QString::number(minValue, 'f', statsPrecision) : "—");
    m_maxLabel->setText(hasData ? QString::number(maxValue, 'f', statsPrecision) : "—");
    m_rangeLabel->setText(hasData ? QString::number(range, 'f', statsPrecision) : "—");

    qDebug() << "Weights:" << weights;
    qDebug() << "Weighted mean:" << wMean;
    qDebug() << "Sum weights:" << std::accumulate(weights.begin(), weights.end(), 0.0);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    auto *header = setupHeader(mainWidget, 20);
    auto *dataSection = setupDataSection(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(header);
    mainLayout->addWidget(dataSection, 1);

    // Проверка инициализации перед подключением сигналов
    if (m_table)
    {
        connect(m_table, &QTableWidget::itemChanged, this, &MainWindow::updateStatistics);
        connect(m_table, &QTableWidget::cellChanged, this, &MainWindow::updateStatistics);
        updateStatistics();
    }
    else
    {
        qFatal("Table initialization failed!");
    }

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
}
