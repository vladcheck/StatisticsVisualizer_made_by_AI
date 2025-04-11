#ifndef IMPORT_H
#define IMPORT_H

#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QRegularExpression>

namespace Import {
    QString getFilePath(QWidget *parent);
    QString readSingleLineFile(const QString &filePath, QWidget *parent); // Возвращает одну строку
    QStringList parseData(const QString &line, const QRegularExpression &regex);
    void updateTableWithData(QTableWidget *table, const QStringList &data);
    void importFileData(QTableWidget *table);
}

#endif // IMPORT_H
