#ifndef EXPORT_H
#define EXPORT_H

#include <QTableWidget>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QList>
#include <QString>
#include <QPair>
#include <QHash>

#include <algorithm>
#include <numeric>
#include "calculate.h"
#include "globals.h"
#include "mainwindow.h"

struct TableMetrics {
    int maxNonEmptyCols;
    bool allEmpty;
};


namespace Export {
    TableMetrics calculateTableMetrics(QTableWidget *table);
    QStringList prepareTableRows(QTableWidget *table, int columns);
    QStringList getHeaderLabels(QTableWidget *table, int columns);
    bool processExportDialog(const QString& fileName, const QList<QPair<QString, QString>>& metrics,
                             const QStringList& tableData, const QStringList& seriesHeaders);
    void exportData(QTableWidget *table, const QList<QPair<QString, QString>>& metrics);
    bool writeFileContent(const QString& path, const QList<QPair<QString, QString>>& metrics, const QStringList& data);
}

#endif // EXPORT_H
