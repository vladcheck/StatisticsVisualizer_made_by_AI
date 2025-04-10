#include "mainwindow.h"
#include "helperFunctions.h"
#include "calculate.h"

const QString fontName = "Arial";
const QString na = "—";
const unsigned int initialRowCount = 1;
const unsigned int initialColCount = 20;
const int statsPrecision = 3;
const float trimmedMeanPercentage = 0.1;

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
                         .clearButton = Helper::createToolButton("Очистить", "clear"),
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
    m_rmsLabel = Helper::createAndRegisterStatRow(meansSection, meansLayout, "Квадр. среднее", "—", "rmsLabel");
    m_trimmedMeanLabel = Helper::createAndRegisterStatRow(meansSection, meansLayout, "Усеч. среднее", "—", "trimmedMeanLabel");
    statsLayout->addWidget(meansSection);

    // Секция распределения
    QWidget *distributionSection = Helper::createStatSection(statsPanel, "Распределение");
    QVBoxLayout *distributionLayout = qobject_cast<QVBoxLayout *>(distributionSection->layout());
    m_medianLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Медиана", "—", "medianLabel");
    m_modeLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Мода", "—", "modeLabel");
    m_stdDevLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Стандартное отклонение", "—", "stdDevLabel");
    m_skewnessLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Асимметрия", "—", "skewnessLabel");
    m_kurtosisLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Эксцесс", "—", "kurtosisLabel");
    m_madLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Медианное абсолютное отклонение", "—", "madLabel");
    m_robustStdLabel = Helper::createAndRegisterStatRow(distributionSection, distributionLayout, "Робастный стандартный разброс", "—", "robustStdLabel");
    statsLayout->addWidget(distributionSection);

    // Секция экстремумов
    QWidget *extremesSection = Helper::createStatSection(statsPanel, "Экстремумы");
    QVBoxLayout *extremesLayout = qobject_cast<QVBoxLayout *>(extremesSection->layout());
    m_minLabel = Helper::createAndRegisterStatRow(extremesSection, extremesLayout, "Минимум", "—", "minLabel");
    m_maxLabel = Helper::createAndRegisterStatRow(extremesSection, extremesLayout, "Максимум", "—", "maxLabel");
    m_rangeLabel = Helper::createAndRegisterStatRow(extremesSection, extremesLayout, "Размах", "—", "rangeLabel");
    statsLayout->addWidget(extremesSection);

    // Секция категориальных данных
    QWidget* categoricalSection = Helper::createStatSection(statsPanel, "Категориальные данные");
    QVBoxLayout* categoricalLayout = qobject_cast<QVBoxLayout*>(categoricalSection->layout());
    m_modalFreqLabel = Helper::createAndRegisterStatRow(categoricalSection, categoricalLayout, "Модальная частота", "—", "modalFreqLabel");
    m_simpsonIndexLabel = Helper::createAndRegisterStatRow(categoricalSection, categoricalLayout, "Индекс Симпсона", "—", "simpsonIndexLabel");
    m_uniqueRatioLabel = Helper::createAndRegisterStatRow(categoricalSection, categoricalLayout, "Доля уникальных", "—", "uniqueRatioLabel");
    statsLayout->addWidget(categoricalSection);

    // Секция анализа взаимосвязий
    QWidget* correlationSection = Helper::createStatSection(statsPanel, "Анализ взаимосвязей");
    QVBoxLayout* correlationLayout = qobject_cast<QVBoxLayout*>(correlationSection->layout());
    m_covarianceLabel = Helper::createAndRegisterStatRow(correlationSection, correlationLayout, "Ковариация", "—", "covarianceLabel");
    m_pearsonLabel = Helper::createAndRegisterStatRow(correlationSection, correlationLayout, "Пирсон", "—", "pearsonLabel");
    m_spearmanLabel = Helper::createAndRegisterStatRow(correlationSection, correlationLayout, "Спирмен", "—", "spearmanLabel");
    m_kendallLabel = Helper::createAndRegisterStatRow(correlationSection, correlationLayout, "Кендалл", "—", "kendallLabel");
    statsLayout->addWidget(correlationSection);

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

void MainWindow::getTableValues(QVector<double>& values, int& count, double& sum) {
    values.clear();
    count = 0;
    sum = 0.0;

    for (int row = 0; row < m_table->rowCount(); ++row) {
        QTableWidgetItem* item = m_table->item(row, 0);
        if (item && !item->text().isEmpty()) {
            bool ok;
            double value = item->text().toDouble(&ok);
            if (ok) {
                values.append(value);
                sum += value;
                count++;
            }
        }
    }
}

bool areWeightsValid(QVector<double> weights, QVector<double> values) {
    return (weights.size() == values.size()) && !weights.isEmpty();
}

void MainWindow::getCategorialData(QVector<QString> &categories) {
    if (m_table->columnCount() > 2) {
        for (int row = 0; row < m_table->rowCount(); ++row) {
            QTableWidgetItem* item = m_table->item(row, 2);
            if (item) categories.append(item->text());
        }
    }
}

void MainWindow::getCorrelationalData(QVector<double>& xData, QVector<double>& yData, int xColumn, int yColumn)
{
    xData.clear();
    yData.clear();

    if (m_table && m_table->columnCount() > qMax(xColumn, yColumn))
    {
        for (int row = 0; row < m_table->rowCount(); ++row)
        {
            bool okX = false, okY = false;
            double x = 0.0, y = 0.0;

            QTableWidgetItem* itemX = m_table->item(row, xColumn);
            QTableWidgetItem* itemY = m_table->item(row, yColumn);

            if (itemX && itemY)
            {
                x = itemX->text().toDouble(&okX);
                y = itemY->text().toDouble(&okY);
            }

            if (okX && okY)
            {
                xData.append(x);
                yData.append(y);
            }
        }
    }
}

bool MainWindow::hasPairs(const QVector<double>& xData) const {
    return xData.size() >= 2;
}

bool MainWindow::hasValidSpearman(const QVector<double>& xData) const {
    return xData.size() >= 3;
}

bool MainWindow::hasValidKendall(const QVector<double>& xData) const {
    return xData.size() >= 2;
}

bool MainWindow::hasCatData(const QVector<QString>& categories) const {
    return !categories.isEmpty();
}

bool MainWindow::areAllLabelsDefined() {
    return (m_table &&
            m_elementCountLabel &&
            m_sumLabel &&
            m_averageLabel &&
            m_geometricMeanLabel &&
            m_medianLabel &&
            m_modeLabel &&
            m_stdDevLabel &&
            m_minLabel &&
            m_maxLabel &&
            m_rangeLabel &&
            m_skewnessLabel &&
            m_kurtosisLabel &&
            m_harmonicMeanLabel &&
            m_weightedMeanLabel &&
            m_rmsLabel &&
            m_trimmedMeanLabel &&
            m_robustStdLabel &&
            m_madLabel &&
            m_modalFreqLabel &&
            m_simpsonIndexLabel &&
            m_uniqueRatioLabel &&
            m_covarianceLabel &&
            m_spearmanLabel &&
            m_kendallLabel &&
            m_pearsonLabel);
}

void MainWindow::updateStatistics()
{
    if (!areAllLabelsDefined()) {
        qWarning() << "Some labels are not initialized!";
        return;
    }

    // Сбор данных только из первого столбца (значения)
    QVector<double> values;
    int count = 0;
    double sum = 0.0;
    getTableValues(values, count, sum);

    // Получение весов с проверкой размера
    const QVector<double> weights = Calculate::findWeights(m_table);

    // Основные расчёты
    const bool hasData = count > 0;
    const double mean = hasData ? sum / count : 0.0;
    const double geomMean = Calculate::geometricMean(values);
    const double harmonicMean = Calculate::harmonicMean(values);
    const double wMean = areWeightsValid(weights,values) ? Calculate::weightedMean(values, weights) : std::numeric_limits<double>::quiet_NaN();
    const double rms = Calculate::rootMeanSquare(values);
    const double mad = Calculate::medianAbsoluteDeviation(values);
    const double median = Calculate::getMedian(values);
    const double mode = Calculate::getMode(values);
    const double stdDev = Calculate::getStandardDeviation(values, mean);
    const double tMean = Calculate::trimmedMean(values, trimmedMeanPercentage);
    const double skew = Calculate::skewness(values, mean, stdDev);
    const double kurt = Calculate::kurtosis(values, mean, stdDev);
    const double robustStd = Calculate::robustStandardDeviation(values);

    // Расчёты для категориальных данных (столбец 2)
    QVector<QString> categories;
    getCategorialData(categories);

    // Расчёты для корреляций (столбцы 0 и 1)
    QVector<double> xData, yData;
    getCorrelationalData(xData,yData,0,1);

    // Обновление интерфейса
    m_elementCountLabel->setText(hasData ? QString::number(count) : na);
    m_sumLabel->setText(hasData ? QString::number(sum, 'f', statsPrecision) : na);
    m_averageLabel->setText(hasData ? QString::number(mean, 'f', statsPrecision) : na);
    m_geometricMeanLabel->setText(hasData ? QString::number(geomMean, 'f', statsPrecision) : na);
    m_harmonicMeanLabel->setText(hasData ? QString::number(harmonicMean, 'f', statsPrecision) : na);
    m_weightedMeanLabel->setText(areWeightsValid(weights,values) ? QString::number(wMean, 'f', statsPrecision) : na);
    m_rmsLabel->setText(hasData ? QString::number(rms, 'f', statsPrecision) : na);
    m_trimmedMeanLabel->setText(hasData ? QString::number(tMean, 'f', statsPrecision) : na);
    m_robustStdLabel->setText(hasData ? QString::number(robustStd, 'f', statsPrecision) : na);
    m_madLabel->setText(hasData ? QString::number(mad, 'f', statsPrecision) : na);
    m_medianLabel->setText(hasData ? QString::number(median, 'f', statsPrecision) : na);
    m_modeLabel->setText(hasData ? QString::number(mode, 'f', statsPrecision) : na);
    m_stdDevLabel->setText(hasData ? QString::number(stdDev, 'f', statsPrecision) : na);
    m_skewnessLabel->setText(hasData ? QString::number(skew, 'f', statsPrecision) : na);
    m_kurtosisLabel->setText(hasData ? QString::number(kurt, 'f', statsPrecision) : na);

    // Обновление категориальных метрик
    m_modalFreqLabel->setText(hasCatData(categories) ? QString::number(Calculate::modalFrequency(categories), 'f', statsPrecision) : na);
    m_simpsonIndexLabel->setText(hasCatData(categories) ? QString::number(Calculate::simpsonDiversityIndex(categories), 'f', statsPrecision) : na);
    m_uniqueRatioLabel->setText(hasCatData(categories) ? QString::number(Calculate::uniqueValueRatio(categories), 'f', statsPrecision) : na);

    // Обновление корреляций
    const double pearson = hasPairs(xData) ? Calculate::pearsonCorrelation(xData, yData) : NAN;
    const double spearman = hasValidSpearman(xData) ? Calculate::spearmanCorrelation(xData, yData) : NAN;
    const double kendall = hasValidKendall(xData) ? Calculate::kendallCorrelation(xData, yData) : NAN;
    const double cov = hasPairs(xData) ? Calculate::covariance(xData, yData) : NAN;

    m_covarianceLabel->setText(hasPairs(xData) ? QString::number(cov, 'f', statsPrecision) : na);
    m_pearsonLabel->setText(hasPairs(xData) ? QString::number(pearson, 'f', statsPrecision) : na);
    m_spearmanLabel->setText(hasValidSpearman(xData) ? QString::number(spearman, 'f', statsPrecision) : na);
    m_kendallLabel->setText(hasValidKendall(xData) ? QString::number(kendall, 'f', statsPrecision) : na);
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
    this->setWindowIcon(QIcon(":/logo.png"));
}

MainWindow::~MainWindow() {}
