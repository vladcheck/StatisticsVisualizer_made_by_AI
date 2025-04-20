#ifndef DRAW_H
#define DRAW_H

#include "import.h"
#include "export.h"
#include "globals.h"

#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QLineSeries>
#include <QFormLayout>
#include <QLineEdit>
#include <QLegendMarker>

namespace Draw
{
    // Для QAction
    template <typename Func>
    static void connect(QAction *action, Func &&callback)
    {
        QObject::connect(action, &QAction::triggered,
                         action, std::forward<Func>(callback));
    }
    // Для QSpinBox
    template <typename Func>
    static void connect(QSpinBox *spinBox, Func &&callback)
    {
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                         spinBox, std::forward<Func>(callback));
    }
    // Для QPushButton
    template <typename Func>
    static void connect(QPushButton *button, Func &&callback)
    {
        QObject::connect(button, &QPushButton::clicked,
                         button, std::forward<Func>(callback));
    }
    QWidget* createSeriesSettingsPanel(QWidget* parent, QWidget** contentWidget);
    QWidget* createSeriesNameRow(QWidget* parent, int seriesIndex, QLineEdit** edit, QPushButton** minBtn, QPushButton** maxBtn);
    void setSizePolicyExpanding(QWidget *w);
    void setSizePolicyFixed(QWidget *w);
    void setupTableActions();
    QScatterSeries* createMarker(double x, double y, QChart* chart, QValueAxis* axisX, QValueAxis* axisY, bool isMax, int markerSize = 10);
    QTableWidget *setupTable(QWidget *parent);
    void createDataHeader(QWidget *statsPanel, QVBoxLayout *statsLayout);
    QWidget *setupTablePanel(QWidget *parent, QTableWidget **outTable);
    QWidget *createSeparator(bool horizontal);
    QSpinBox *createSpinBox(QWidget *parent, int max, int value, int min);
    QHBoxLayout *createSpinBoxWithLabel(QWidget *parent, const std::string text, int max, int min);
    QPushButton *createToolButton(const QString &tooltip, const QString &iconName);
    QWidget *createStatRow(QWidget *parent, const QString &title, const QString &value, const QString &objectName);
    QLabel *createAndRegisterStatRow(QWidget *parent, QLayout *layout, const QString &title, const QString &defaultValue, const QString &objectName);
    QWidget *createStatSection(QWidget *parent, const QString &title);     // Создание секции с заголовком
    void addStatRows(QWidget *parent, QLayout *layout, const std::initializer_list<QPair<QString, QString>> &rows);
    void updateStatValue(QWidget *section, const QString &title, const QString &value);
    QWidget* setupGraphSection(QWidget* parent, QLineEdit** xAxisEdit, QLineEdit** yAxisEdit, QWidget** seriesContent);
    QValueAxis* setupAxis(QString name, int a, int b);
    QSplitter* addSplitter(QWidget* parent, QWidget* w1, QWidget* w2, int stretch1, int stretch2);
};

#endif // DRAW_H
