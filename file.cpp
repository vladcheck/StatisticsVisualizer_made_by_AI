// file.cpp
#include "file.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileInfo>

namespace File {
QString getFilePath(QWidget *parent) {
    return QFileDialog::getOpenFileName(
        parent,
        "Импорт файла данных",
        "",
        "Файлы данных (*.csv *.txt);;Все файлы (*)"
        );
}

QString readSingleLineFile(const QString &filePath, QWidget *parent) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(parent, "Ошибка", "Не удалось открыть файл.");
        return QString();
    }

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        QString currentLine = in.readLine().trimmed();
        if (!currentLine.isEmpty()) {
            if (!line.isEmpty()) { // Найдена вторая непустая строка
                QMessageBox::critical(parent, "Ошибка",
                                      "Файл должен содержать только одну непустую строку.");
                file.close();
                return QString();
            }
            line = currentLine;
        }
    }
    file.close();

    if (line.isEmpty()) {
        QMessageBox::critical(parent, "Ошибка", "Файл пуст.");
    }

    return line;
}

QStringList parseData(const QString &line, const QRegularExpression &regex) {
    return line.split(regex, Qt::SkipEmptyParts);
}

void updateTableWithData(QTableWidget *table, const QStringList &data) {
    table->clearContents();
    table->setRowCount(1); // Всегда одна строка
    table->setColumnCount(data.size());

    for (int j = 0; j < data.size(); ++j) {
        QTableWidgetItem *item = new QTableWidgetItem(data[j].trimmed());
        table->setItem(0, j, item);
    }
    table->resizeColumnsToContents();
}

void importFileData(QTableWidget *table) {
    const QString filePath = getFilePath(table);
    if (filePath.isEmpty()) return;

    const QString line = readSingleLineFile(filePath, table);
    if (line.isEmpty()) return;

    const QRegularExpression dataRegex("[;, \\t-]+");
    const QStringList parsedData = parseData(line, dataRegex);

    if (!parsedData.isEmpty()) {
        updateTableWithData(table, parsedData);
    }
}
}
