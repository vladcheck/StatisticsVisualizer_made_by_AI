#include "file.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>

namespace File {
    // 1. Функция для получения пути к CSV-файлу
    QString getCsvFilePath(QWidget *parent) {
        return QFileDialog::getOpenFileName(
            parent,
            "Импорт CSV файла",
            "",
            "CSV файлы (*.csv);;Все файлы (*)"
            );
    }

    // 2. Функция для чтения данных из CSV-файла
    QStringList readCsvFile(const QString &filePath, QWidget *parent) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(parent, "Ошибка", "Не удалось открыть файл.");
            return {};
        }

        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.close();

        return lines;
    }

    // 3. Функция для парсинга CSV-данных
    QList<QStringList> parseCsvData(const QStringList &lines, const QRegularExpression &regex) {
        QList<QStringList> parsedData;
        for (const QString &line : lines) {
            parsedData.append(line.split(regex, Qt::SkipEmptyParts));
        }
        return parsedData;
    }

    // 4. Функция для обновления таблицы
    void fillTableWithData(QTableWidget *table, const QList<QStringList> &data) {
        table->clearContents();
        table->setRowCount(0);
        table->setColumnCount(0);

        if (data.isEmpty()) return;

        // Определение максимального количества столбцов
        int maxColumns = 0;
        for (const QStringList &row : data) {
            if (row.size() > maxColumns) maxColumns = row.size();
        }
        table->setColumnCount(maxColumns);

        // Заполнение данными
        for (int i = 0; i < data.size(); ++i) {
            table->insertRow(i);
            for (int j = 0; j < data[i].size(); ++j) {
                if (j >= table->columnCount()) break;
                QTableWidgetItem *item = new QTableWidgetItem(data[i][j].trimmed());
                table->setItem(i, j, item);
            }
        }
        table->resizeColumnsToContents();
    }

    // Основная функция
    void importCsvData(QTableWidget *table) {
        // Получение пути к файлу
        QString filePath = getCsvFilePath(table);
        if (filePath.isEmpty()) return;

        // Чтение файла
        QStringList lines = readCsvFile(filePath, table);
        if (lines.isEmpty()) return;

        // Парсинг данных
        QRegularExpression csvRegex("[;, \\t-]+");
        QList<QStringList> parsedData = parseCsvData(lines, csvRegex);

        // Обновление таблицы
        fillTableWithData(table, parsedData);
    }
}
