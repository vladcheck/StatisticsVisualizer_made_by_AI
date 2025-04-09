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
private slots:
    void updateStatistics();
private:
    QLabel* m_elementCountLabel = nullptr;
    QLabel* m_sumLabel = nullptr;
    QLabel* m_averageLabel = nullptr;
    QLabel* m_medianLabel = nullptr; // Добавляем новый член
    QTableWidget* m_table = nullptr;

    QWidget* setupDataSection(QWidget* parent);
    QWidget* setupStatsPanel(QWidget* parent, QLabel**, QLabel**, QLabel**);
    double getMedian(QVector<double>& values); // Объявляем функцию
};
#endif // MAINWINDOW_H
