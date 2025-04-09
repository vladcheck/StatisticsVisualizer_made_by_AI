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
    void updateElementCount();
private:
    QLabel* m_elementCountLabel = nullptr;
    QTableWidget* m_table = nullptr;

    QWidget* setupDataSection(QWidget* parent);
    QWidget* setupStatsPanel(QWidget* parent, QLabel** elementCountLabel);
};
#endif // MAINWINDOW_H
