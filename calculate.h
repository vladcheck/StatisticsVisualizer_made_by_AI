#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QVector>
#include <limits>
#include <cmath>

namespace Calculate
{
    double getMean(double sum, double count) { return sum / count; }

    double getMedian(QVector<double> &values)
    {
        if (values.isEmpty())
            return 0.0;

        std::sort(values.begin(), values.end());
        const int size = values.size();
        const int mid = size / 2;

        return (size % 2 == 0)
                   ? (values[mid - 1] + values[mid]) / 2.0
                   : values[mid];
    }

    double getMode(const QVector<double> &values)
    {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        QMap<double, int> frequencyMap;
        for (double value : values)
        {
            frequencyMap[value]++;
        }

        int maxFrequency = 0;
        double mode = std::numeric_limits<double>::quiet_NaN();
        for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it)
        {
            if (it.value() > maxFrequency)
            {
                maxFrequency = it.value();
                mode = it.key();
            }
        }

        // Считаем моду существующей только если частота > 1
        return (maxFrequency > 1) ? mode : std::numeric_limits<double>::quiet_NaN();
    }

    double getStandardDeviation(const QVector<double> &values, double mean)
    {
        if (values.size() < 2)
            return std::numeric_limits<double>::quiet_NaN();

        double sumSqDifferences = 0.0;
        for (double value : values)
        {
            sumSqDifferences += std::pow(value - mean, 2);
        }

        // Несмещённая оценка (n-1)
        return std::sqrt(sumSqDifferences / (values.size() - 1));
    }

    double geometricMean(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        double product = 1.0;
        for (double value : values) {
            if (value <= 0)
                return std::numeric_limits<double>::quiet_NaN();
            product *= value;
        }

        return std::pow(product, 1.0 / values.size());
    }

    double harmonicMean(const QVector<double>& values) {
        if (values.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        double reciprocalSum = 0.0;
        for (double value : values) {
            if (value <= 0) return std::numeric_limits<double>::quiet_NaN();
            reciprocalSum += 1.0 / value;
        }

        return values.size() / reciprocalSum;
    }

    double skewness(const QVector<double>& values, double mean, double stdDev) {
        const int n = values.size();
        if (n < 3 || stdDev == 0) return std::numeric_limits<double>::quiet_NaN();

        double sumCubedDeviations = 0.0;
        for (double value : values) {
            sumCubedDeviations += std::pow(value - mean, 3);
        }

        const double factor = n / static_cast<double>((n - 1) * (n - 2));
        return factor * (sumCubedDeviations / std::pow(stdDev, 3));
    }

    double kurtosis(const QVector<double>& values, double mean, double stdDev) {
        const int n = values.size();
        if (n < 4 || stdDev == 0) return std::numeric_limits<double>::quiet_NaN();

        double sumFourthDeviations = 0.0;
        for (double value : values) {
            sumFourthDeviations += std::pow(value - mean, 4);
        }

        const double biasCorrection = (n * (n + 1)) / static_cast<double>((n - 1) * (n - 2) * (n - 3));
        const double term = sumFourthDeviations / std::pow(stdDev, 4);
        return biasCorrection * term - 3 * std::pow(n - 1, 2) / static_cast<double>((n - 2) * (n - 3));
    }
};

#endif // CALCULATIONS_H
