#include "export.h"

namespace Export
{
    TableMetrics calculateTableMetrics(QTableWidget *table)
    {
        TableMetrics metrics{0, true};

        for (int row = 0; row < table->rowCount(); ++row)
        {
            int lastNonEmptyCol = -1;
            for (int col = 0; col < table->columnCount(); ++col)
            {
                if (auto *item = table->item(row, col); item && !item->text().isEmpty())
                {
                    lastNonEmptyCol = col;
                    metrics.allEmpty = false;
                }
            }
            metrics.maxNonEmptyCols = qMax(metrics.maxNonEmptyCols, lastNonEmptyCol + 1);
        }
        return metrics;
    }

    QStringList prepareTableRows(QTableWidget *table, int columns)
    {
        QStringList rows;
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QStringList rowData;
            for (int col = 0; col < columns; ++col)
            {
                auto *item = table->item(row, col);
                rowData << (item && !item->text().isEmpty() ? item->text().trimmed() : "-");
            }
            if (!rowData.join("").remove("-").isEmpty())
                rows << rowData.join(" ");
        }
        return rows;
    }

    QStringList getHeaderLabels(QTableWidget *table, int columns)
    {
        QStringList headers;
        for (int col = 0; col < columns; ++col)
        {
            auto *header = table->horizontalHeaderItem(col);
            headers << (header && !header->text().isEmpty()
                            ? header->text()
                            : "Столбец " + QString::number(col + 1));
        }
        return headers;
    }

    bool writeFileContent(const QString &path, const QList<QPair<QString, QString>> &metrics,
                          const QStringList &headers, const QStringList &data)
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return false;

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);

        // Запись данных
        for (const auto &row : data)
            out << row << "\n";
        out << "\n\n\n";

        // Запись метрик
        for (const auto &[name, value] : metrics)
            out << name << ": " << value << "\n";

        // Запись заголовков
        out << "\n# Заголовки\n"
            << headers.join(" | ") << "\n";

        return true;
    }

    // Основная функция экспорта
    void exportData(QTableWidget *table, const QList<QPair<QString, QString>> &metrics)
    {
        if (!table)
        {
            QMessageBox::critical(nullptr, "Ошибка", "Таблица не инициализирована!");
            return;
        }

        // Анализ данных таблицы
        const auto [maxCols, allEmpty] = calculateTableMetrics(table);
        if (allEmpty || maxCols == 0)
        {
            QMessageBox::warning(nullptr, "Ошибка", "Нет данных для экспорта!");
            return;
        }

        // Подготовка данных
        const auto tableData = prepareTableRows(table, maxCols);
        if (tableData.isEmpty())
        {
            QMessageBox::warning(nullptr, "Ошибка", "Нет данных для экспорта!");
            return;
        }

        // Получение заголовков
        const auto headers = getHeaderLabels(table, maxCols);

        // Диалог сохранения
        const QString fileName = QFileDialog::getSaveFileName(
            nullptr, "Экспорт данных", "", "Текстовый файл (*.txt);;CSV (*.csv)");
        if (fileName.isEmpty())
            return;

        // Запись в файл
        if (writeFileContent(fileName, metrics, headers, tableData))
        {
            QMessageBox::information(nullptr, "Успех", "Данные экспортированы!");
        }
        else
        {
            QMessageBox::critical(nullptr, "Ошибка", "Ошибка записи файла!");
        }
    }
}
