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
    QLabel* m_weightedMeanLabel = nullptr;
    QLabel* m_trimmedMeanLabel = nullptr;
    QLabel* m_madLabel = nullptr;
    QLabel* m_rmsLabel = nullptr;
    QLabel* m_robustStdLabel = nullptr;
    QLabel* m_modalFreqLabel = nullptr;         // Модальная частота
    QLabel* m_simpsonIndexLabel = nullptr;      // Индекс Симпсона
    QLabel* m_uniqueRatioLabel = nullptr;       // Доля уникальных
    QLabel* m_covarianceLabel = nullptr;        // Ковариация
    QLabel* m_spearmanLabel = nullptr;          // Спирмен
    QLabel* m_kendallLabel = nullptr;           // Кендалл
    QLabel* m_pearsonLabel = nullptr;           // Пирсон
    QLabel* m_entropyLabel = nullptr;           // Энтропия
    QLabel* m_shapiroWilkLabel = nullptr;
    QLabel* m_densityLabel = nullptr;
    QLabel* m_chiSquareLabel = nullptr;
    QLabel* m_kolmogorovLabel = nullptr;

    QWidget *setupDataSection(QWidget *parent);
    QWidget *setupStatsPanel(QWidget *parent, QLabel **, QLabel **, QLabel **);
    QWidget* createBasicStatsSection(QWidget *parent, QLabel **elementCountLabel, QLabel **sumLabel, QLabel **averageLabel);
    QWidget* createMeansSection(QWidget *parent);
    QWidget* createDistributionSection(QWidget *parent);
    QWidget* createExtremesSection(QWidget *parent);
    QWidget* createCategoricalSection(QWidget *parent);
    QWidget* createCorrelationSection(QWidget *parent);
    void getTableValues(QVector<double>& values, int& count, double& sum);
    void getCategorialData(QVector<QString> &categories);
    void getCorrelationalData(QVector<double>& xData, QVector<double>& yData, int xColumn = 0, int yColumn = 1);
    bool hasPairs(const QVector<double>& xData) const;
    bool hasValidSpearman(const QVector<double>& xData) const;
    bool hasValidKendall(const QVector<double>& xData) const;
    bool hasCatData(const QVector<QString>& categories) const;
    void createStatsHeader(QWidget *statsPanel, QVBoxLayout *statsLayout);
    bool areAllLabelsDefined();
};
#endif // MAINWINDOW_H
