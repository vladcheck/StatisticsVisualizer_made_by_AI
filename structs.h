#ifndef STRUCTS_H
#define STRUCTS_H

#include <QVector>
#include <QString>
#include <QTableWidget>
#include <QPushButton>
#include <QSpinBox>

struct TableActions {
    QTableWidget* table;
    QPushButton* addColBtn;
    QPushButton* delColBtn;
    QPushButton* clearButton;
    QPushButton* autoSizeBtn;
    QSpinBox* colSpin;
};

struct TableRow {
    QVector<double> values;
    bool isValid = false;
};

#endif // STRUCTS_H
