#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <limits>
#include <iostream>
#include "structs.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateStatistics();

private:
    QTableWidget *m_table = nullptr;
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

    TableRow parse() const;
    QWidget *setupDataSection(QWidget *parent);
    QWidget *setupDataPanel(QWidget *parent, QLabel **, QLabel **, QLabel **);
    QWidget* createBasicDataSection(QWidget *parent, QLabel **elementCountLabel, QLabel **sumLabel, QLabel **averageLabel);
    QWidget* createMeansSection(QWidget *parent);
    QWidget* createDistributionSection(QWidget *parent);
    QWidget* createExtremesSection(QWidget *parent);
    QWidget* createCorrelationSection(QWidget *parent);
    void updateUI(const TableRow& rowData);
    void createDataHeader(QWidget *statsPanel, QVBoxLayout *statsLayout);
    bool areAllLabelsDefined();
};
#endif // MAINWINDOW_H
