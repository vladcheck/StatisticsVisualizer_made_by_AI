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

namespace Export {
    void exportData(QTableWidget *table, const QList<QPair<QString, QString>>& metrics);
}

#endif // EXPORT_H
