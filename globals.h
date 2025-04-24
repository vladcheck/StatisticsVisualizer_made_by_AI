#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

// Таблица
constexpr unsigned int initialRowCount = 1;
constexpr unsigned int initialColCount = 100;

// Расчёты
constexpr int statsPrecision = 3;
constexpr float trimmedMeanPercentage = 0.1;
constexpr int MIN_SAMPLE_SIZE = 3;
constexpr int MAX_SAMPLE_SIZE = 5000;
constexpr double SW_CRITICAL_VALUE = 0.05; // Шапиро-Уилк
// Ядерная оценка плотности
constexpr double KDE_BANDWIDTH = 0.5;  // Параметр сглаживания
constexpr double KDE_EPSILON = 1e-8;   // Для устойчивости вычислений
// Критерий χ²
constexpr double CHI2_BINS = 5.0;           // Количество интервалов
constexpr double CHI2_MIN_EXPECTED = 5.0;
constexpr double ALPHA_LEVEL = 0.05; // Уровни значимости
// Критерий Шапиро-Уилка
constexpr double a[] = {-3.969683028665376e+01, 2.209460984245205e+02,
                    -2.759285104469687e+02, 1.383577518672690e+02,
                    -3.066479806614716e+01, 2.506628277459239e+00};

constexpr double b[] = {-5.447609879822406e+01, 1.615858368580409e+02,
                    -1.556989798598866e+02, 6.680131188771972e+01,
                    -1.328068155288572e+01};

// Интерфейс
const QString na = "—";
const QString fontName = "Arial";
constexpr unsigned int buttonSize = 36;
constexpr unsigned int buttonIconSize = static_cast<int>(buttonSize * 0.7);

#endif // GLOBALS_H
