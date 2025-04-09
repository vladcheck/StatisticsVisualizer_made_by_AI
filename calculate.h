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
};

#endif // CALCULATIONS_H
