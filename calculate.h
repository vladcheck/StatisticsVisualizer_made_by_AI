#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QTableWidgetItem>
#include <QString>

namespace Calculate
{
    std::vector<double> getWeights(const QTableWidget* table, int weightColumn);
    std::vector<double> findWeights(const QTableWidget* table); // Автоматический поиск столбца с весами
    double getSum(const std::vector<double>& values);
    double getMean(const std::vector<double>& values);
    double getMedian(const std::vector<double>& values);
    double getMode(const std::vector<double> &values);
    double getStandardDeviation(const std::vector<double> &values, double mean);
    double geometricMean(const std::vector<double>& values);
    double harmonicMean(const std::vector<double>& values);
    double weightedMean(const std::vector<double>& values, const std::vector<double>& weights);
    double rootMeanSquare(const std::vector<double>& values);
    double skewness(const std::vector<double>& values, double mean, double stdDev);
    double kurtosis(const std::vector<double>& values, double mean, double stdDev);
    double trimmedMean(const std::vector<double>& values, double trimFraction);
    double medianAbsoluteDeviation(const std::vector<double>& values);
    double robustStandardDeviation(const std::vector<double>& values);
    double modalFrequency(const std::vector<QString>& categories);
    double simpsonDiversityIndex(const std::vector<QString>& categories);
    double uniqueValueRatio(const std::vector<QString>& categories);
    double entropy(const std::vector<QString>& categories);
    double shapiroWilkTest(const std::vector<double>& data);
    double calculateDensity(const std::vector<double>& data, double point);
    double chiSquareTest(const std::vector<double>& data);
    double kolmogorovSmirnovTest(const std::vector<double>& data);
};

#endif // CALCULATIONS_H
