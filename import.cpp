#include "import.h"

namespace Import {
QRegularExpression regex(R"([,;\t\s]+)");

struct ParsedRow {
    QStringList data;
    int lastNonEmptyIndex = -1;
};

struct ParseResult {
    QVector<ParsedRow> rows;
    int maxColumns = 0;
    QStringList seriesHeaders;
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

QList<int> findInvalidDataLines(const QString& filePath, QWidget* parent) {
    QFile file(filePath);
    QList<int> invalidLines;
    QRegularExpression letterRegex("[A-Za-zА-Яа-яЁё]"); // Регулярка для поиска букв

    if (!openFile(file, parent)) return invalidLines;

    QTextStream in(&file);
    int lineNumber = 0;
    int emptyLineCounter = 0;
    const int STOP_LINES = 3;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        lineNumber++;

        if (line.isEmpty()) {
            emptyLineCounter++;
            if (emptyLineCounter >= STOP_LINES) break;
            continue;
        } else {
            emptyLineCounter = 0;

            // Проверка на наличие букв в непустых строках данных
            if (line.contains(letterRegex)) {
                invalidLines.append(lineNumber);
            }
        }
    }
    file.close();
    return invalidLines;
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
    int emptyLineCounter = 0;  // Счетчик пустых строк
    const int STOP_LINES = 3;  // Количество пустых строк для остановки

    QList<int> invalidLines = findInvalidDataLines(filePath, parent);
    if (!invalidLines.isEmpty()) {
        QString errorMsg = "Невозможно импортировать. Найдены буквы в строках:\n";
        for (int ln : invalidLines) {
            errorMsg += QString::number(ln) + ", ";
        }
        errorMsg.chop(2);
        showError(parent, errorMsg);
        return result;
    }

    if (!openFile(file, parent)) return result;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Проверяем разделитель окончания данных
        if (line.isEmpty()) {
            emptyLineCounter++;
            if (emptyLineCounter >= STOP_LINES) {
                break; // Обнаружен конец данных
            }
            continue;
        } else {
            emptyLineCounter = 0; // Сбрасываем счетчик
        }

        ParsedRow row = parseLine(line);
        if (row.lastNonEmptyIndex >= 0) {
            result.rows.append(row);
            result.maxColumns = std::max(result.maxColumns, row.lastNonEmptyIndex + 1);
        }
    }

    QVector<QString> seriesHeaders;
    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if(line.startsWith("# Заголовки рядов")) {
            if(!in.atEnd()) {
                QString headersLine = in.readLine().trimmed();
                seriesHeaders = headersLine.split(", ", Qt::SkipEmptyParts);
            }
            break;
        }
    }
    result.seriesHeaders = seriesHeaders;

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

    MainWindow* mainWindow = qobject_cast<MainWindow*>(table->window());
    if(mainWindow && !result.seriesHeaders.isEmpty()) {
        mainWindow->setSeriesHeaders(result.seriesHeaders);
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void importFile(QTableWidget* table) {
    const QString filePath = getFilePath(table);
    if (filePath.isEmpty()) return;
    const auto parseResult = readAndParseFile(filePath, table);

    // Уже была показана ошибка, выходим
    if (parseResult.rows.isEmpty() && parseResult.seriesHeaders.isEmpty()) {
        return;
    }

    updateTable(table, parseResult);
}
}
