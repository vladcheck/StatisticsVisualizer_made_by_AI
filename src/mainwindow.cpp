#include "mainwindow.h"

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
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Горизонтальную отключаем
    scrollArea->setVerticalScrollBar(new QScrollBar(Qt::Vertical, scrollArea));
    scrollArea->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    return scrollArea;
}

QWidget* MainWindow::setupTablePanel(QWidget *parent) {
    QWidget *tableSection = new QWidget(parent);

    this->m_table = Draw::setupTable(tableSection); // Создаем таблицу и возвращаем через outTable
    auto *tableToolbar = setupTableToolbar(tableSection, m_table);

    QVBoxLayout *tableSectionLayout = new QVBoxLayout(tableSection);
    tableSectionLayout->addWidget(tableToolbar);
    tableSectionLayout->addWidget(m_table);

    return tableSection;
}

QWidget* MainWindow::setupDataSection(QWidget* parent) {
    QWidget* dataSection = new QWidget(parent);
    QHBoxLayout* dataSectionLayout = new QHBoxLayout(dataSection);
    dataSectionLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* statsPanel = setupDataPanel(dataSection, &m_elementCountLabel, &m_sumLabel, &m_averageLabel);
    QScrollArea* statsScrollArea = setupDataSectionScrollArea(dataSection, statsPanel);
    QWidget* tablePanel = setupTablePanel(dataSection);
    QSplitter* splitter = Draw::addSplitter(dataSection, statsScrollArea, tablePanel, 1, 2);

    dataSectionLayout->addWidget(splitter);

    // Обработчик прокрутки
    QObject::connect(statsScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged,
                     [statsScrollArea](int min, int max) {
                         statsScrollArea->setVerticalScrollBarPolicy(max > min ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
                     });

    return dataSection;
}

QWidget* MainWindow::setupTableToolbar(QWidget* parent, QTableWidget* table) {
    QWidget *toolbar = new QWidget(parent);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(5);

    // Спинбоксы
    auto *columnsContainer = Draw::createSpinBoxWithLabel(toolbar, "Столбцы", 512, initialColCount);
    QSpinBox *colSpinBox = qobject_cast<QSpinBox *>(columnsContainer->layout()->itemAt(1)->widget());
    auto *rowsContainer = Draw::createSpinBoxWithLabel(toolbar, "Ряды", 512, initialRowCount);
    QSpinBox *rowSpinBox = qobject_cast<QSpinBox *>(rowsContainer->layout()->itemAt(1)->widget());

    m_table = table;
    m_addColBtn = Draw::createToolButton("Добавить столбец", "add-column");
    m_delColBtn = Draw::createToolButton("Удалить столбец", "delete-column");
    m_clearBtn = Draw::createToolButton("Очистить", "clear");
    m_autoSizeBtn = Draw::createToolButton("Авторазмер", "auto-size");
    m_colSpin = colSpinBox;
    m_rowSpin = rowSpinBox;
    m_addRowBtn = Draw::createToolButton("Добавить ряд", "add-row");
    m_delRowBtn = Draw::createToolButton("Удалить ряд", "delete-row");
    m_importBtn = Draw::createToolButton("Импортировать данные", "import-file");
    m_exportBtn = Draw::createToolButton("Экспортировать данные", "export-file");

    setupTableActions();
    toolbarLayout->addLayout(rowsContainer);
    toolbarLayout->addLayout(columnsContainer);

    QList<QWidget*> toolbarWidgets = {
        m_addRowBtn,
        m_delRowBtn,
        m_addColBtn,
        m_delColBtn,
        m_autoSizeBtn,
        m_clearBtn,
        m_importBtn,
        m_exportBtn
    };

    // Добавляем элементы в layout
    for (QWidget *widget : toolbarWidgets) {
        toolbarLayout->addWidget(widget);
    }

    toolbarLayout->addStretch();

    return toolbar;
}

void MainWindow::setupTableActions()
{
    // Добавление столбца
    Draw::connect(m_addColBtn, [=]()
                  {
                      m_table->setColumnCount(m_table->columnCount() + 1);
                      m_colSpin->setValue(m_table->columnCount()); });

    // Удаление столбца
    Draw::connect(m_delColBtn, [=]()
                  {
                      if(m_table->columnCount() > 1) {
                          m_table->setColumnCount(m_table->columnCount() - 1);
                          m_colSpin->setValue(m_table->columnCount());
                      } });

    // Очистка таблицы
    Draw::connect(m_clearBtn, [=]()
                  {
                      auto reply = QMessageBox::question(
                          m_table,
                          "Очистка таблицы",
                          "Удалить все данные?",
                          QMessageBox::Yes | QMessageBox::No
                          );

                      if (reply == QMessageBox::Yes) {
                          m_table->clearContents();
                          m_colSpin->setValue(m_colSpin->minimum());
                      } });

    // Авторазмер
    Draw::connect(m_autoSizeBtn, [=]()
                  {
                      m_table->resizeColumnsToContents();
                      m_table->resizeRowsToContents(); });

    // Обработка изменения спинбокса столбцов
    Draw::connect(m_colSpin, [=](int value)
                  {
                      if (value >= m_colSpin->minimum()) {
                          m_table->setColumnCount(value);
                      } });

    // Импорт файлов
    QObject::connect(m_importBtn, &QPushButton::clicked, [=]() {
        Import::importFile(m_table);
    });

    // Добавление ряда
    Draw::connect(m_addRowBtn, [=]() {
        m_table->setRowCount(m_table->rowCount() + 1);
        m_rowSpin->setValue(m_table->rowCount());
    });

    // Удаление ряда
    Draw::connect(m_delRowBtn, [=]() {
        if(m_table->rowCount() > 1) {
            m_table->setRowCount(m_table->rowCount() - 1);
            m_rowSpin->setValue(m_table->rowCount());
        }
    });

    // Обработка изменения спиннера рядов
    Draw::connect(m_rowSpin, [=](int value) {
        if (value >= m_rowSpin->minimum()) {
            m_table->setRowCount(value);
        }
    });
}

bool MainWindow::areAllLabelsDefined() {
    return (m_table &&
            m_elementCountLabel && m_sumLabel && m_averageLabel && m_geometricMeanLabel &&
            m_medianLabel && m_modeLabel && m_stdDevLabel && m_minLabel && m_maxLabel &&
            m_rangeLabel && m_skewnessLabel && m_kurtosisLabel && m_harmonicMeanLabel &&
            m_rmsLabel && m_trimmedMeanLabel && m_robustStdLabel && m_madLabel &&
            m_shapiroWilkLabel && m_kolmogorovLabel && m_chiSquareLabel && m_densityLabel);
}

TableData MainWindow::parse() const {
    TableData plotData;
    for (int row = 0; row < m_table->rowCount(); ++row) {
        std::vector<std::pair<int, int>> rowData; // Вектор для текущей строки

        for (int col = 0; col < m_table->columnCount(); ++col) {
            if (auto item = m_table->item(row, col)) {
                bool ok;
                double value = item->text().toDouble(&ok);
                if (ok) {
                    rowData.emplace_back(col, value); // Добавляем пару (столбец, значение)
                }
            }
        }

        if (!rowData.empty()) {
            plotData.push_back(rowData); // Добавляем строку в результат
        }
    }
    return plotData;
}

void MainWindow::setupChartAxes() {
    // Создаем оси один раз
    m_axisX = Draw::setupAxis("Ось X", 0, 10);
    m_axisY = Draw::setupAxis("Ось Y", 0, 10);

    m_chartView->chart()->addAxis(m_axisX, Qt::AlignBottom);
    m_chartView->chart()->addAxis(m_axisY, Qt::AlignLeft);
}

void MainWindow::initializeChart() {
    QWidget* graphSection = findChild<QWidget*>("graphSection");

    if (graphSection) {
        m_chartView = graphSection->findChild<QChartView*>();

        if (m_chartView) {
            setupChartAxes();
            m_chartView->setRenderHint(QPainter::Antialiasing);
            m_chartView->chart()->setBackgroundBrush(Qt::white);
        }
    }
}

void MainWindow::updateAxesRange(const TableData& data) {
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();

    // Ищем минимальные и максимальные значения
    for (const auto& series : data) {
        for (const auto& [x, y] : series) {
            minX = std::min(minX, static_cast<double>(x));
            maxX = std::max(maxX, static_cast<double>(x));
            minY = std::min(minY, static_cast<double>(y));
            maxY = std::max(maxY, static_cast<double>(y));
        }
    }

    // Устанавливаем новые границы с небольшим запасом
    const double padding = 0.1;
    m_axisX->setRange(minX - padding, maxX + padding);
    m_axisY->setRange(minY - padding, maxY + padding);
}

void MainWindow::plotData(const std::vector<std::vector<std::pair<int, int>>>& data) {
    if (!m_chartView || !m_axisX || !m_axisY) return;

    clearChart();

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < data.size(); ++i) {
        auto [scatter, line] = createSeries(i);

        addPointsToSeries(scatter, data[i], minX, maxX, minY, maxY);
        addPointsToSeries(line, data[i], minX, maxX, minY, maxY);

        m_chartView->chart()->addSeries(scatter);
        m_chartView->chart()->addSeries(line);

        attachSeriesToAxes(scatter);
        attachSeriesToAxes(line);
    }

    updateAxisRanges(minX, maxX, minY, maxY);
    m_chartView->chart()->update();
}

void MainWindow::clearChart() {
    if (m_chartView) {
        m_chartView->chart()->removeAllSeries();
    }
}

QPair<QScatterSeries*, QLineSeries*> MainWindow::createSeries(int seriesIndex) {
    QScatterSeries* scatter = new QScatterSeries();
    QLineSeries* line = new QLineSeries();

    // Настройка стилей
    QColor color = m_seriesColors[seriesIndex % m_seriesColors.size()];

    scatter->setName(QString("Ряд %1").arg(seriesIndex + 1));
    scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatter->setMarkerSize(12.0);
    scatter->setColor(color);
    scatter->setBorderColor(Qt::white);

    line->setPen(QPen(color, 2));
    line->setName(QString("Линия %1").arg(seriesIndex + 1));

    return qMakePair(scatter, line);
}

void MainWindow::addPointsToSeries(QXYSeries* series, const std::vector<std::pair<int, int>>& data,
                                   double& minX, double& maxX, double& minY, double& maxY) {
    for (const auto& [x, y] : data) {
        QPointF point(x, y);
        *series << point;

        // Обновляем границы
        minX = qMin(minX, point.x());
        maxX = qMax(maxX, point.x());
        minY = qMin(minY, point.y());
        maxY = qMax(maxY, point.y());
    }
}

void MainWindow::attachSeriesToAxes(QXYSeries* series) {
    series->attachAxis(m_axisX);
    series->attachAxis(m_axisY);
}

void MainWindow::updateAxisRanges(double minX, double maxX, double minY, double maxY) {
    if (minX == std::numeric_limits<double>::max()) { // Нет данных
        m_axisX->setRange(0, 10);
        m_axisY->setRange(0, 10);
        return;
    }

    const double xPadding = (maxX - minX) * 0.1;
    const double yPadding = (maxY - minY) * 0.1;

    m_axisX->setRange(minX - xPadding, maxX + xPadding);
    m_axisY->setRange(minY - yPadding, maxY + yPadding);
}

void MainWindow::updateUI(const TableData data) {
    const bool hasData = !data.empty() && !data[0].empty();
    std::vector<double> values;
    if (hasData) {
        for (const auto& pair : data[0]) {
            values.push_back(pair.second);
        }
    }

    double mean = hasData ? Calculate::getMean(values) : 0.0;
    double stdDev = hasData ? Calculate::getStandardDeviation(values, mean) : 0.0;
    double min = hasData ? *std::min_element(values.begin(), values.end()) : 0.0;
    double max = hasData ? *std::max_element(values.begin(), values.end()) : 0.0;
    double range = max - min;

    // Основные метрики
    m_elementCountLabel->setText(hasData ? QString::number(values.size()) : na);
    m_sumLabel->setText(hasData ? QString::number(Calculate::getSum(values), 'f', statsPrecision) : na);
    m_averageLabel->setText(hasData ? QString::number(hasData ? mean : 0.0, 'f', statsPrecision) : na);

    // Средние значения
    m_geometricMeanLabel->setText(hasData ? QString::number(Calculate::geometricMean(values), 'f', statsPrecision) : na);
    m_harmonicMeanLabel->setText(hasData ? QString::number(Calculate::harmonicMean(values), 'f', statsPrecision) : na);
    m_rmsLabel->setText(hasData ? QString::number(Calculate::rootMeanSquare(values), 'f', statsPrecision) : na);
    m_trimmedMeanLabel->setText(hasData ? QString::number(Calculate::trimmedMean(values, trimmedMeanPercentage), 'f', statsPrecision) : na);

    // Распределение
    m_medianLabel->setText(hasData ? QString::number(Calculate::getMedian(values), 'f', statsPrecision) : na);
    m_modeLabel->setText(hasData ? QString::number(Calculate::getMode(values), 'f', statsPrecision) : na);
    m_stdDevLabel->setText(hasData ? QString::number(stdDev, 'f', statsPrecision) : na);
    m_skewnessLabel->setText(hasData ? QString::number(Calculate::skewness(values, mean, stdDev), 'f', statsPrecision) : na);
    m_kurtosisLabel->setText(hasData ? QString::number(Calculate::kurtosis(values, mean, stdDev), 'f', statsPrecision) : na);
    m_madLabel->setText(hasData ? QString::number(Calculate::medianAbsoluteDeviation(values), 'f', statsPrecision) : na);
    m_robustStdLabel->setText(hasData ? QString::number(Calculate::robustStandardDeviation(values), 'f', statsPrecision) : na);

    // Статистические тесты
    m_shapiroWilkLabel->setText(hasData ? QString::number(Calculate::shapiroWilkTest(values), 'f', statsPrecision) : na);
    m_densityLabel->setText(hasData ? QString::number(Calculate::calculateDensity(values, mean), 'f', statsPrecision) : na);
    m_chiSquareLabel->setText(hasData ? QString::number(Calculate::chiSquareTest(values), 'f', statsPrecision) : na);
    m_kolmogorovLabel->setText(hasData ? QString::number(Calculate::kolmogorovSmirnovTest(values), 'f', statsPrecision) : na);

    // Экстремумы
    m_minLabel->setText(hasData ? QString::number(min, 'f', statsPrecision) : na);
    m_maxLabel->setText(hasData ? QString::number(max, 'f', statsPrecision) : na);
    m_rangeLabel->setText(hasData ? QString::number(range, 'f', statsPrecision) : na);
}

void MainWindow::updateStatistics() {
    if (!areAllLabelsDefined()) return;

    const TableData data = parse();
    updateUI(data);
    plotData(data);
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QWidget *dataSection = setupDataSection(mainWidget);
    QWidget* graphSection = Draw::setupGraphSection(mainWidget);
    initializeChart(); // Инициализация графика один раз

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(dataSection, 1);
    mainLayout->addWidget(graphSection, 1);

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
