#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "globals.h"
#include "draw.h"
#include "calculate.h"
#include "structs.h"
#include "export.h"
#include "import.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QSpinBox>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QMap>
#include <QScrollArea>
#include <QScroller>
#include <QScrollBar>
#include <QChart>
#include <QColor>
#include <QChartView>
#include <QVector>
#include <QXYSeries>
#include <QLineEdit>
#include <QStyleFactory>
#include <QApplication>
#include <QLegendMarker>
#include <QPen>

#include <limits>
#include <iostream>

struct SeriesMarkers {
    QScatterSeries* minMarker = nullptr;
    QScatterSeries* maxMarker = nullptr;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateStatistics();
    void plotData(const TableData& data);
    void updateXAxisTitle();
    void updateYAxisTitle();
    void updateSeriesNames();
    void handleSeriesAdded(const QModelIndex &parent, int first, int last);
    void handleSeriesRemoved(const QModelIndex &parent, int first, int last);
    void handleShowMin(int seriesIndex);
    void handleShowMax(int seriesIndex);

private:
    QWidget* m_seriesSettingsContent;
    QLineEdit* m_xAxisTitleEdit;
    QLineEdit* m_yAxisTitleEdit;
    QTableWidget *m_table = nullptr;
    QPushButton* m_addColBtn = nullptr;
    QPushButton* m_delColBtn = nullptr;
    QPushButton* m_clearBtn = nullptr;
    QPushButton* m_autoSizeBtn = nullptr;
    QSpinBox* m_colSpin = nullptr;
    QSpinBox* m_rowSpin = nullptr;
    QPushButton* m_importBtn = nullptr;
    QPushButton* m_exportBtn = nullptr;
    QPushButton* m_addRowBtn = nullptr;
    QPushButton* m_delRowBtn = nullptr;

    QLabel *m_elementCountLabel = nullptr;
    QLabel *m_sumLabel = nullptr;
    QLabel *m_averageLabel = nullptr;
    QLabel *m_medianLabel = nullptr;
    QLabel *m_modeLabel = nullptr;
    QLabel *m_stdDevLabel = nullptr;
    QLabel *m_minLabel = nullptr;
    QLabel *m_maxLabel = nullptr;
    QLabel *m_rangeLabel = nullptr;
    QLabel* m_geometricMeanLabel = nullptr;
    QLabel* m_skewnessLabel = nullptr;
    QLabel* m_kurtosisLabel = nullptr;
    QLabel* m_harmonicMeanLabel = nullptr;
    QLabel* m_trimmedMeanLabel = nullptr;
    QLabel* m_madLabel = nullptr;
    QLabel* m_rmsLabel = nullptr;
    QLabel* m_robustStdLabel = nullptr;
    QLabel* m_shapiroWilkLabel = nullptr;
    QLabel* m_densityLabel = nullptr;
    QLabel* m_chiSquareLabel = nullptr;
    QLabel* m_kolmogorovLabel = nullptr;
    QChartView* m_chartView = nullptr;
    QValueAxis* m_axisX = nullptr;
    QValueAxis* m_axisY = nullptr;

    QVector<QLineEdit*> m_seriesNameEdits;
    QVector<QColor> m_seriesColors {
        QColor("#A8E6CF"), QColor("#DCEDC1"), QColor("#FFD3B6"), QColor("#FFAAA5"),  // Мятный, салатовый, персиковый, розовый
        QColor("#D4A5A5"), QColor("#FFC8DD"), QColor("#B5EAD7"), QColor("#E2F0CB"),  // Пудровый, малиновый, аквамарин, лайм
        QColor("#FFDAC1"), QColor("#C7CEEA"), QColor("#F8B195"), QColor("#F67280"),  // Карамельный, лавандовый, коралловый, арбузный
        QColor("#6B5B95"), QColor("#88B04B"), QColor("#FF6F61"), QColor("#92A8D1")   // Фиолетовый, оливковый, красный, голубой
    };
    QHash<int, SeriesMarkers> m_seriesMarkers; // Хранит маркеры для каждого ряда
    QVector<QPushButton*> m_minButtons;
    QVector<QPushButton*> m_maxButtons;

    void clearChart();
    QLineSeries* createSeries(int seriesIndex, bool showPoints = false);
    void addPointsToSeries(QLineSeries* series,
                           const std::vector<std::pair<int, int>>& data,
                           double& minX, double& maxX,
                           double& minY, double& maxY);
    void updateAxisRanges(double minX, double maxX, double minY, double maxY);
    TableData parse() const;
    QWidget *setupDataSection(QWidget *parent);
    QWidget *setupDataPanel(QWidget *parent);
    QWidget* createBasicDataSection(QWidget *parent, QLabel **elementCountLabel, QLabel **sumLabel, QLabel **averageLabel);
    QWidget* createMeansSection(QWidget *parent);
    QWidget* createDistributionSection(QWidget *parent);
    QWidget* createExtremesSection(QWidget *parent);
    QWidget* createCorrelationSection(QWidget *parent);
    void updateAxesRange(const TableData& data);
    QWidget* setupTableToolbar(QWidget* parent, QTableWidget* table);
    QWidget* setupTablePanel(QWidget *parent);
    void setupTableActions();
    void updateUI(const TableData& data);
    void createDataHeader(QWidget *statsPanel, QVBoxLayout *statsLayout);
    bool areAllLabelsDefined();
    void setupChartAxes();
    void initializeChart();
    void attachSeriesToAxes(QXYSeries* series);
    void setupGraphSettingsSlots();
    void setupPalette();
    void updateMarker(int seriesIndex, bool isMax);
    std::pair<double, int> findExtremum(int seriesIndex, bool findMax);
    bool isSeriesEmpty(int seriesIndex) const;
    void updateButtonsState(int seriesIndex);
    void setupTableSlots();
    void refreshLegend();
    QColor getSeriesColor(int index) const;
    QPen getSeriesPen(int index) const;
    QColor getBorderColor(int index) const;
    void addPointsToSeriesGraph(int seriesIndex, QLineSeries* series);
    void loadStylesheets();
    void updateBasicMetrics(bool hasData, const std::vector<double>& values, double mean);
    void updateAverages(bool hasData, const std::vector<double>& values);
    void updateDistribution(bool hasData, const std::vector<double>& values, double mean, double stdDev);
    void updateStatisticalTests(bool hasData, const std::vector<double>& values, double mean);
    void updateExtremes(bool hasData, double min, double max, double range);

    template<typename Func, typename... Args>
    QString calculateAndFormat(bool hasData, Func func, Args&&... args) const;
};

#endif // MAINWINDOW_H
