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

#include <limits>
#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateStatistics();
    void plotData(const TableData& data);

private:
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
    QVector<QColor> m_seriesColors {
        QColor("#1f77b4"), QColor("#ff7f0e"), QColor("#2ca02c"),
        QColor("#d62728"), QColor("#9467bd"), QColor("#8c564b")
    };

    void clearChart();
    QLineSeries* createSeries(int seriesIndex);
    void addPointsToSeries(QLineSeries* series,
                           const std::vector<std::pair<int, int>>& data,
                           double& minX, double& maxX,
                           double& minY, double& maxY);
    void updateAxisRanges(double minX, double maxX, double minY, double maxY);
    TableData parse() const;
    QWidget *setupDataSection(QWidget *parent);
    QWidget *setupDataPanel(QWidget *parent, QLabel **, QLabel **, QLabel **);
    QWidget* createBasicDataSection(QWidget *parent, QLabel **elementCountLabel, QLabel **sumLabel, QLabel **averageLabel);
    QWidget* createMeansSection(QWidget *parent);
    QWidget* createDistributionSection(QWidget *parent);
    QWidget* createExtremesSection(QWidget *parent);
    QWidget* createCorrelationSection(QWidget *parent);
    void updateAxesRange(const TableData& data);
    QWidget* setupTableToolbar(QWidget* parent, QTableWidget* table);
    QWidget* setupTablePanel(QWidget *parent);
    void setupTableActions();
    void updateUI(const TableData data);
    void createDataHeader(QWidget *statsPanel, QVBoxLayout *statsLayout);
    bool areAllLabelsDefined();
    void setupChartAxes();
    void initializeChart();
    void attachSeriesToAxes(QXYSeries* series);
};
#endif // MAINWINDOW_H
