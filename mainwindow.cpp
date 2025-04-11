#include "mainwindow.h"
#include "globals.h"
#include "draw.h"
#include "calculate.h"
#include "structs.h"

bool areWeightsValid(const QVector<double>& weights, const QVector<double>& values) {
    return (weights.size() == values.size()) && !weights.isEmpty();
}

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

bool MainWindow::areAllLabelsDefined() {
    return (m_table &&
            m_elementCountLabel && m_sumLabel && m_averageLabel && m_geometricMeanLabel &&
            m_medianLabel && m_modeLabel && m_stdDevLabel && m_minLabel && m_maxLabel &&
            m_rangeLabel && m_skewnessLabel && m_kurtosisLabel && m_harmonicMeanLabel &&
            m_rmsLabel && m_trimmedMeanLabel && m_robustStdLabel && m_madLabel &&
            m_shapiroWilkLabel && m_kolmogorovLabel && m_chiSquareLabel && m_densityLabel);
}

TableRow MainWindow::parse() const {
    TableRow result;
    result.isValid = false;

    for (int col = 0; col < m_table->columnCount(); ++col) {
        QTableWidgetItem* item = m_table->item(0, col);
        if (!item || item->text().isEmpty()) continue;

        bool ok;
        double value = item->text().toDouble(&ok);
        if (ok) result.values.append(value);
    }

    result.isValid = !result.values.isEmpty();
    return result;
}

void MainWindow::updateUI(const TableRow& rowData) {
    const bool hasData = rowData.isValid;
    double mean = hasData ? Calculate::getMean(rowData.values) : 0.0;
    double stdDev = hasData ? Calculate::getStandardDeviation(rowData.values, mean) : 0.0;
    double min = hasData ? *std::min_element(rowData.values.begin(), rowData.values.end()) : 0.0;
    double max = hasData ? *std::max_element(rowData.values.begin(), rowData.values.end()) : 0.0;
    double range = max - min;

    // Основные метрики
    m_elementCountLabel->setText(hasData ? QString::number(rowData.values.size()) : na);
    m_sumLabel->setText(hasData ? QString::number(Calculate::getSum(rowData.values), 'f', statsPrecision) : na);
    m_averageLabel->setText(hasData ? QString::number(hasData ? mean : 0.0, 'f', statsPrecision) : na);

    // Средние значения
    m_geometricMeanLabel->setText(hasData ? QString::number(Calculate::geometricMean(rowData.values), 'f', statsPrecision) : na);
    m_harmonicMeanLabel->setText(hasData ? QString::number(Calculate::harmonicMean(rowData.values), 'f', statsPrecision) : na);
    m_rmsLabel->setText(hasData ? QString::number(Calculate::rootMeanSquare(rowData.values), 'f', statsPrecision) : na);
    m_trimmedMeanLabel->setText(hasData ? QString::number(Calculate::trimmedMean(rowData.values, trimmedMeanPercentage), 'f', statsPrecision) : na);

    // Распределение
    m_medianLabel->setText(hasData ? QString::number(Calculate::getMedian(rowData.values), 'f', statsPrecision) : na);
    m_modeLabel->setText(hasData ? QString::number(Calculate::getMode(rowData.values), 'f', statsPrecision) : na);
    m_stdDevLabel->setText(hasData ? QString::number(stdDev, 'f', statsPrecision) : na);
    m_skewnessLabel->setText(hasData ? QString::number(Calculate::skewness(rowData.values, mean, stdDev), 'f', statsPrecision) : na);
    m_kurtosisLabel->setText(hasData ? QString::number(Calculate::kurtosis(rowData.values, mean, stdDev), 'f', statsPrecision) : na);
    m_madLabel->setText(hasData ? QString::number(Calculate::medianAbsoluteDeviation(rowData.values), 'f', statsPrecision) : na);
    m_robustStdLabel->setText(hasData ? QString::number(Calculate::robustStandardDeviation(rowData.values), 'f', statsPrecision) : na);

    // Статистические тесты
    m_shapiroWilkLabel->setText(hasData ? QString::number(Calculate::shapiroWilkTest(rowData.values), 'f', statsPrecision) : na);
    m_densityLabel->setText(hasData ? QString::number(Calculate::calculateDensity(rowData.values, mean), 'f', statsPrecision) : na);
    m_chiSquareLabel->setText(hasData ? QString::number(Calculate::chiSquareTest(rowData.values), 'f', statsPrecision) : na);
    m_kolmogorovLabel->setText(hasData ? QString::number(Calculate::kolmogorovSmirnovTest(rowData.values), 'f', statsPrecision) : na);

    // Экстремумы
    m_minLabel->setText(hasData ? QString::number(min, 'f', statsPrecision) : na);
    m_maxLabel->setText(hasData ? QString::number(max, 'f', statsPrecision) : na);
    m_rangeLabel->setText(hasData ? QString::number(range, 'f', statsPrecision) : na);
}

void MainWindow::updateStatistics() {
    if (!areAllLabelsDefined()) return;

    const TableRow rowData = parse();
    updateUI(rowData);
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
        // Обработчик изменения текущей ячейки
        connect(m_table, &QTableWidget::currentCellChanged, [this](int row, int, int, int) {
            updateStatistics();
        });

        // Обработчик изменения данных
        connect(m_table, &QTableWidget::itemChanged, this, &MainWindow::updateStatistics);
        updateStatistics();
    } else {
        qFatal("Table initialization failed!");
    }

    this->setWindowState(Qt::WindowMaximized);
    this->setWindowIcon(QIcon(":/logo.png"));
}

MainWindow::~MainWindow() {}
