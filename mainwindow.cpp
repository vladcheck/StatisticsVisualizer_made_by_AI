#include "mainwindow.h"
#include "globals.h"
#include "draw.h"
#include "calculate.h"
#include "tableactions.h"

void MainWindow::createStatsHeader(QWidget *statsPanel, QVBoxLayout *statsLayout)
{
    QLabel *mainHeader = new QLabel("Анализ данных", statsPanel);
    mainHeader->setStyleSheet("font-size: 32px; font-weight: 600; color: #ddd;");
    statsLayout->addWidget(mainHeader);
}

QWidget* MainWindow::createBasicStatsSection(QWidget *parent, QLabel **elementCountLabel,
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

QWidget* MainWindow::setupStatsPanel(QWidget *parent, QLabel **elementCountLabel,
                                     QLabel **sumLabel, QLabel **averageLabel)
{
    QWidget *statsPanel = new QWidget(parent);
    statsPanel->setLayout(new QVBoxLayout(statsPanel));
    Draw::setSizePolicyExpanding(statsPanel);

    QVBoxLayout *statsLayout = qobject_cast<QVBoxLayout *>(statsPanel->layout());
    statsLayout->setContentsMargins(12, 8, 12, 8);
    statsLayout->setSpacing(8);

    createStatsHeader(statsPanel, statsLayout);
    statsLayout->addWidget(createBasicStatsSection(statsPanel, elementCountLabel, sumLabel, averageLabel));
    statsLayout->addWidget(createMeansSection(statsPanel));
    statsLayout->addWidget(createDistributionSection(statsPanel));
    statsLayout->addWidget(createExtremesSection(statsPanel));
    statsLayout->addWidget(createCategoricalSection(statsPanel));
    statsLayout->addWidget(createCorrelationSection(statsPanel));

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
    auto *tablePanel = Draw::setupTablePanel(dataSection, &table);
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
            m_elementCountLabel && m_sumLabel && m_averageLabel && m_geometricMeanLabel &&
            m_medianLabel && m_modeLabel && m_stdDevLabel && m_minLabel && m_maxLabel &&
            m_rangeLabel && m_skewnessLabel && m_kurtosisLabel && m_harmonicMeanLabel && m_weightedMeanLabel &&
            m_rmsLabel && m_trimmedMeanLabel && m_robustStdLabel && m_madLabel && m_modalFreqLabel &&
            m_simpsonIndexLabel && m_uniqueRatioLabel && m_covarianceLabel && m_spearmanLabel && m_kendallLabel &&
            m_pearsonLabel && m_entropyLabel);
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
    const double entropyValue = Calculate::entropy(categories);

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
    m_entropyLabel->setText(hasCatData(categories) ? QString::number(entropyValue, 'f', statsPrecision) : na);

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
