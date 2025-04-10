#include "calculate.h"
#include <QVector>
#include <QTableWidgetItem>
#include <QHash>
#include <QSet>

#include <limits>
#include <cmath>

namespace Calculate
{
    QVector<double> getWeights(const QTableWidget* table, int weightColumn = 1) {
        QVector<double> weights;
        if (!table || weightColumn >= table->columnCount()) return weights;

        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* item = table->item(row, weightColumn);
            if (item && !item->text().isEmpty()) {
                bool ok;
                double weight = item->text().toDouble(&ok);
                if (ok && weight >= 0) weights.append(weight);
            }
        }
        return weights;
    }

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

    double weightedMean(const QVector<double>& values, const QVector<double>& weights) {
        qDebug() << "=== weightedMean calculation ===";
        qDebug() << "Values:" << values;
        qDebug() << "Weights:" << weights;

        // Проверка на равенство размеров
        if (values.size() != weights.size()) {
            qDebug() << "Error: Values and weights size mismatch";
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Проверка на пустые данные
        if (values.isEmpty() || weights.isEmpty()) {
            qDebug() << "Error: Empty input data";
            return std::numeric_limits<double>::quiet_NaN();
        }

        double sumProducts = 0.0, sumWeights = 0.0;
        for (int i = 0; i < values.size(); ++i) {
            sumProducts += values[i] * weights[i];
            sumWeights += weights[i];
        }

        // Проверка на нулевые веса
        if (sumWeights <= 0) {
            qDebug() << "Error: Sum of weights is zero";
            return std::numeric_limits<double>::quiet_NaN();
        }

        return sumProducts / sumWeights;
    }

    // 2. Автоматический поиск столбца с весами
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
        // Возвращаем единичные веса, если столбец не найден
        return QVector<double>(table->rowCount(), 1.0);
    }

    double rootMeanSquare(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        double sumSquares = 0.0;
        for (double value : values) {
            sumSquares += std::pow(value, 2);
        }

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
        if (n < 4 || stdDev == 0) return std::numeric_limits<double>::quiet_NaN();

        double sumFourthDeviations = 0.0;
        for (double value : values) {
            sumFourthDeviations += std::pow(value - mean, 4);
        }

        const double biasCorrection = (n * (n + 1)) / static_cast<double>((n - 1) * (n - 2) * (n - 3));
        const double term = sumFourthDeviations / std::pow(stdDev, 4);
        return biasCorrection * term - 3 * std::pow(n - 1, 2) / static_cast<double>((n - 2) * (n - 3));
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

        // Вычисляем медиану исходных данных
        QVector<double> sorted = values;
        std::sort(sorted.begin(), sorted.end());
        const double median = getMedian(sorted);

        // Вычисляем абсолютные отклонения
        QVector<double> deviations;
        for (double value : values) {
            deviations.append(std::abs(value - median));
        }

        // Медиана отклонений
        return getMedian(deviations);
    }

    double robustStandardDeviation(const QVector<double>& values) {
        const double mad = medianAbsoluteDeviation(values);
        return (mad != 0.0 && !std::isnan(mad)) ? 1.4826 * mad
                                                : std::numeric_limits<double>::quiet_NaN();
    }

    double modalFrequency(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        QHash<QString, int> freqMap;
        for (const auto& cat : categories) freqMap[cat]++;

        int maxFreq = 0;
        for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
            if (it.value() > maxFreq) maxFreq = it.value();
        }

        return static_cast<double>(maxFreq) / categories.size();
    }

    double simpsonDiversityIndex(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        QHash<QString, int> freqMap;
        for (const auto& cat : categories) freqMap[cat]++;

        double sum = 0.0;
        const double total = categories.size();
        for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
            const double p = it.value() / total;
            sum += p * p;
        }

        return 1.0 - sum; // Индекс Симпсона (1 - сумма квадратов долей)
    }

    double uniqueValueRatio(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        QSet<QString> unique(categories.begin(), categories.end());
        return static_cast<double>(unique.size()) / categories.size();
    }

    // Ковариация
    double covariance(const QVector<double>& x, const QVector<double>& y) {
        const size_t n = x.size();
        if (n != y.size() || n < 2) return NAN;

        double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
        double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
        double sum_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);

        return (sum_xy - sum_x * sum_y / n) / (n - 1); // Исправленная формула
    }

    // Ранги для Спирмена
    QVector<double> calculateRanks(const QVector<double>& data) {
        QVector<std::pair<double, int>> sorted;
        for (int i = 0; i < data.size(); ++i) {
            sorted.append({data[i], i});
        }
        std::sort(sorted.begin(), sorted.end());

        QVector<double> ranks(data.size());
        for (int i = 0; i < sorted.size(); ++i) {
            int j = i;
            while (j < sorted.size() && sorted[j].first == sorted[i].first) j++;
            const double rank = (i + j - 1) / 2.0;
            for (int k = i; k < j; ++k) {
                ranks[sorted[k].second] = rank + 1; // Ранги от 1
            }
            i = j - 1;
        }
        return ranks;
    }

    double pearsonCorrelation(const QVector<double>& x, const QVector<double>& y) {
        const size_t n = x.size();
        if (n != y.size() || n < 2) return NAN;

        // Вычисляем ковариацию и стандартные отклонения
        const double cov = covariance(x, y);
        const double std_x = getStandardDeviation(x, getMean(std::accumulate(x.begin(), x.end(), 0.0), n));
        const double std_y = getStandardDeviation(y, getMean(std::accumulate(y.begin(), y.end(), 0.0), n));

        if (std_x == 0 || std_y == 0) return NAN;

        return cov / (std_x * std_y);
    }

    // Корреляция Спирмена
    double spearmanCorrelation(const QVector<double>& x, const QVector<double>& y) {
        if (x.size() != y.size() || x.size() < 3) return NAN;

        // Ранжирование с учётом одинаковых значений
        auto rank = [](const QVector<double>& data) {
            QVector<size_t> indexes(data.size());
            std::iota(indexes.begin(), indexes.end(), 0);
            std::sort(indexes.begin(), indexes.end(),
                      [&data](size_t a, size_t b) { return data[a] < data[b]; });

            QVector<double> ranks(data.size());
            for (size_t i = 0; i < indexes.size();) {
                size_t j = i;
                while (j < indexes.size() && data[indexes[j]] == data[indexes[i]]) ++j;
                const double average_rank = 0.5 * (i + j - 1);
                for (size_t k = i; k < j; ++k) {
                    ranks[indexes[k]] = average_rank;
                }
                i = j;
            }
            return ranks;
        };

        QVector<double> rank_x = rank(x);
        QVector<double> rank_y = rank(y);
        return pearsonCorrelation(rank_x, rank_y);
    }

    // Корреляция Кендалла
    double kendallCorrelation(const QVector<double>& x, const QVector<double>& y) {
        const size_t n = x.size();
        if (n < 2 || n != y.size()) return NAN;

        size_t concordant = 0, discordant = 0;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i+1; j < n; ++j) {
                const double x_diff = x[j] - x[i];
                const double y_diff = y[j] - y[i];
                const double product = x_diff * y_diff;

                if (product > 0) concordant++;
                else if (product < 0) discordant++;
            }
        }

        const double total = concordant + discordant;
        return total > 0 ? (concordant - discordant) / total : 0;
    }

    double entropy(const QVector<QString>& categories) {
        if (categories.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        QHash<QString, int> freqMap;
        for (const auto& cat : categories) {
            freqMap[cat]++;
        }

        double entropy = 0.0;
        const double total = categories.size();
        for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
            const double p = it.value() / total;
            if (p > 0) entropy += -p * std::log2(p);
        }

        return entropy;
    }
};
