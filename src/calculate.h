#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QVector>
#include <QTableWidgetItem>
#include <QString>

namespace Calculate
{
    QVector<double> getWeights(const QTableWidget* table, int weightColumn);
    QVector<double> findWeights(const QTableWidget* table); // Автоматический поиск столбца с весами
    double getSum(const QVector<double>& values);
    double getMean(const QVector<double>& values);
    double getMedian(const QVector<double>& values);
    double getMode(const QVector<double> &values);
    double getStandardDeviation(const QVector<double> &values, double mean);
    double geometricMean(const QVector<double>& values);
    double harmonicMean(const QVector<double>& values);
    double weightedMean(const QVector<double>& values, const QVector<double>& weights);
    double rootMeanSquare(const QVector<double>& values);
    double skewness(const QVector<double>& values, double mean, double stdDev);
    double kurtosis(const QVector<double>& values, double mean, double stdDev);
    double trimmedMean(const QVector<double>& values, double trimFraction);
    double medianAbsoluteDeviation(const QVector<double>& values);
    double robustStandardDeviation(const QVector<double>& values);
    double modalFrequency(const QVector<QString>& categories);
    double simpsonDiversityIndex(const QVector<QString>& categories);
    double uniqueValueRatio(const QVector<QString>& categories);
    double entropy(const QVector<QString>& categories);
    double shapiroWilkTest(const QVector<double>& data);
    double calculateDensity(const QVector<double>& data, double point);
    double chiSquareTest(const QVector<double>& data);
    double kolmogorovSmirnovTest(const QVector<double>& data);
};

#endif // CALCULATIONS_H
