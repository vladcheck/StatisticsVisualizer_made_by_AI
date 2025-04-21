#ifndef IMPORT_H
#define IMPORT_H

#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QRegularExpression>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileInfo>

#include "mainwindow.h"

namespace Import {
    QString getFilePath(QWidget *parent);
    QString readSingleLineFile(const QString &filePath, QWidget *parent); // Возвращает одну строку
    QStringList parseData(const QString &line, const QRegularExpression &regex);
    void updateTableWithData(QTableWidget *table, const QStringList &data);
    void importFile(QTableWidget *table);
}

#endif // IMPORT_H
