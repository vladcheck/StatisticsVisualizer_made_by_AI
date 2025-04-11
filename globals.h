#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

// Таблица
constexpr unsigned int initialRowCount = 1;
constexpr unsigned int initialColCount = 20;

// Расчёты
constexpr int statsPrecision = 3;
constexpr float trimmedMeanPercentage = 0.1;
constexpr int MIN_SAMPLE_SIZE = 5;
constexpr int MAX_SAMPLE_SIZE = 5000;
constexpr double SW_CRITICAL_VALUE = 0.95; // Шапиро-Уилк
// Ядерная оценка плотности
constexpr double KDE_BANDWIDTH = 0.5;  // Параметр сглаживания
constexpr double KDE_EPSILON = 1e-8;   // Для устойчивости вычислений
// Критерий χ²
constexpr double CHI2_BINS = 5.0;           // Количество интервалов
constexpr double CHI2_MIN_EXPECTED = 5.0;
constexpr double ALPHA_LEVEL = 0.05; // Уровни значимости

// Интерфейс
const QString na = "—";
const QString fontName = "Arial";
constexpr unsigned int buttonSize = 36;
constexpr unsigned int buttonIconSize = static_cast<int>(buttonSize * 0.7);

#endif // GLOBALS_H
