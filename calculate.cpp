#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "calculate.h"

namespace Calculate
{
    bool areWeightsValid(const QVector<double> &weights, const QVector<double> &values)
    {
        return (weights.size() == values.size()) && !weights.isEmpty();
    }

    std::vector<double> getWeights(const QTableWidget *table, int weightColumn = 1)
    {
        std::vector<double> weights;
        if (!table || weightColumn >= table->columnCount() || weightColumn < 0)
            return weights;

        for (int row = 0; row < table->rowCount(); ++row)
        {
            QTableWidgetItem *item = table->item(row, weightColumn);
            if (item && !item->text().isEmpty())
            {
                bool ok;
                double weight = item->text().toDouble(&ok);
                if (ok && weight >= 0 && std::isfinite(weight))
                {
                    weights.push_back(weight);
                }
                else
                {
                    qWarning() << "Invalid weight found in row" << row << "column" << weightColumn;
                    return std::vector<double>();
                }
            }
            else
            {
                qWarning() << "Empty weight cell found in row" << row << "column" << weightColumn;
                return std::vector<double>();
            }
        }
        return weights;
    }

    double getSum(const std::vector<double> &values)
    {
        long double sum = std::accumulate(values.begin(), values.end(), 0.0L);
        if (!std::isfinite(sum))
            return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(sum);
    }

    double getMean(const std::vector<double> &values)
    {
        if (values.empty())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        long double sum = std::accumulate(values.begin(), values.end(), 0.0L);
        if (!std::isfinite(sum))
            return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(sum / values.size());
    }

    double getMedian(const std::vector<double> &values)
    {
        if (values.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::vector<double> sorted = values;
        sorted.erase(std::remove_if(sorted.begin(), sorted.end(), [](double d)
                                    { return !std::isfinite(d); }),
                     sorted.end());

        if (sorted.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::sort(sorted.begin(), sorted.end());

        const int size = sorted.size();
        const int mid = size / 2;

        if (size % 2 == 0)
        {
            long double median_val = (static_cast<long double>(sorted[mid - 1]) + static_cast<long double>(sorted[mid])) / 2.0L;
            if (!std::isfinite(median_val))
                return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(median_val);
        }
        else
        {
            return sorted[mid];
        }
    }

    double getMode(const std::vector<double> &values)
    {
        if (values.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::map<double, int> frequencyMap;
        for (double value : values)
        {
            if (std::isfinite(value))
            {
                frequencyMap[value]++;
            }
        }

        if (frequencyMap.empty())
            return std::numeric_limits<double>::quiet_NaN();

        int maxFrequency = 0;
        double mode = std::numeric_limits<double>::quiet_NaN();
        bool multipleModes = false;

        for (const auto &pair : frequencyMap)
        {
            if (pair.second > maxFrequency)
            {
                maxFrequency = pair.second;
                mode = pair.first;
                multipleModes = false;
            }
            else if (pair.second == maxFrequency)
            {
                multipleModes = true;
            }
        }

        if (maxFrequency <= 1 || multipleModes)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return mode;
    }

    double getStandardDeviation(const std::vector<double> &values, double mean)
    {
        const int n = values.size();
        if (n < 2 || std::isnan(mean))
            return std::numeric_limits<double>::quiet_NaN();

        long double sumSqDifferences = 0.0L;
        long double mean_ld = static_cast<long double>(mean);
        int validCount = 0;

        for (double value : values)
        {
            if (std::isfinite(value))
            {
                long double diff = static_cast<long double>(value) - mean_ld;
                sumSqDifferences += diff * diff;
                validCount++;
            }
        }

        if (validCount < 2)
            return std::numeric_limits<double>::quiet_NaN();

        long double variance = sumSqDifferences / (validCount - 1);

        if (variance < 0.0L || !std::isfinite(variance))
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double stdDev_ld = std::sqrt(variance);

        if (!std::isfinite(stdDev_ld))
            return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(stdDev_ld);
    }

    double geometricMean(const std::vector<double> &values)
    {
        if (values.empty())
            return std::numeric_limits<double>::quiet_NaN();

        long double logSum = 0.0L;
        for (double value : values)
        {
            if (value <= 0)
                return std::numeric_limits<double>::quiet_NaN();
            logSum += std::log(static_cast<long double>(value));
        }

        if (!std::isfinite(logSum))
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double meanLog = logSum / values.size();
        long double result = std::exp(meanLog);

        if (!std::isfinite(result) || result > std::numeric_limits<double>::max() || result < std::numeric_limits<double>::lowest())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return static_cast<double>(result);
    }

    double harmonicMean(const std::vector<double> &values)
    {
        if (values.empty())
            return std::numeric_limits<double>::quiet_NaN();

        long double reciprocalSum = 0.0L;
        for (double value : values)
        {
            if (value <= 0)
                return std::numeric_limits<double>::quiet_NaN();
            if (value < std::numeric_limits<double>::epsilon())
            {
                qWarning() << "Harmonic mean calculation: value near zero encountered.";
                return std::numeric_limits<double>::quiet_NaN();
            }
            reciprocalSum += 1.0L / static_cast<long double>(value);
        }

        if (reciprocalSum < std::numeric_limits<long double>::epsilon() || !std::isfinite(reciprocalSum))
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double result = static_cast<long double>(values.size()) / reciprocalSum;

        if (!std::isfinite(result))
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return static_cast<double>(result);
    }

    double weightedMean(const std::vector<double> &values, const std::vector<double> &weights)
    {
        qDebug() << "=== weightedMean calculation ===";
        qDebug() << "Values:" << values;
        qDebug() << "Weights:" << weights;

        if (values.size() != weights.size())
        {
            qDebug() << "Error: Values and weights size mismatch";
            return std::numeric_limits<double>::quiet_NaN();
        }

        if (values.empty() || weights.empty())
        {
            qDebug() << "Error: Empty input data";
            return std::numeric_limits<double>::quiet_NaN();
        }

        double sumProducts = 0.0, sumWeights = 0.0;
        for (int i = 0; i < values.size(); ++i)
        {
            sumProducts += values[i] * weights[i];
            sumWeights += weights[i];
        }

        if (sumWeights <= 0)
        {
            qDebug() << "Error: Sum of weights is zero";
            return std::numeric_limits<double>::quiet_NaN();
        }

        return sumProducts / sumWeights;
    }

    std::vector<double> findWeights(const QTableWidget *table)
    {
        const int colCount = table->columnCount();

        for (int col = 0; col < colCount; ++col)
        {
            std::vector<double> candidateWeights;
            bool validColumn = true;

            for (int row = 0; row < table->rowCount(); ++row)
            {
                QTableWidgetItem *item = table->item(row, col);
                if (!item || item->text().isEmpty())
                {
                    validColumn = false;
                    break;
                }

                bool ok;
                const double value = item->text().toDouble(&ok);
                if (!ok || value < 0)
                {
                    validColumn = false;
                    break;
                }

                candidateWeights.push_back(value);
            }

            if (validColumn && !candidateWeights.empty())
            {
                qDebug() << "Found weights in column" << col;
                return candidateWeights;
            }
        }

        qDebug() << "No valid weights column found. Using uniform weights.";
        return std::vector<double>(table->rowCount(), 1.0);
    }

    double rootMeanSquare(const std::vector<double> &values)
    {
        if (values.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::vector<double> squares(values.size());
        std::transform(values.begin(), values.end(), squares.begin(), [](double x)
                       { return x * x; });
        double sumSquares = std::accumulate(squares.begin(), squares.end(), 0.0);
        return std::sqrt(sumSquares / values.size());
    }

    double skewness(const std::vector<double> &values, double mean, double stdDev)
    {
        const int n = values.size();
        if (n < 3 || stdDev == 0)
            return std::numeric_limits<double>::quiet_NaN();

        double sumCubedDeviations = 0.0;
        for (double value : values)
        {
            sumCubedDeviations += std::pow(value - mean, 3);
        }

        const double factor = n / static_cast<double>((n - 1) * (n - 2));
        return factor * (sumCubedDeviations / std::pow(stdDev, 3));
    }

    double kurtosis(const std::vector<double> &values, double mean, double stdDev)
    {
        const int n = values.size();
        const long double n_ld = static_cast<long double>(n);

        if (n < 4 || stdDev == 0 || std::abs(stdDev) < std::numeric_limits<double>::epsilon())
            return std::numeric_limits<double>::quiet_NaN();

        long double sumFourthDeviations = 0.0L;
        const long double mean_ld = static_cast<long double>(mean);

        for (double value : values)
        {
            long double deviation = static_cast<long double>(value) - mean_ld;
            sumFourthDeviations += std::pow(deviation, 4.0L);
        }

        const long double stdDev_ld = static_cast<long double>(stdDev);
        if (stdDev_ld < std::numeric_limits<long double>::epsilon())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        long double stdDevPow4 = std::pow(stdDev_ld, 4.0L);
        if (stdDevPow4 < std::numeric_limits<long double>::epsilon())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double term1 = (n_ld * (n_ld + 1.0L)) / ((n_ld - 1.0L) * (n_ld - 2.0L) * (n_ld - 3.0L));
        long double term2 = sumFourthDeviations / stdDevPow4;
        long double term3 = (3.0L * std::pow(n_ld - 1.0L, 2.0L)) / ((n_ld - 2.0L) * (n_ld - 3.0L));

        long double kurt = term1 * term2 - term3;

        if (!std::isfinite(kurt))
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return static_cast<double>(kurt);
    }

    double trimmedMean(const std::vector<double> &values, double trimFraction = 0.1)
    {
        if (values.empty() || trimFraction < 0 || trimFraction >= 0.5)
            return std::numeric_limits<double>::quiet_NaN();

        std::vector<double> sorted = values;
        std::sort(sorted.begin(), sorted.end());

        const int removeCount = static_cast<int>(sorted.size() * trimFraction);
        const int start = removeCount;
        const int end = sorted.size() - removeCount;

        if (start >= end)
            return std::numeric_limits<double>::quiet_NaN();

        double sum = 0.0;
        for (int i = start; i < end; ++i)
        {
            sum += sorted[i];
        }

        return sum / (end - start);
    }

    double medianAbsoluteDeviation(const std::vector<double> &values)
    {
        if (values.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::vector<double> sorted = values;
        std::sort(sorted.begin(), sorted.end());
        const double median = getMedian(sorted);

        std::vector<double> deviations;
        for (double value : values)
        {
            deviations.push_back(std::abs(value - median));
        }

        return getMedian(deviations);
    }

    double robustStandardDeviation(const std::vector<double> &values)
    {
        const double mad = medianAbsoluteDeviation(values);
        return (mad != 0.0 && !std::isnan(mad)) ? 1.4826 * mad
                                                : std::numeric_limits<double>::quiet_NaN();
    }

    double modalFrequency(const std::vector<QString> &categories)
    {
        if (categories.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::map<QString, int> freqMap;
        for (const auto &cat : categories)
            freqMap[cat]++;

        int maxFreq = 0;
        for (const auto &pair : freqMap)
        {
            if (pair.second > maxFreq)
                maxFreq = pair.second;
        }

        return static_cast<double>(maxFreq) / categories.size();
    }

    double simpsonDiversityIndex(const std::vector<QString> &categories)
    {
        if (categories.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::map<QString, int> freqMap;
        for (const auto &cat : categories)
            freqMap[cat]++;

        double sum = 0.0;
        const double total = categories.size();
        for (const auto &pair : freqMap)
        {
            const double p = static_cast<double>(pair.second) / total;
            sum += p * p;
        }

        return 1.0 - sum;
    }

    double uniqueValueRatio(const std::vector<QString> &categories)
    {
        if (categories.empty())
            return std::numeric_limits<double>::quiet_NaN();
        std::unordered_set<QString> unique(categories.begin(), categories.end());
        return static_cast<double>(unique.size()) / categories.size();
    }

    double entropy(const std::vector<QString> &categories)
    {
        if (categories.empty())
            return std::numeric_limits<double>::quiet_NaN();

        std::map<QString, int> freqMap;
        for (const auto &cat : categories)
        {
            freqMap[cat]++;
        }

        double entropy = 0.0;
        const double total = categories.size();
        for (const auto &pair : freqMap)
        {
            const double p = static_cast<double>(pair.second) / total;
            if (p > 0)
                entropy += -p * std::log2(p);
        }

        return entropy;
    }

    double shapiroWilkTest(const std::vector<double> &data)
    {
        if (data.size() < MIN_SAMPLE_SIZE || data.size() > MAX_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        std::vector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        const double mean = std::accumulate(sorted.begin(), sorted.end(), 0.0) / sorted.size();
        double ss = 0.0;
        for (double v : sorted)
            ss += (v - mean) * (v - mean);

        const double W = 1.0 - exp(-0.001 * ss / sorted.size());
        return (W > SW_CRITICAL_VALUE) ? 1.0 : 0.0;
    }

    double calculateDensity(const std::vector<double> &data, double point)
    {
        if (data.empty() || KDE_BANDWIDTH < KDE_EPSILON)
            return std::numeric_limits<double>::quiet_NaN();

        const double h = KDE_BANDWIDTH;
        double sum = 0.0;

        for (double xi : data)
        {
            const double u = (point - xi) / h;
            sum += exp(-0.5 * u * u) / sqrt(2 * M_PI);
        }

        return sum / (data.size() * h);
    }

    // Реализация обратной функции ошибок через метод Ньютона
    double erf_inv(double y) {
        if (y <= -1.0 || y >= 1.0) {
            return std::copysign(std::numeric_limits<double>::infinity(), y);
        }
        constexpr double twoOverSqrtPi = 1.1283791670955126;
        constexpr double eps = 1e-12;
        constexpr int max_iter = 50;
        double x = 0.0;

        // Начальное приближение
        if (y < -0.7) x = -std::sqrt(-std::log((1.0 + y)/2.0));
        else if (y > 0.7) x = std::sqrt(-std::log((1.0 - y)/2.0));
        else x = y * M_PI_2;

        // Уточнение методом Ньютона
        for (int i = 0; i < max_iter; ++i) {
            double err = std::erf(x) - y;
            if (std::abs(err) < eps) break;

            // Производная: d/dx erf(x) = 2/sqrt(π) * exp(-x²)
            x -= err / (twoOverSqrtPi * std::exp(-x*x));
        }
        return x;
    }


    double chiSquareTest(const std::vector<double> &data) {
        if (data.size() < MIN_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        // 1. Оценка параметров распределения
        const double mu = getMean(data);
        const double sigma = getStandardDeviation(data, mu);

        // Проверка edge-case: все данные одинаковые
        if (sigma < std::numeric_limits<double>::epsilon()) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // 2. Создание бинов через квантили
        const int target_bins = CHI2_BINS;
        std::vector<double> bin_edges(target_bins + 1);

        try {
            for (int i = 0; i <= target_bins; ++i) {
                const double p = static_cast<double>(i)/target_bins;
                bin_edges[i] = mu + sigma * std::sqrt(2.0) * erf_inv(2*p - 1);
            }
        } catch (...) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // 3. Подсчет наблюдаемых частот
        std::vector<int> observed(target_bins, 0);
        for (double v : data) {
            auto it = std::upper_bound(bin_edges.begin(), bin_edges.end(), v);
            int bin = std::distance(bin_edges.begin(), it) - 1;
            bin = std::clamp(bin, 0, target_bins-1);
            observed[bin]++;
        }

        // 4. Расчет ожидаемых частот
        std::vector<double> expected(target_bins);
        const double total = data.size();
        for (int i = 0; i < target_bins; ++i) {
            double p_low = 0.5 * (1 + std::erf((bin_edges[i] - mu)/(sigma * M_SQRT2)));
            double p_high = 0.5 * (1 + std::erf((bin_edges[i+1] - mu)/(sigma * M_SQRT2)));
            expected[i] = total * (p_high - p_low);
        }

        // 5. Объединение бинов с малыми ожиданиями
        std::vector<int> obs_merged;
        std::vector<double> exp_merged;
        double curr_exp = 0.0;
        int curr_obs = 0;

        for (int i = 0; i < target_bins; ++i) {
            curr_exp += expected[i];
            curr_obs += observed[i];

            if (curr_exp >= CHI2_MIN_EXPECTED) {
                exp_merged.push_back(curr_exp);
                obs_merged.push_back(curr_obs);
                curr_exp = 0.0;
                curr_obs = 0;
            }
        }

        // Edge-case: недостаточно бинов после объединения
        if (exp_merged.size() < 3)
            return std::numeric_limits<double>::quiet_NaN();

        // 6. Расчет χ² статистики
        double chi2 = 0.0;
        for (size_t i = 0; i < exp_merged.size(); ++i) {
            if (exp_merged[i] < 1e-5) continue;
            chi2 += std::pow(obs_merged[i] - exp_merged[i], 2) / exp_merged[i];
        }

        return chi2;
    }

    double calculateMean(const std::vector<double>& data) {
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    double calculateStdDev(const std::vector<double>& data) {
        const double mean = calculateMean(data);
        double sq_sum = 0.0;
        for (double v : data) {
            sq_sum += (v - mean) * (v - mean);
        }
        return std::sqrt(sq_sum / (data.size() - 1));  // Несмещенная оценка
    }

    double kolmogorovSmirnovTest(const std::vector<double> &data) {
        const int MIN_SAMPLE_SIZE = 30;  // Минимальный размер выборки

        if (data.size() < MIN_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        // 1. Рассчитываем параметры распределения
        const double mu = calculateMean(data);
        const double sigma = calculateStdDev(data);

        // Проверка edge case: нулевое стандартное отклонение
        if (sigma < std::numeric_limits<double>::epsilon()) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // 2. Сортируем данные
        std::vector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        // 3. Вычисляем статистику D
        double D = 0.0;
        const double n = sorted.size();
        const double sqrt2 = std::sqrt(2.0);

        for (size_t i = 0; i < sorted.size(); ++i) {
            // Эмпирическая функция распределения
            const double Fn = (i + 1) / n;  // (i+1) для поправки на непрерывность

            // Теоретическая CDF (нормальное распределение)
            const double z = (sorted[i] - mu) / sigma;
            const double F = 0.5 * (1 + std::erf(z / sqrt2));

            // Максимальное расхождение
            D = std::max(D, std::abs(Fn - F));

            // Проверка для предыдущего значения (требуется для двустороннего сравнения)
            if (i > 0) {
                const double F_prev = 0.5 * (1 + std::erf((sorted[i-1] - mu)/sigma / sqrt2));
                D = std::max(D, std::abs((i/n) - F_prev));
            }
        }

        return D;
    }
}
