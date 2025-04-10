#include "mainwindow.h"
#include "globals.h"
#include "draw.h"
#include "calculate.h"
#include "tableactions.h"

void MainWindow::createDataHeader(QWidget *statsPanel, QVBoxLayout *statsLayout)
{
    QLabel *mainHeader = new QLabel("Анализ данных", statsPanel);
    mainHeader->setStyleSheet("font-size: 32px; font-weight: 600; color: #ddd;");
    statsLayout->addWidget(mainHeader);
}

QWidget* MainWindow::createBasicDataSection(QWidget *parent, QLabel **elementCountLabel,
                                             QLabel **sumLabel, QLabel **averageLabel)
{
    QWidget *section = Draw::createStatSection(parent, "Основные метрики");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(section->layout());

    *elementCountLabel = Draw::createAndRegisterStatRow(section, layout, "Количество элементов", "0", "elementCountLabel");
    *sumLabel = Draw::createAndRegisterStatRow(section, layout, "Сумма", "0", "sumLabel");
    *averageLabel = Draw::createAndRegisterStatRow(section, layout, "Среднее арифметическое", "—", "averageLabel");

    return section;
}

QWidget* MainWindow::createMeansSection(QWidget *parent)
{
    QWidget *section = Draw::createStatSection(parent, "Средние");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(section->layout());

    m_geometricMeanLabel = Draw::createAndRegisterStatRow(section, layout, "Геом. среднее", "—", "geometricMeanLabel");
    m_harmonicMeanLabel = Draw::createAndRegisterStatRow(section, layout, "Гарм. среднее", "—", "harmonicMeanLabel");
    m_weightedMeanLabel = Draw::createAndRegisterStatRow(section, layout, "Взвеш. среднее", "—", "weightedMeanLabel");
    m_rmsLabel = Draw::createAndRegisterStatRow(section, layout, "Квадр. среднее", "—", "rmsLabel");
    m_trimmedMeanLabel = Draw::createAndRegisterStatRow(section, layout, "Усеч. среднее", "—", "trimmedMeanLabel");

    return section;
}

QWidget* MainWindow::createDistributionSection(QWidget *parent)
{
    QWidget *section = Draw::createStatSection(parent, "Распределение");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(section->layout());

    m_medianLabel = Draw::createAndRegisterStatRow(section, layout, "Медиана", "—", "medianLabel");
    m_modeLabel = Draw::createAndRegisterStatRow(section, layout, "Мода", "—", "modeLabel");
    m_stdDevLabel = Draw::createAndRegisterStatRow(section, layout, "Стандартное отклонение", "—", "stdDevLabel");
    m_skewnessLabel = Draw::createAndRegisterStatRow(section, layout, "Асимметрия", "—", "skewnessLabel");
    m_kurtosisLabel = Draw::createAndRegisterStatRow(section, layout, "Эксцесс", "—", "kurtosisLabel");
    m_madLabel = Draw::createAndRegisterStatRow(section, layout, "Медианное абсолютное отклонение", "—", "madLabel");
    m_robustStdLabel = Draw::createAndRegisterStatRow(section, layout, "Робастный стандартный разброс", "—", "robustStdLabel");
    m_shapiroWilkLabel = Draw::createAndRegisterStatRow(section, layout, "Шапиро-Уилк", "—", "shapiroWilkLabel");
    m_densityLabel = Draw::createAndRegisterStatRow(section, layout, "Плотность", "—", "densityLabel");
    m_chiSquareLabel = Draw::createAndRegisterStatRow(section, layout, "χ²-критерий", "—", "chiSquareLabel");
    m_kolmogorovLabel = Draw::createAndRegisterStatRow(section, layout, "Колмогоров-Смирнов", "—", "kolmogorovLabel");

    return section;
}

QWidget* MainWindow::createExtremesSection(QWidget *parent)
{
    QWidget *section = Draw::createStatSection(parent, "Экстремумы");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(section->layout());

    m_minLabel = Draw::createAndRegisterStatRow(section, layout, "Минимум", "—", "minLabel");
    m_maxLabel = Draw::createAndRegisterStatRow(section, layout, "Максимум", "—", "maxLabel");
    m_rangeLabel = Draw::createAndRegisterStatRow(section, layout, "Размах", "—", "rangeLabel");

    return section;
}

QWidget* MainWindow::createCategoricalSection(QWidget *parent)
{
    QWidget *section = Draw::createStatSection(parent, "Категориальные данные");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(section->layout());

    m_modalFreqLabel = Draw::createAndRegisterStatRow(section, layout, "Модальная частота", "—", "modalFreqLabel");
    m_simpsonIndexLabel = Draw::createAndRegisterStatRow(section, layout, "Индекс Симпсона", "—", "simpsonIndexLabel");
    m_uniqueRatioLabel = Draw::createAndRegisterStatRow(section, layout, "Доля уникальных", "—", "uniqueRatioLabel");
    m_entropyLabel = Draw::createAndRegisterStatRow(section, layout, "Энтропия", "—", "entropyLabel");

    return section;
}

QWidget* MainWindow::createCorrelationSection(QWidget *parent)
{
    QWidget *section = Draw::createStatSection(parent, "Анализ взаимосвязей");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(section->layout());

    m_covarianceLabel = Draw::createAndRegisterStatRow(section, layout, "Ковариация", "—", "covarianceLabel");
    m_pearsonLabel = Draw::createAndRegisterStatRow(section, layout, "Пирсон", "—", "pearsonLabel");
    m_spearmanLabel = Draw::createAndRegisterStatRow(section, layout, "Спирмен", "—", "spearmanLabel");
    m_kendallLabel = Draw::createAndRegisterStatRow(section, layout, "Кендалл", "—", "kendallLabel");

    return section;
}

QWidget* MainWindow::setupDataPanel(QWidget *parent, QLabel **elementCountLabel,
                                     QLabel **sumLabel, QLabel **averageLabel)
{
    QWidget *statsPanel = new QWidget(parent);
    statsPanel->setLayout(new QVBoxLayout(statsPanel));
    Draw::setSizePolicyExpanding(statsPanel);

    QVBoxLayout *statsLayout = qobject_cast<QVBoxLayout *>(statsPanel->layout());
    statsLayout->setContentsMargins(12, 8, 12, 8);
    statsLayout->setSpacing(8);

    createDataHeader(statsPanel, statsLayout);
    statsLayout->addWidget(createBasicDataSection(statsPanel, elementCountLabel, sumLabel, averageLabel));
    statsLayout->addWidget(createMeansSection(statsPanel));
    statsLayout->addWidget(createDistributionSection(statsPanel));
    statsLayout->addWidget(createExtremesSection(statsPanel));
    statsLayout->addWidget(createCategoricalSection(statsPanel));
    statsLayout->addWidget(createCorrelationSection(statsPanel));

    statsLayout->addStretch();
    return statsPanel;
}

QScrollArea *setupDataSectionScrollArea(QWidget *parent, QWidget *toScroll) {
    QScrollArea *scrollArea = new QScrollArea(parent);
    scrollArea->setWidget(toScroll);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setFrameShape(QFrame::NoFrame); // Убираем рамку
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    return scrollArea;
}

QWidget *MainWindow::setupDataSection(QWidget *parent) {
    QWidget *dataSection = new QWidget(parent);
    QHBoxLayout *dataSectionLayout = new QHBoxLayout(dataSection);
    dataSectionLayout->setContentsMargins(0, 0, 0, 0);

    // Создаем панель статистики
    QWidget *statsPanel = setupDataPanel(dataSection, &m_elementCountLabel,
                                          &m_sumLabel, &m_averageLabel);

    // Создаем scroll area и настраиваем
    QScrollArea *scrollArea = setupDataSectionScrollArea(dataSection, statsPanel);

    // Добавляем таблицу и панель статистики
    QTableWidget *table = nullptr;
    auto *tablePanel = Draw::setupTablePanel(dataSection, &table);
    m_table = table;

    dataSectionLayout->addWidget(scrollArea, 1); // Заменяем statsPanel на scrollArea
    dataSectionLayout->addWidget(tablePanel, 1);
    QScroller::grabGesture(scrollArea, QScroller::LeftMouseButtonGesture);

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
            m_elementCountLabel && m_sumLabel && m_averageLabel && m_geometricMeanLabel &&
            m_medianLabel && m_modeLabel && m_stdDevLabel && m_minLabel && m_maxLabel &&
            m_rangeLabel && m_skewnessLabel && m_kurtosisLabel && m_harmonicMeanLabel && m_weightedMeanLabel &&
            m_rmsLabel && m_trimmedMeanLabel && m_robustStdLabel && m_madLabel && m_modalFreqLabel &&
            m_simpsonIndexLabel && m_uniqueRatioLabel && m_covarianceLabel && m_spearmanLabel && m_kendallLabel &&
            m_pearsonLabel && m_entropyLabel && m_shapiroWilkLabel && m_kolmogorovLabel &&
            m_chiSquareLabel && m_densityLabel);
}

MainWindow::StatisticalData MainWindow::collectStatisticalData() {
    StatisticalData result;

    // Сбор основных данных
    getTableValues(result.values, result.count, result.sum);
    result.weights = Calculate::findWeights(m_table);
    result.hasData = result.count > 0;

    // Сбор категориальных данных
    getCategorialData(result.categories);

    // Сбор данных для корреляций
    getCorrelationalData(result.xData, result.yData, 0, 1);

    return result;
}

void MainWindow::updateUI(const StatisticalData& data) {
    const bool hasData = data.hasData;
    const QString na = "—";
    const double mean = hasData ? data.sum / data.count : 0.0;

    // Основные метрики
    m_elementCountLabel->setText(hasData ? QString::number(data.count) : na);
    m_sumLabel->setText(hasData ? QString::number(data.sum, 'f', statsPrecision) : na);
    m_averageLabel->setText(hasData ? QString::number(mean, 'f', statsPrecision) : na);

    // Средние значения
    m_geometricMeanLabel->setText(hasData ? QString::number(Calculate::geometricMean(data.values), 'f', statsPrecision) : na);
    m_harmonicMeanLabel->setText(hasData ? QString::number(Calculate::harmonicMean(data.values), 'f', statsPrecision) : na);
    m_weightedMeanLabel->setText(areWeightsValid(data.weights, data.values) ? QString::number(Calculate::weightedMean(data.values, data.weights), 'f', statsPrecision) : na);
    m_rmsLabel->setText(hasData ? QString::number(Calculate::rootMeanSquare(data.values), 'f', statsPrecision) : na);
    m_trimmedMeanLabel->setText(hasData ? QString::number(Calculate::trimmedMean(data.values, trimmedMeanPercentage), 'f', statsPrecision) : na);

    // Распределение
    m_medianLabel->setText(hasData ? QString::number(Calculate::getMedian(data.values), 'f', statsPrecision) : na);
    m_modeLabel->setText(hasData ? QString::number(Calculate::getMode(data.values), 'f', statsPrecision) : na);
    m_stdDevLabel->setText(hasData ? QString::number(Calculate::getStandardDeviation(data.values, mean), 'f', statsPrecision) : na);
    m_skewnessLabel->setText(hasData ? QString::number(Calculate::skewness(data.values, mean, Calculate::getStandardDeviation(data.values, mean)), 'f', statsPrecision) : na);
    m_kurtosisLabel->setText(hasData ? QString::number(Calculate::kurtosis(data.values, mean, Calculate::getStandardDeviation(data.values, mean)), 'f', statsPrecision) : na);
    m_madLabel->setText(hasData ? QString::number(Calculate::medianAbsoluteDeviation(data.values), 'f', statsPrecision) : na);
    m_robustStdLabel->setText(hasData ? QString::number(Calculate::robustStandardDeviation(data.values), 'f', statsPrecision) : na);

    // Статистические тесты
    m_shapiroWilkLabel->setText(hasData ? QString::number(Calculate::shapiroWilkTest(data.values), 'f', statsPrecision) : na);
    m_densityLabel->setText(hasData ? QString::number(Calculate::calculateDensity(data.values, mean), 'f', statsPrecision) : na);
    m_chiSquareLabel->setText(hasData ? QString::number(Calculate::chiSquareTest(data.values), 'f', statsPrecision) : na);
    m_kolmogorovLabel->setText(hasData ? QString::number(Calculate::kolmogorovSmirnovTest(data.values), 'f', statsPrecision) : na);

    // Категориальные данные
    const bool hasCatData = !data.categories.isEmpty();
    m_modalFreqLabel->setText(hasCatData ? QString::number(Calculate::modalFrequency(data.categories), 'f', statsPrecision) : na);
    m_simpsonIndexLabel->setText(hasCatData ? QString::number(Calculate::simpsonDiversityIndex(data.categories), 'f', statsPrecision) : na);
    m_uniqueRatioLabel->setText(hasCatData ? QString::number(Calculate::uniqueValueRatio(data.categories), 'f', statsPrecision) : na);
    m_entropyLabel->setText(hasCatData ? QString::number(Calculate::entropy(data.categories), 'f', statsPrecision) : na);

    // Корреляции
    const bool hasPairs = data.xData.size() >= 2;
    const bool hasValidSpearman = data.xData.size() >= 3;
    m_covarianceLabel->setText(hasPairs ? QString::number(Calculate::covariance(data.xData, data.yData), 'f', statsPrecision) : na);
    m_pearsonLabel->setText(hasPairs ? QString::number(Calculate::pearsonCorrelation(data.xData, data.yData), 'f', statsPrecision) : na);
    m_spearmanLabel->setText(hasValidSpearman ? QString::number(Calculate::spearmanCorrelation(data.xData, data.yData), 'f', statsPrecision) : na);
    m_kendallLabel->setText(hasPairs ? QString::number(Calculate::kendallCorrelation(data.xData, data.yData), 'f', statsPrecision) : na);
}

void MainWindow::updateStatistics() {
    if (!areAllLabelsDefined()) {
        qWarning() << "Some labels are not initialized!";
        return;
    }

    const auto statisticalData = collectStatisticalData();
    updateUI(statisticalData);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    auto *header = Draw::setupHeader(mainWidget, 20);
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
