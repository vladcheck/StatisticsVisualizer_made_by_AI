#include "import.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileInfo>

namespace Import {
QRegularExpression regex(R"([,;\t\s]+)");

struct ParsedRow {
    QStringList data;
    int lastNonEmptyIndex = -1;
};

struct ParseResult {
    QVector<ParsedRow> rows;
    int maxColumns = 0;
};

// Вспомогательные функции
bool openFile(QFile& file, QWidget* parent) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(parent, "Ошибка", "Не удалось открыть файл.");
        return false;
    }
    return true;
}

void showError(QWidget* parent, const QString& message) {
    QMessageBox::critical(parent, "Ошибка", message);
}

ParsedRow parseLine(const QString& line) {
    ParsedRow row;
    QStringList tokens = line.split(regex, Qt::SkipEmptyParts);

    for (int i = 0; i < tokens.size(); ++i) {
        QString token = tokens[i].trimmed();
        if (token == "-") {
            row.data.append("");
        } else if (!token.isEmpty()) {
            row.data.append(token);
            row.lastNonEmptyIndex = i;
        }
    }
    return row;
}

void adjustRows(QVector<ParsedRow>& rows, int maxColumns) {
    for (ParsedRow& row : rows) {
        while (row.data.size() > maxColumns)
            row.data.removeLast();

        while (row.data.size() < maxColumns)
            row.data.append("");
    }
}

// Основные функции
QString getFilePath(QWidget* parent) {
    return QFileDialog::getOpenFileName(
        parent,
        "Импорт файла данных",
        "",
        "Файлы данных (*.csv *.txt);;Все файлы (*)"
        );
}

ParseResult readAndParseFile(const QString& filePath, QWidget* parent) {
    QFile file(filePath);
    ParseResult result;

    if (!openFile(file, parent)) return result;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        ParsedRow row = parseLine(line);
        if (row.lastNonEmptyIndex >= 0) {
            result.rows.append(row);
            result.maxColumns = std::max(result.maxColumns, row.lastNonEmptyIndex + 1);
        }
    }
    file.close();

    adjustRows(result.rows, result.maxColumns);
    return result;
}

void updateTable(QTableWidget* table, const ParseResult& result) {
    if (result.rows.isEmpty()) {
        QMessageBox::warning(table, "Предупреждение", "Файл пуст!");
        return;
    }

    table->clearContents();
    table->setRowCount(result.rows.size());
    table->setColumnCount(result.maxColumns);

    for (int i = 0; i < result.rows.size(); ++i) {
        const auto& rowData = result.rows[i].data;
        for (int j = 0; j < result.maxColumns; ++j) {
            if (j < rowData.size() && !rowData[j].isEmpty()) {
                QTableWidgetItem* item = new QTableWidgetItem(rowData[j]);
                table->setItem(i, j, item);
            }
        }
    }
    table->resizeColumnsToContents();
}

void importFile(QTableWidget* table) {
    const QString filePath = getFilePath(table);
    if (filePath.isEmpty()) return;

    const auto parseResult = readAndParseFile(filePath, table);
    updateTable(table, parseResult);
}
}
