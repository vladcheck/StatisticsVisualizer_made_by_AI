#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QSpinBox>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include "iostream"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
// private slots:
//     void clearTable(); // Новый слот для очистки таблицы
// private:
//     QTableWidget *m_table; // Добавляем член класса для хранения указателя на таблицу
//     QSpinBox *m_rowSpinBox; // Указатель на спинбокс строк
//     QSpinBox *m_colSpinBox; // Указатель на спинбокс столбцов
};
#endif // MAINWINDOW_H
