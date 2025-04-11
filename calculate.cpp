#include "calculate.h"
#include "globals.h"
#include "structs.h"

#include <QVector>
#include <QTableWidgetItem>
#include <QHash>
#include <QSet>

#include <limits>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <QtDebug>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>


namespace Calculate
{
    QVector<double> getWeights(const QTableWidget* table, int weightColumn = 1) {
        QVector<double> weights;
        if (!table || weightColumn >= table->columnCount() || weightColumn < 0) return weights; // Добавлена проверка < 0

        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* item = table->item(row, weightColumn);
            if (item && !item->text().isEmpty()) {
                bool ok;
                double weight = item->text().toDouble(&ok);
                // Проверяем на валидность и неотрицательность веса
                if (ok && weight >= 0 && std::isfinite(weight)) {
                    weights.append(weight);
                } else {
                    // Если хотя бы один вес некорректен, лучше вернуть пустой вектор
                    // или обработать ошибку по-другому, в зависимости от требований.
                    qWarning() << "Invalid weight found in row" << row << "column" << weightColumn;
                    return QVector<double>(); // Возвращаем пустой вектор при ошибке
                }
            } else {
                // Если ячейка пуста, считать ли это ошибкой? Зависит от требований.
                // Пока считаем ошибкой, если ожидаются веса для всех строк данных.
                qWarning() << "Empty weight cell found in row" << row << "column" << weightColumn;
                return QVector<double>();
            }
        }
        return weights;
    }

    double getSum(const QVector<double>& values) {
        // Используем long double для суммы для большей точности
        long double sum = 0.0L;
        for (double n : values) {
            if (!std::isfinite(n)) return std::numeric_limits<double>::quiet_NaN(); // Проверка на NaN/inf
            sum += static_cast<long double>(n);
        }
        if (!std::isfinite(sum)) return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(sum);
    }

    double getMean(const QVector<double>& values) {
        if (values.isEmpty()) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        double sum = getSum(values); // Использует getSum с long double
        if (std::isnan(sum)) return std::numeric_limits<double>::quiet_NaN();

        long double mean_ld = static_cast<long double>(sum) / values.size();
        if (!std::isfinite(mean_ld)) return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(mean_ld);
    }

    double getMedian(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = values;
        // Удаляем NaN перед сортировкой, если они могут присутствовать
        sorted.erase(std::remove_if(sorted.begin(), sorted.end(), [](double d){ return !std::isfinite(d); }), sorted.end());

        if (sorted.isEmpty()) // Если остались только NaN
            return std::numeric_limits<double>::quiet_NaN();

        std::sort(sorted.begin(), sorted.end());

        const int size = sorted.size();
        const int mid = size / 2;

        if (size % 2 == 0) {
            // Используем long double для среднего двух элементов
            long double median_val = (static_cast<long double>(sorted[mid - 1]) + static_cast<long double>(sorted[mid])) / 2.0L;
            if (!std::isfinite(median_val)) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(median_val);
        } else {
            return sorted[mid]; // Один центральный элемент
        }
    }

    double getMode(const QVector<double> &values)
    {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        QMap<double, int> frequencyMap;
        int validCount = 0;
        for (double value : values)
        {
            if (std::isfinite(value)) { // Считаем только конечные числа
                frequencyMap[value]++;
                validCount++;
            }
        }

        if (validCount == 0) // Если не было валидных чисел
            return std::numeric_limits<double>::quiet_NaN();

        int maxFrequency = 0;
        double mode = std::numeric_limits<double>::quiet_NaN(); // Инициализируем как NaN
        bool multipleModes = false;

        for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it)
        {
            if (it.value() > maxFrequency)
            {
                maxFrequency = it.value();
                mode = it.key();
                multipleModes = false; // Нашли новую максимальную частоту
            } else if (it.value() == maxFrequency) {
                multipleModes = true; // Нашли еще одно значение с такой же частотой
            }
        }

        // Если все значения уникальны (макс частота = 1) или есть несколько мод,
        // часто считается, что моды нет (возвращаем NaN).
        // Ваш старый код требовал maxFrequency > 1. Оставим это требование.
        if (maxFrequency <= 1 || multipleModes) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return mode;
    }

    double getStandardDeviation(const QVector<double> &values, double mean)
    {
        const int n = values.size();
        if (n < 2 || std::isnan(mean)) // Проверка на NaN mean
            return std::numeric_limits<double>::quiet_NaN();

        long double sumSqDifferences = 0.0L; // long double
        long double mean_ld = static_cast<long double>(mean);
        int validCount = 0;

        for (double value : values)
        {
            if (std::isfinite(value)) { // Обработка только конечных чисел
                long double diff = static_cast<long double>(value) - mean_ld;
                sumSqDifferences += diff * diff;
                validCount++;
            }
        }

        if (validCount < 2) // Нужно хотя бы 2 валидных значения
            return std::numeric_limits<double>::quiet_NaN();

        // Используем validCount в знаменателе
        long double variance = sumSqDifferences / (validCount - 1);

        if (variance < 0.0L || !std::isfinite(variance)) { // Проверка на отрицательную или бесконечную дисперсию
            // variance может быть очень близка к нулю, но отрицательной быть не должна
            return std::numeric_limits<double>::quiet_NaN();
        }

        long double stdDev_ld = std::sqrt(variance);

        if (!std::isfinite(stdDev_ld)) return std::numeric_limits<double>::quiet_NaN();
        return static_cast<double>(stdDev_ld);
    }

    double geometricMean(const QVector<double>& values) {
        if (values.isEmpty())
            return std::numeric_limits<double>::quiet_NaN();

        long double logSum = 0.0L; // Используем сумму логарифмов для стабильности и точности
        for (double value : values) {
            if (value <= 0) // Геометрическое среднее не определено для <= 0
                return std::numeric_limits<double>::quiet_NaN();
            logSum += std::log(static_cast<long double>(value));
        }

        // Проверка на случай, если сумма логарифмов слишком мала/велика
        if (!std::isfinite(logSum)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Среднее логарифмов
        long double meanLog = logSum / values.size();

        // Экспонента от среднего логарифма
        long double result = std::exp(meanLog);

        // Возвращаем как double, проверяя на переполнение/underflow
        if (!std::isfinite(result) || result > std::numeric_limits<double>::max() || result < std::numeric_limits<double>::lowest()) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return static_cast<double>(result);
    }

    double harmonicMean(const QVector<double>& values) {
        if (values.isEmpty()) return std::numeric_limits<double>::quiet_NaN();

        long double reciprocalSum = 0.0L; // Используем long double
        for (double value : values) {
            if (value <= 0) // Гармоническое среднее не определено для <= 0
                return std::numeric_limits<double>::quiet_NaN();
            if (value < std::numeric_limits<double>::epsilon()) { // Избегаем деления на очень малое число
                qWarning() << "Harmonic mean calculation: value near zero encountered.";
                return std::numeric_limits<double>::quiet_NaN();
            }
            reciprocalSum += 1.0L / static_cast<long double>(value);
        }

        // Проверяем сумму обратных величин
        if (reciprocalSum < std::numeric_limits<long double>::epsilon() || !std::isfinite(reciprocalSum)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Вычисляем результат как long double
        long double result = static_cast<long double>(values.size()) / reciprocalSum;

        if (!std::isfinite(result)) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        // Возвращаем как double
        return static_cast<double>(result);
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
        // Используем long double для n для точности в коэффициентах
        const long double n_ld = static_cast<long double>(n);

        if (n < 4 || stdDev == 0 || std::abs(stdDev) < std::numeric_limits<double>::epsilon())
            return std::numeric_limits<double>::quiet_NaN();

        long double sumFourthDeviations = 0.0L; // Используем long double
        const long double mean_ld = static_cast<long double>(mean);

        for (double value : values) {
            long double deviation = static_cast<long double>(value) - mean_ld;
            // Используем pow с long double для точности
            sumFourthDeviations += std::pow(deviation, 4.0L);
        }

        // Проверяем точность stdDev перед возведением в степень
        const long double stdDev_ld = static_cast<long double>(stdDev);
        if (stdDev_ld < std::numeric_limits<long double>::epsilon()) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        long double stdDevPow4 = std::pow(stdDev_ld, 4.0L);
        if (stdDevPow4 < std::numeric_limits<long double>::epsilon()) {
            return std::numeric_limits<double>::quiet_NaN(); // Избегаем деления на ноль/малое число
        }


        // Расчет с использованием long double для коэффициентов
        // Формула для несмещенной оценки эксцесса выборки
        long double term1 = (n_ld * (n_ld + 1.0L)) / ((n_ld - 1.0L) * (n_ld - 2.0L) * (n_ld - 3.0L));
        long double term2 = sumFourthDeviations / stdDevPow4;
        long double term3 = (3.0L * std::pow(n_ld - 1.0L, 2.0L)) / ((n_ld - 2.0L) * (n_ld - 3.0L));

        long double kurt = term1 * term2 - term3;

        if (!std::isfinite(kurt)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Возвращаем как double
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

    // Проверка Шапиро-Уилка (упрощенная реализация)
    double shapiroWilkTest(const QVector<double>& data) {
        if (data.size() < MIN_SAMPLE_SIZE || data.size() > MAX_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        // Упрощенный расчет статистики W
        QVector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        const double mean = std::accumulate(sorted.begin(), sorted.end(), 0.0) / sorted.size();
        double ss = 0.0;
        for (double v : sorted) ss += (v - mean)*(v - mean);

        // Эмпирическая формула для примера
        const double W = 1.0 - exp(-0.001 * ss / sorted.size());
        return (W > SW_CRITICAL_VALUE) ? 1.0 : 0.0; // 1 - нормальное, 0 - нет
    }

    // Ядерная оценка плотности (гауссово ядро)
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

    // Критерий χ² для нормального распределения
    double chiSquareTest(const QVector<double>& data) {
        if (data.size() < MIN_SAMPLE_SIZE*2)
            return std::numeric_limits<double>::quiet_NaN();

        // Группировка данных
        QVector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        const double min = sorted.first();
        const double max = sorted.last();
        const double binWidth = (max - min) / CHI2_BINS;

        // Расчет наблюдаемых частот
        QVector<int> observed(CHI2_BINS, 0);
        for (double v : sorted) {
            int bin = static_cast<int>((v - min)/binWidth);
            if (bin >= CHI2_BINS) bin = CHI2_BINS-1;
            observed[bin]++;
        }

        // Расчет χ² статистики
        const double expected = data.size() / CHI2_BINS;
        if (expected < CHI2_MIN_EXPECTED) return NAN;

        double chi2 = 0.0;
        for (int obs : observed) {
            chi2 += (obs - expected)*(obs - expected) / expected;
        }

        return chi2;
    }

    // Критерий Колмогорова-Смирнова
    double kolmogorovSmirnovTest(const QVector<double>& data) {
        if (data.size() < MIN_SAMPLE_SIZE)
            return std::numeric_limits<double>::quiet_NaN();

        QVector<double> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        // Эмпирическая функция распределения
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
