#ifndef STRUCTS_H
#define STRUCTS_H

#include <QVector>
#include <QString>
#include <QTableWidget>
#include <QPushButton>
#include <QSpinBox>

struct TableRow {
    QVector<double> values;
    bool isValid = false;
};

#endif // STRUCTS_H
