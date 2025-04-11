#include "calculate.h"
#include "globals.h"
#include "structs.h"

#include <QVector>
#include <QTableWidgetItem>
#include <QHash>
#include <QSet>
#include <QtDebug>

#include <limits>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <map>
#include <vector>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>


namespace Calculate
{
    QVector<double> getWeights(const QTableWidget* table, int weightColumn = 1) {
        QVector<double> weights;
        if (!table || weightColumn >= table->columnCount() || weightColumn < 0) return weights;

        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* item = table->item(row, weightColumn);
            if (item && !item->text().isEmpty()) {
                bool ok;
                double weight = item->text().toDouble(&ok);
                if (ok && weight >= 0 && std::isfinite(weight)) {
                    weights.append(weight);
                } else {
                    qWarning() << "Invalid weight found in row" << row << "column" << weightColumn;
                    return QVector<double>();
                }
            } else {
                qWarning() << "Empty weight cell found in row" << row << "column" << weightColumn;
                return QVector<double>();
            }
        }
        return weights;
    }

    double getSum(const QVector<double>& values) {
        long double sum = std::accumulate(values.begin(), values.end(), 0.0L);
        if (!std::isfinite(sum)) return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(sum);
    }

    double getMean(const QVector<double>& values) {
        if (values.isEmpty()) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        long double sum = std::accumulate(values.begin(), values.end(), 0.0L);
        if (!std::isfinite(sum)) return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(sum / values.size());
    }

    double getMedian(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = values;
        sorted.erase(std::remove_if(sorted.begin(), sorted.end(), [](double d){ return !std::isfinite(d); }), sorted.end());

        if (sorted.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        std::sort(sorted.begin(), sorted.end());

        const int size = sorted.size();
        const int mid = size / 2;

        if (size % 2 == 0) {
            long double median_val = (static_cast<long double>(sorted[mid - 1]) + static_cast<long double>(sorted[mid])) / 2.0L;
            if (!std::isfinite(median_val)) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(median_val);
        } else {
            return sorted[mid];
        }
    }

    double getMode(const QVector<double> &values)
    {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        std::map<double, int> frequencyMap;
        for (double value : values)
        {
            if (std::isfinite(value)) {
                frequencyMap[value]++;
            }
        }

        if (frequencyMap.empty())
            return std::numeric_limits<double>::quiet_NaN();

        int maxFrequency = 0;
        double mode = std::numeric_limits<double>::quiet_NaN();
        bool multipleModes = false;

        for (const auto& pair : frequencyMap)
        {
            if (pair.second > maxFrequency)
            {
                maxFrequency = pair.second;
                mode = pair.first;
                multipleModes = false;
            } else if (pair.second == maxFrequency) {
                multipleModes = true;
            }
        }

        if (maxFrequency <= 1 || multipleModes) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return mode;
    }

    double getStandardDeviation(const QVector<double> &values, double mean)
    {
        const int n = values.size();
        if (n < 2 || std::isnan(mean))
            return std::numeric_limits<double>::quiet_NaN();

        long double sumSqDifferences = 0.0L;
        long double mean_ld = static_cast<long double>(mean);
        int validCount = 0;

        for (double value : values)
        {
            if (std::isfinite(value)) {
                long double diff = static_cast<long double>(value) - mean_ld;
                sumSqDifferences += diff * diff;
                validCount++;
            }
        }

        if (validCount < 2)
            return std::numeric_limits<double>::quiet_NaN();

        long double variance = sumSqDifferences / (validCount - 1);

        if (variance < 0.0L || !std::isfinite(variance)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double stdDev_ld = std::sqrt(variance);

        if (!std::isfinite(stdDev_ld)) return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(stdDev_ld);
    }

    double geometricMean(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        long double logSum = 0.0L;
        for (double value : values) {
            if (value <= 0)
                return std::numeric_limits<double>::quiet_NaN();
            logSum += std::log(static_cast<long double>(value));
        }

        if (!std::isfinite(logSum)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double meanLog = logSum / values.size();
        long double result = std::exp(meanLog);

        if (!std::isfinite(result) || result > std::numeric_limits<double>::max() || result < std::numeric_limits<double>::lowest()) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return static_cast<double>(result);
    }

    double harmonicMean(const QVector<double>& values) {
        if (values.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        long double reciprocalSum = 0.0L;
        for (double value : values) {
            if (value <= 0)
                return std::numeric_limits<double>::quiet_NaN();
            if (value < std::numeric_limits<double>::epsilon()) {
                qWarning() << "Harmonic mean calculation: value near zero encountered.";
                return std::numeric_limits<double>::quiet_NaN();
            }
            reciprocalSum += 1.0L / static_cast<long double>(value);
        }

        if (reciprocalSum < std::numeric_limits<long double>::epsilon() || !std::isfinite(reciprocalSum)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double result = static_cast<long double>(values.size()) / reciprocalSum;

        if (!std::isfinite(result)) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return static_cast<double>(result);
    }

    double weightedMean(const QVector<double>& values, const QVector<double>& weights) {
        qDebug() << "=== weightedMean calculation ===";
        qDebug() << "Values:" << values;
        qDebug() << "Weights:" << weights;

        if (values.size() != weights.size()) {
            qDebug() << "Error: Values and weights size mismatch";
            return std::numeric_limits<double>::quiet_NaN();
        }

        if (values.isEmpty() || weights.isEmpty()) {
            qDebug() << "Error: Empty input data";
            return std::numeric_limits<double>::quiet_NaN();
        }

        double sumProducts = 0.0, sumWeights = 0.0;
        for (int i = 0; i < values.size(); ++i) {
            sumProducts += values[i] * weights[i];
            sumWeights += weights[i];
        }

        if (sumWeights <= 0) {
            qDebug() << "Error: Sum of weights is zero";
            return std::numeric_limits<double>::quiet_NaN();
        }

        return sumProducts / sumWeights;
    }

    QVector<double> findWeights(const QTableWidget* table) {
        const int colCount = table->columnCount();

        for (int col = 0; col < colCount; ++col) {
            QVector<double> candidateWeights;
            bool validColumn = true;

            for (int row = 0; row < table->rowCount(); ++row) {
                QTableWidgetItem* item = table->item(row, col);
                if (!item || item->text().isEmpty()) {
                    validColumn = false;
                    break;
                }

                bool ok;
                const double value = item->text().toDouble(&ok);
                if (!ok || value < 0) {
                    validColumn = false;
                    break;
                }

                candidateWeights.append(value);
            }

            if (validColumn && !candidateWeights.isEmpty()) {
                qDebug() << "Found weights in column" << col;
                return candidateWeights;
            }
        }

        qDebug() << "No valid weights column found. Using uniform weights.";
        return QVector<double>(table->rowCount(), 1.0);
    }

    double rootMeanSquare(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        std::vector<double> squares(values.size());
        std::transform(values.begin(), values.end(), squares.begin(), [](double x){ return x * x; });
        double sumSquares = std::accumulate(squares.begin(), squares.end(), 0.0);
        return std::sqrt(sumSquares / values.size());
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
        const long double n_ld = static_cast<long double>(n);

        if (n < 4 || stdDev == 0 || std::abs(stdDev) < std::numeric_limits<double>::epsilon())
            return std::numeric_limits<double>::quiet_NaN();

        long double sumFourthDeviations = 0.0L;
        const long double mean_ld = static_cast<long double>(mean);

        for (double value : values) {
            long double deviation = static_cast<long double>(value) - mean_ld;
            sumFourthDeviations += std::pow(deviation, 4.0L);
        }

        const long double stdDev_ld = static_cast<long double>(stdDev);
        if (stdDev_ld < std::numeric_limits<long double>::epsilon()) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        long double stdDevPow4 = std::pow(stdDev_ld, 4.0L);
        if (stdDevPow4 < std::numeric_limits<long double>::epsilon()) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double term1 = (n_ld * (n_ld + 1.0L)) / ((n_ld - 1.0L) * (n_ld - 2.0L) * (n_ld - 3.0L));
        long double term2 = sumFourthDeviations / stdDevPow4;
        long double term3 = (3.0L * std::pow(n_ld - 1.0L, 2.0L)) / ((n_ld - 2.0L) * (n_ld - 3.0L));

        long double kurt = term1 * term2 - term3;

        if (!std::isfinite(kurt)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return static_cast<double>(kurt);
    }

    double trimmedMean(const QVector<double>& values, double trimFraction = 0.1) {
        if (values.isEmpty() || trimFraction < 0 || trimFraction >= 0.5)
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = values;
        std::sort(sorted.begin(), sorted.end());

        const int removeCount = static_cast<int>(sorted.size() * trimFraction);
        const int start = removeCount;
        const int end = sorted.size() - removeCount;

        if (start >= end) return std::numeric_limits<double>::quiet_NaN();

        double sum = 0.0;
        for (int i = start; i < end; ++i) {
            sum += sorted[i];
        }

        return sum / (end - start);
    }

    double medianAbsoluteDeviation(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = values;
        std::sort(sorted.begin(), sorted.end());
        const double median = getMedian(sorted);

        QVector<double> deviations;
        for (double value : values) {
            deviations.append(std::abs(value - median));
        }

        return getMedian(deviations);
    }

    double robustStandardDeviation(const QVector<double>& values) {
        const double mad = medianAbsoluteDeviation(values);
        return (mad != 0.0 && !std::isnan(mad)) ? 1.4826 * mad
                                                : std::numeric_limits<double>::quiet_NaN();
    }

    double modalFrequency(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        std::map<QString, int> freqMap;
        for (const auto& cat : categories) freqMap[cat]++;

        int maxFreq = 0;
        for (const auto& pair : freqMap) {
            if (pair.second > maxFreq) maxFreq = pair.second;
        }

        return static_cast<double>(maxFreq) / categories.size();
    }

    double simpsonDiversityIndex(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        std::map<QString, int> freqMap;
        for (const auto& cat : categories) freqMap[cat]++;

        double sum = 0.0;
        const double total = categories.size();
        for (const auto& pair : freqMap) {
            const double p = static_cast<double>(pair.second) / total;
            sum += p * p;
        }

        return 1.0 - sum;
    }

    double uniqueValueRatio(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();
        std::unordered_set<QString> unique(categories.begin(), categories.end());
        return static_cast<double>(unique.size()) / categories.size();
    }

    double entropy(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        std::map<QString, int> freqMap;
        for (const auto& cat : categories) {
            freqMap[cat]++;
        }

        double entropy = 0.0;
        const double total = categories.size();
        for (const auto& pair : freqMap) {
            const double p = static_cast<double>(pair.second) / total;
            if (p > 0) entropy += -p * std::log2(p);
        }

        return entropy;
    }

    double shapiroWilkTest(const QVector<double>& data) {
        if (data.size() < MIN_SAMPLE_SIZE || data.size() > MAX_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        const double mean = std::accumulate(sorted.begin(), sorted.end(), 0.0) / sorted.size();
        double ss = 0.0;
        for (double v : sorted) ss += (v - mean)*(v - mean);

        const double W = 1.0 - exp(-0.001 * ss / sorted.size());
        return (W > SW_CRITICAL_VALUE) ? 1.0 : 0.0;
    }

    double calculateDensity(const QVector<double>& data, double point) {
        if (data.isEmpty() || KDE_BANDWIDTH < KDE_EPSILON)
            return std::numeric_limits<double>::quiet_NaN();

        const double h = KDE_BANDWIDTH;
        double sum = 0.0;

        for (double xi : data) {
            const double u = (point - xi) / h;
            sum += exp(-0.5 * u*u) / sqrt(2 * M_PI);
        }

        return sum / (data.size() * h);
    }

    double chiSquareTest(const QVector<double>& data) {
        if (data.size() < MIN_SAMPLE_SIZE*2)
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        const double min = sorted.first();
        const double max = sorted.last();
        const double binWidth = (max - min) / CHI2_BINS;

        QVector<int> observed(CHI2_BINS, 0);
        for (double v : sorted) {
            int bin = static_cast<int>((v - min)/binWidth);
            if (bin >= CHI2_BINS) bin = CHI2_BINS-1;
            observed[bin]++;
        }

        const double expected = data.size() / CHI2_BINS;
        if (expected < CHI2_MIN_EXPECTED) return NAN;

        double chi2 = 0.0;
        for (int obs : observed) {
            chi2 += (obs - expected)*(obs - expected) / expected;
        }

        return chi2;
    }

    double kolmogorovSmirnovTest(const QVector<double>& data) {
        if (data.size() < MIN_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        const double n = sorted.size();
        double D = 0.0;

        for (int i = 0; i < sorted.size(); ++i) {
            const double Fn = (i + 1) / n;
            const double F = 0.5 * (1 + erf((sorted[i] - 0)/1)); // Для N(0,1)
            D = std::max(D, std::abs(Fn - F));
        }

        return D;
    }
};
