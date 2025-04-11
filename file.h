#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QRegularExpression>

namespace File {
    QString getCsvFilePath(QWidget *parent);
    QStringList readCsvFile(const QString &filePath, QWidget *parent);
    QList<QStringList> parseCsvData(const QStringList &lines, const QRegularExpression &regex);
    void updateTableWithData(QTableWidget *table, const QList<QStringList> &data);
    void importCsvData(QTableWidget *table);
}

#endif // FILE_H
