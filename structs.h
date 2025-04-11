#ifndef STRUCTS_H
#define STRUCTS_H

#include <QVector>
#include <QString>
#include <QTableWidget>
#include <QPushButton>
#include <QSpinBox>

struct TableActions {
    QTableWidget* table;
    QPushButton* addRowBtn;
    QPushButton* addColBtn;
    QPushButton* delRowBtn;
    QPushButton* delColBtn;
    QPushButton* clearButton;
    QPushButton* autoSizeBtn;
    QSpinBox* rowSpin;
    QSpinBox* colSpin;
};

struct TableRow {
    QVector<double> values;
    QVector<double> weights;
    QVector<QString> categories;
    bool isValid = false;
    int count = 0;
    double sum = 0.0;
    double mean = 0.0;
};

#endif // STRUCTS_H
