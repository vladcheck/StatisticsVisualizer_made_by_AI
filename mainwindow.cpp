#include "mainwindow.h"

// Получение цвета по индексу с цикличностью
QColor MainWindow::getSeriesColor(int index) const {
    return m_seriesColors[index % m_seriesColors.size()];
}

QPen MainWindow::getSeriesPen(int index) const {
    QPen pen(getSeriesColor(index));
    pen.setWidthF(2.5); // Толщина линии
    pen.setStyle(Qt::SolidLine); // Стиль линии
    return pen;
}

// Автоматическое затемнение для границ
QColor MainWindow::getBorderColor(int index) const {
    QColor base = getSeriesColor(index);
    return base.darker(120); // Затемнение на 20%
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

    QWidget* statsPanel = setupDataPanel(dataSection);
    QScrollArea* statsScrollArea = Draw::setupDataSectionScrollArea(dataSection, statsPanel);
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

QWidget* MainWindow::setupDataPanel(QWidget *parent)
{
    QWidget *statsPanel = new QWidget(parent);
    statsPanel->setLayout(new QVBoxLayout(statsPanel));
    Draw::setSizePolicyExpanding(statsPanel);

    QVBoxLayout *statsLayout = qobject_cast<QVBoxLayout *>(statsPanel->layout());
    statsLayout->setContentsMargins(12, 8, 12, 8);
    statsLayout->setSpacing(8);

    Draw::createDataHeader(statsPanel, statsLayout);
    statsLayout->addWidget(Draw::createBasicDataSection(statsPanel, &m_elementCountLabel, &m_sumLabel, &m_averageLabel));
    statsLayout->addWidget(Draw::createMeansSection(statsPanel, &m_geometricMeanLabel, &m_harmonicMeanLabel,
                                                    &m_rmsLabel, &m_trimmedMeanLabel));
    statsLayout->addWidget(Draw::createDistributionSection(statsPanel, &m_medianLabel, &m_modeLabel, &m_stdDevLabel,
                                                           &m_skewnessLabel, &m_kurtosisLabel, &m_madLabel, &m_robustStdLabel,
                                                           &m_shapiroWilkLabel, &m_densityLabel, &m_chiSquareLabel, &m_kolmogorovLabel));
    statsLayout->addWidget(Draw::createExtremesSection(statsPanel, &m_minLabel, &m_maxLabel, &m_rangeLabel));

    statsLayout->addStretch();
    return statsPanel;
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

    QList<QWidget*> toolbarWidgets = {m_addRowBtn, m_delRowBtn, m_addColBtn, m_delColBtn,
                                       m_autoSizeBtn, m_clearBtn, m_importBtn, m_exportBtn};

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

void MainWindow::setupGraphSettingsSlots() {
    connect(m_table->model(), &QAbstractItemModel::rowsInserted,
            this, &MainWindow::handleSeriesAdded);

    connect(m_table->model(), &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &MainWindow::handleSeriesRemoved);

    connect(m_xAxisTitleEdit, &QLineEdit::textChanged,
            this, &MainWindow::updateXAxisTitle);

    connect(m_yAxisTitleEdit, &QLineEdit::textChanged,
            this, &MainWindow::updateYAxisTitle);
}

void MainWindow::updateMarker(int seriesIndex, bool isMax) {
    // Проверяем валидность индекса
    if (seriesIndex < 0 || seriesIndex >= m_table->rowCount()) return;

    // Проверяем существование контейнера маркеров
    if (!m_seriesMarkers.contains(seriesIndex)) {
        m_seriesMarkers.insert(seriesIndex, SeriesMarkers{});
    }

    auto& markers = m_seriesMarkers[seriesIndex];
    QScatterSeries** marker = isMax ? &markers.maxMarker : &markers.minMarker;

    // Удаляем только если маркер существует и добавлен на график
    if (*marker && m_chartView->chart()->series().contains(*marker)) {
        m_chartView->chart()->removeSeries(*marker);
        delete *marker;
        *marker = nullptr;
    }

    // Пересчитываем экстремум только если кнопка активна
    if ((isMax && m_maxButtons[seriesIndex]->isChecked()) ||
        (!isMax && m_minButtons[seriesIndex]->isChecked())) {

        auto [val, col] = findExtremum(seriesIndex, isMax);
        if (col != -1) {
            *marker = Draw::createMarker(col, val, m_chartView->chart(), m_axisX, m_axisY, isMax);
            // Дополнительная проверка перед добавлением
            if (*marker && !m_chartView->chart()->series().contains(*marker)) {
                m_chartView->chart()->addSeries(*marker);
                (*marker)->attachAxis(m_axisX);
                (*marker)->attachAxis(m_axisY);
            }
        }
    }
}

void MainWindow::handleExtremumToggle(int seriesIndex, bool isMax, bool checked) {
    auto& markers = m_seriesMarkers[seriesIndex];
    QScatterSeries** targetMarker = isMax ? &markers.maxMarker : &markers.minMarker;

    // Удаляем предыдущий маркер
    if (*targetMarker) {
        m_chartView->chart()->removeSeries(*targetMarker);
        delete *targetMarker;
        *targetMarker = nullptr;
    }

    if (checked) {
        auto [value, col] = findExtremum(seriesIndex, isMax);
        if (col != -1) {
            *targetMarker = Draw::createMarker(col, value,
                                               m_chartView->chart(), m_axisX, m_axisY, isMax);

            // Подключаем обновление при изменении данных
            connect(m_table->model(), &QAbstractItemModel::dataChanged,
                    [this, seriesIndex, isMax]() {
                        updateExtremumMarker(seriesIndex, isMax);
                    });
        }
    }
}

void MainWindow::updateExtremumMarker(int seriesIndex, bool isMax) {
    auto& markers = m_seriesMarkers[seriesIndex];
    QScatterSeries** targetMarker = isMax ? &markers.maxMarker : &markers.minMarker;
    QPushButton* btn = isMax ? m_maxButtons[seriesIndex] : m_minButtons[seriesIndex];

    if (btn->isChecked()) {
        handleExtremumToggle(seriesIndex, isMax, true);
    }
}

void MainWindow::handleSeriesAdded(const QModelIndex &parent, int first, int last) {
    if (QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(m_seriesSettingsContent->layout())) {
        for(int row = first; row <= last; ++row) {
            if(row >= m_seriesNameEdits.size()) {
                QLineEdit* edit;
                QPushButton *minBtn, *maxBtn;
                QWidget* rowWidget = Draw::createSeriesNameRow(m_seriesSettingsContent, row, &edit, &minBtn, &maxBtn);

                // Настройка кнопок
                minBtn->setCheckable(true);
                maxBtn->setCheckable(true);
                updateButtonsState(row);

                connect(minBtn, &QPushButton::toggled, [=](bool checked) {
                    if (!minBtn->isEnabled()) return;
                    handleExtremumToggle(row, false, checked);
                });

                connect(maxBtn, &QPushButton::toggled, [=](bool checked) {
                    if (!maxBtn->isEnabled()) return;
                    handleExtremumToggle(row, true, checked);
                });

                // Сохраняем ссылки
                m_seriesNameEdits.append(edit);
                m_minButtons.append(minBtn);
                m_maxButtons.append(maxBtn);
                m_seriesMarkers.insert(row, SeriesMarkers{});

                layout->insertWidget(row, rowWidget);
                connect(edit, &QLineEdit::textChanged, this, &MainWindow::updateSeriesNames);
            }
        }
    }
}

void MainWindow::handleSeriesRemoved(const QModelIndex &parent, int first, int last) {
    if (QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(m_seriesSettingsContent->layout())) {
        for(int i = last; i >= first; --i) {
            // Удаляем маркеры перед удалением ряда
            if (m_seriesMarkers.contains(i)) {
                auto& markers = m_seriesMarkers[i];
                if (markers.minMarker) {
                    m_chartView->chart()->removeSeries(markers.minMarker);
                    delete markers.minMarker;
                }
                if (markers.maxMarker) {
                    m_chartView->chart()->removeSeries(markers.maxMarker);
                    delete markers.maxMarker;
                }
                m_seriesMarkers.remove(i);
            }

            // Обновляем индексы для оставшихся элементов
            for (int j = i; j < m_seriesMarkers.size(); ++j) {
                if (m_seriesMarkers.contains(j + 1)) {
                    m_seriesMarkers[j] = m_seriesMarkers.take(j + 1);
                }
            }
        }
    }
}

// Общая функция для поиска экстремумов
std::pair<double, int> MainWindow::findExtremum(int seriesIndex, bool findMax) {
    double extremumVal = findMax ? std::numeric_limits<double>::lowest()
                                : std::numeric_limits<double>::max();
    int extremumCol = -1;

    if(seriesIndex < 0 || seriesIndex >= m_table->rowCount())
        return {extremumVal, extremumCol};

    for(int col = 0; col < m_table->columnCount(); ++col) {
        if(auto item = m_table->item(seriesIndex, col)) {
            bool ok;
            double val = item->text().toDouble(&ok);
            if(ok) {
                if((findMax && val > extremumVal) || (!findMax && val < extremumVal)) {
                    extremumVal = val;
                    extremumCol = col;
                }
            }
        }
    }
    return {extremumVal, extremumCol};
}

void MainWindow::updateButtonsState(int seriesIndex) {
    if(seriesIndex < 0 || seriesIndex >= m_minButtons.size())
        return;

    bool isEmpty = isSeriesEmpty(seriesIndex);

    // Обновляем состояния кнопок
    QPushButton* minBtn = m_minButtons[seriesIndex];
    QPushButton* maxBtn = m_maxButtons[seriesIndex];

    // Блокировка кнопок
    minBtn->setDisabled(isEmpty);
    maxBtn->setDisabled(isEmpty);

    // Принудительное обновление стилей
    minBtn->style()->unpolish(minBtn);
    minBtn->style()->polish(minBtn);

    maxBtn->style()->unpolish(maxBtn);
    maxBtn->style()->polish(maxBtn);
}

bool MainWindow::isSeriesEmpty(int seriesIndex) const {
    if(seriesIndex < 0 || seriesIndex >= m_table->rowCount())
        return true;

    for(int col = 0; col < m_table->columnCount(); ++col) {
        if(auto item = m_table->item(seriesIndex, col)) {
            if(!item->text().isEmpty()) return false;
        }
    }
    return true;
}

void MainWindow::refreshLegend() {
    if (!m_chartView || !m_chartView->chart()) return;

    QLegend* legend = m_chartView->chart()->legend();
    for (QLegendMarker* marker : legend->markers()) {
        if (auto series = dynamic_cast<QScatterSeries*>(marker->series())) {
            if (series->property("isHiddenMarker").toBool()) {
                marker->setVisible(false);
            }
        }
    }
}

// Слоты для обработки кнопок
void MainWindow::handleShowMin(int seriesIndex) {
    auto [minVal, minCol] = findExtremum(seriesIndex, false);
    if(minCol != -1) {
        Draw::createMarker(minCol, minVal,  m_chartView->chart(), m_axisX, m_axisY, false); // RED
    }
}

void MainWindow::handleShowMax(int seriesIndex) {
    auto [maxVal, maxCol] = findExtremum(seriesIndex, true);
    if(maxCol != -1) {
        Draw::createMarker(maxCol, maxVal,  m_chartView->chart(), m_axisX, m_axisY, true); // GREEN
    }
}

void MainWindow::updateSeriesNames() {
    for(int i = 0; i < m_seriesNameEdits.size(); ++i) {
        if(QAbstractSeries* series = m_chartView->chart()->series().value(i)) {
            series->setName(m_seriesNameEdits[i]->text());
        }
    }
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

void MainWindow::plotData(const TableData& data) {
    if (!m_chartView || !m_axisX || !m_axisY) return;

    clearChart();

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < data.size(); ++i) {
        // Создаем серию один раз
        QLineSeries* series = createSeries(i,false);

        // Устанавливаем имя из поля ввода или по умолчанию
        QString seriesName = "Наименование ";
        if (i < static_cast<size_t>(m_seriesNameEdits.size())) {
            seriesName = m_seriesNameEdits[i]->text().isEmpty()
                       ? seriesName
                       : m_seriesNameEdits[i]->text();
        }
        series->setName(seriesName);

        // Добавляем точки и обновляем границы
        addPointsToSeries(series, data[i], minX, maxX, minY, maxY);

        // Добавляем серию на график
        m_chartView->chart()->addSeries(series);
        attachSeriesToAxes(series);
    }

    updateAxisRanges(minX, maxX, minY, maxY);
    m_chartView->chart()->update();
}
void MainWindow::clearChart() {
    if (m_chartView) {
        m_chartView->chart()->removeAllSeries();
    }
}

void MainWindow::addPointsToSeriesGraph(int seriesIndex, QLineSeries* series) {
    QScatterSeries* scatter = new QScatterSeries();
    scatter->setMarkerSize(8);
    scatter->setColor(getSeriesColor(seriesIndex).lighter(120));
    scatter->setBorderColor(getSeriesColor(seriesIndex).darker(150));

    // Привязываем маркеры к линии
    series->setPointsVisible(true);
    m_chartView->chart()->addSeries(scatter);
    scatter->attachAxis(m_axisX);
    scatter->attachAxis(m_axisY);
}

QLineSeries* MainWindow::createSeries(int seriesIndex, bool showPoints) {
    QLineSeries* series = new QLineSeries();

    // Устанавливаем стиль линии
    QPen pen = getSeriesPen(seriesIndex);
    series->setPen(pen);

    if (showPoints) {
        addPointsToSeriesGraph(seriesIndex, series);
    }

    return series;
}

void MainWindow::addPointsToSeries(QLineSeries* series,
                                   const std::vector<std::pair<int, int>>& data,
                                   double& minX, double& maxX,
                                   double& minY, double& maxY) {
    for (const auto& [x, y] : data) {
        QPointF point(x, y);
        series->append(point.x(), point.y());

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

QString formatValue(double value, int precision = 2) {
    return QString::number(value, 'f', precision);
}

template<typename Func, typename... Args>
QString MainWindow::calculateAndFormat(bool hasData, Func func, Args&&... args) const {
    return hasData ? formatValue(func(std::forward<Args>(args)...)) : na;
}

void MainWindow::updateBasicMetrics(bool hasData, const std::vector<double>& values, double mean) {
    m_elementCountLabel->setText(hasData ? QString::number(values.size()) : na);
    m_sumLabel->setText(calculateAndFormat(hasData, Calculate::getSum, values));
    m_averageLabel->setText(calculateAndFormat(hasData, [mean](){ return mean; }));
}

void MainWindow::updateAverages(bool hasData, const std::vector<double>& values) {
    m_geometricMeanLabel->setText(calculateAndFormat(hasData, Calculate::geometricMean, values));
    m_harmonicMeanLabel->setText(calculateAndFormat(hasData, Calculate::harmonicMean, values));
    m_rmsLabel->setText(calculateAndFormat(hasData, Calculate::rootMeanSquare, values));
    m_trimmedMeanLabel->setText(calculateAndFormat(hasData,
                                                   Calculate::trimmedMean, values, trimmedMeanPercentage));
}

void MainWindow::updateDistribution(bool hasData, const std::vector<double>& values, double mean, double stdDev) {
    m_medianLabel->setText(calculateAndFormat(hasData, Calculate::getMedian, values));
    m_modeLabel->setText(calculateAndFormat(hasData, Calculate::getMode, values));
    m_stdDevLabel->setText(calculateAndFormat(hasData, [stdDev](){ return stdDev; }));
    m_skewnessLabel->setText(calculateAndFormat(hasData, Calculate::skewness, values, mean, stdDev));
    m_kurtosisLabel->setText(calculateAndFormat(hasData, Calculate::kurtosis, values, mean, stdDev));
    m_madLabel->setText(calculateAndFormat(hasData, Calculate::medianAbsoluteDeviation, values));
    m_robustStdLabel->setText(calculateAndFormat(hasData, Calculate::robustStandardDeviation, values));
}

void MainWindow::updateStatisticalTests(bool hasData, const std::vector<double>& values, double mean) {
    m_shapiroWilkLabel->setText(calculateAndFormat(hasData, Calculate::shapiroWilkTest, values));
    m_densityLabel->setText(calculateAndFormat(hasData, Calculate::calculateDensity, values, mean));
    m_chiSquareLabel->setText(calculateAndFormat(hasData, Calculate::chiSquareTest, values));
    m_kolmogorovLabel->setText(calculateAndFormat(hasData, Calculate::kolmogorovSmirnovTest, values));
}

void MainWindow::updateExtremes(bool hasData, double min, double max, double range) {
    auto format = [](double val){ return QString::number(val, 'f', statsPrecision); };
    m_minLabel->setText(hasData ? format(min) : na);
    m_maxLabel->setText(hasData ? format(max) : na);
    m_rangeLabel->setText(hasData ? format(range) : na);
}

void MainWindow::updateUI(const TableData& data) {
    const bool hasData = !data.empty() && !data[0].empty();
    std::vector<double> values;

    if(hasData) {
        values.reserve(data[0].size());
        for(const auto& pair : data[0]) {
            values.push_back(pair.second);
        }
    }

    const double mean = hasData ? Calculate::getMean(values) : 0.0;
    const double stdDev = hasData ? Calculate::getStandardDeviation(values, mean) : 0.0;
    const double min = hasData ? *std::min_element(values.begin(), values.end()) : 0.0;
    const double max = hasData ? *std::max_element(values.begin(), values.end()) : 0.0;
    const double range = max - min;

    updateBasicMetrics(hasData, values, mean);
    updateAverages(hasData, values);
    updateDistribution(hasData, values, mean, stdDev);
    updateStatisticalTests(hasData, values, mean);
    updateExtremes(hasData, min, max, range);
}

void MainWindow::updateStatistics() {
    if (!areAllLabelsDefined()) return;

    const TableData data = parse();
    updateUI(data);
    plotData(data);
    for(int i = 0; i < m_table->rowCount(); ++i) {
        updateButtonsState(i);
    }
    refreshLegend();
}

void MainWindow::updateXAxisTitle() {
    if(m_chartView && m_chartView->chart()) {
        // Получаем все горизонтальные оси
        QList<QAbstractAxis*> xAxes = m_chartView->chart()->axes(Qt::Horizontal);
        if(!xAxes.isEmpty()) {
            if(QValueAxis* xAxis = qobject_cast<QValueAxis*>(xAxes.first())) {
                xAxis->setTitleText(m_xAxisTitleEdit->text());
            }
        }
    }
}

void MainWindow::updateYAxisTitle() {
    if(m_chartView && m_chartView->chart()) {
        // Получаем все вертикальные оси
        QList<QAbstractAxis*> yAxes = m_chartView->chart()->axes(Qt::Vertical);
        if(!yAxes.isEmpty()) {
            if(QValueAxis* yAxis = qobject_cast<QValueAxis*>(yAxes.first())) {
                yAxis->setTitleText(m_yAxisTitleEdit->text());
            }
        }
    }
}

void MainWindow::setupPalette() {
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    qApp->setPalette(darkPalette);
}

void MainWindow::setupTableSlots() {
    connect(m_table, &QTableWidget::currentCellChanged, [this](int row, int, int, int) {
        updateStatistics();
    });

    connect(m_table, &QTableWidget::itemChanged, this, &MainWindow::updateStatistics);

    connect(m_table->model(), &QAbstractItemModel::dataChanged, [this]() {
        const int rows = m_table->rowCount();
        const int buttonsCount = qMin(m_minButtons.size(), m_maxButtons.size());

        for (int i = 0; i < qMin(rows, buttonsCount); ++i) {
            if (m_minButtons[i]->isChecked() || m_maxButtons[i]->isChecked()) {
                QTimer::singleShot(0, [this, i]() { // Добавляем задержку
                    if (i < m_table->rowCount()) { // Проверка актуальности индекса
                        if (m_minButtons[i]->isChecked()) updateMarker(i, false);
                        if (m_maxButtons[i]->isChecked()) updateMarker(i, true);
                    }
                });
            }
        }
    });
}

void MainWindow::loadStylesheets() {
    QFile styleFile(":/stylesheets/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    qApp->setStyleSheet(style);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    loadStylesheets();

    // Объявляем переменные для хранения элементов управления
    QLineEdit* xAxisEdit = nullptr;
    QLineEdit* yAxisEdit = nullptr;
    QWidget* seriesContent = nullptr;

    QWidget *dataSection = setupDataSection(mainWidget);
    QWidget* graphSection = Draw::setupGraphSection(
        mainWidget,
        &xAxisEdit,
        &yAxisEdit,
        &seriesContent
        );

    // Сохраняем ссылки на элементы управления
    m_xAxisTitleEdit = xAxisEdit;
    m_yAxisTitleEdit = yAxisEdit;
    m_seriesSettingsContent = seriesContent;

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(dataSection, 1);
    mainLayout->addWidget(graphSection, 1);

    if (m_table) {
        setupTableSlots();
        initializeChart();
        setupGraphSettingsSlots();

        // Принудительно создаем поля для начальных рядов
        QTimer::singleShot(0, this, [this]() {
            int rows = m_table->rowCount();
            if(rows > 0) {
                handleSeriesAdded(QModelIndex(), 0, rows-1);
            }
        });

        updateStatistics();
    } else {
        qFatal("Table initialization failed!");
    }

    this->setWindowState(Qt::WindowMaximized);
    this->setWindowIcon(QIcon(":/logo.png"));
    this->setWindowTitle(QString::fromStdString("Glacé"));
}

MainWindow::~MainWindow() {}
