#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

// Таблица
const unsigned int initialRowCount = 1;
const unsigned int initialColCount = 20;

// Расчёты
const int statsPrecision = 3;
const float trimmedMeanPercentage = 0.1;

// Интерфейс
const QString na = "—";
const QString fontName = "Arial";
const unsigned int buttonSize = 36;
const unsigned int buttonIconSize = static_cast<int>(buttonSize * 0.7);

#endif // GLOBALS_H
