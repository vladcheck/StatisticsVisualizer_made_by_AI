#ifndef TABLEACTIONS_H
#define TABLEACTIONS_H

struct TableActions
{
    QPushButton *addRowBtn;
    QPushButton *addColBtn;
    QPushButton *delRowBtn;
    QPushButton *delColBtn;
    QPushButton *clearButton;
    QPushButton *autoSizeBtn;
    QTableWidget *table;
    QSpinBox *rowSpin;
    QSpinBox *colSpin;
};

#endif // TABLEACTIONS_H
