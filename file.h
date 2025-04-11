// file.h
#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QRegularExpression>

namespace File {
    QString getFilePath(QWidget *parent);
    QString readSingleLineFile(const QString &filePath, QWidget *parent); // Возвращает одну строку
    QStringList parseData(const QString &line, const QRegularExpression &regex);
    void updateTableWithData(QTableWidget *table, const QStringList &data);
    void importFileData(QTableWidget *table);
}

#endif // FILE_H
