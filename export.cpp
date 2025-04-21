#include "export.h"

namespace Export
{
    QList<QVector<double>> collectRowsData(QTableWidget *table)
    {
        QList<QVector<double>> rowsData;
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QVector<double> rowData;
            for (int col = 0; col < table->columnCount(); ++col)
            {
                if (auto *item = table->item(row, col); item && !item->text().isEmpty())
                {
                    bool ok;
                    double value = item->text().toDouble(&ok);
                    if (ok) rowData.append(value);
                }
            }
            if (!rowData.isEmpty()) rowsData.append(rowData);
        }
        return rowsData;
    }

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

    QList<QPair<QString, std::function<QString(const QVector<double>&)>>> createMetricHandlers() {
        const int precision = 2; // Единый формат для всех числовых значений
        const QString na = "N/A"; // Обозначение для отсутствующих данных

        auto safeCall = [na](auto func, const auto& data, auto... args) -> QString {
            if(data.isEmpty()) return na;
            try {
                std::vector<double> vec(data.begin(), data.end());
                return QString::number(func(vec, args...), 'f', precision);
            } catch(...) {
                return na;
            }
        };

        return {
            {"Количество элементов", [na](const QVector<double>& data) {
                 return data.isEmpty() ? na : QString::number(data.size());
             }},
            {"Сумма", [=](const QVector<double>& data) {
                 return safeCall(Calculate::getSum, data);
             }},
            {"Среднее арифметическое", [=](const QVector<double>& data) {
                 return safeCall(Calculate::getMean, data);
             }},
            {"Геометрическое среднее", [=](const QVector<double>& data) {
                 return safeCall(Calculate::geometricMean, data);
             }},
            {"Гармоническое среднее", [=](const QVector<double>& data) {
                 return safeCall(Calculate::harmonicMean, data);
             }},
            {"Квадратичное среднее", [=](const QVector<double>& data) {
                 return safeCall(Calculate::rootMeanSquare, data);
             }},
            {"Усечённое среднее", [=](const QVector<double>& data) {
                 return safeCall(Calculate::trimmedMean, data, trimmedMeanPercentage);
             }},
            {"Медиана", [=](const QVector<double>& data) {
                 return safeCall(Calculate::getMedian, data);
             }},
            {"Мода", [=](const QVector<double>& data) {
                 return safeCall(Calculate::getMode, data);
             }},
            {"Стандартное отклонение", [=](const QVector<double>& data) {
                 std::vector<double> vec(data.begin(), data.end());
                 const double mean = Calculate::getMean(vec);
                 return safeCall(Calculate::getStandardDeviation, data, mean);
             }},
            {"Асимметрия", [=](const QVector<double>& data) {
                 std::vector<double> vec(data.begin(), data.end());
                 const double mean = Calculate::getMean(vec);
                 const double stdDev = Calculate::getStandardDeviation(vec, mean);
                 return safeCall(Calculate::skewness, data, mean, stdDev);
             }},
            {"Эксцесс", [=](const QVector<double>& data) {
                 std::vector<double> vec(data.begin(), data.end());
                 const double mean = Calculate::getMean(vec);
                 const double stdDev = Calculate::getStandardDeviation(vec, mean);
                 return safeCall(Calculate::kurtosis, data, mean, stdDev);
             }},
            {"Медианное абс. отклонение", [=](const QVector<double>& data) {
                 return safeCall(Calculate::medianAbsoluteDeviation, data);
             }},
            {"Робастное стан. отклонение", [=](const QVector<double>& data) {
                 return safeCall(Calculate::robustStandardDeviation, data);
             }},
            {"Тест Шапиро-Уилка", [=](const QVector<double>& data) {
                 return safeCall(Calculate::shapiroWilkTest, data);
             }},
            {"Плотность распределения", [=](const QVector<double>& data) {
                 if(data.isEmpty()) return na;
                 std::vector<double> vec(data.begin(), data.end());
                 const double mean = Calculate::getMean(vec);
                 return safeCall(Calculate::calculateDensity, data, mean);
             }},
            {"χ²-критерий", [=](const QVector<double>& data) {
                 return safeCall(Calculate::chiSquareTest, data);
             }},
            {"Критерий Колмогорова-Смирнова", [=](const QVector<double>& data) {
                 return safeCall(Calculate::kolmogorovSmirnovTest, data);
             }},
            {"Минимум", [=](const QVector<double>& data) {
                 if(data.isEmpty()) return na;
                 return QString::number(*std::min_element(data.begin(), data.end()), 'f', precision);
             }},
            {"Максимум", [=](const QVector<double>& data) {
                 if(data.isEmpty()) return na;
                 return QString::number(*std::max_element(data.begin(), data.end()), 'f', precision);
             }},
            {"Размах", [=](const QVector<double>& data) {
                 if(data.isEmpty()) return na;
                 const auto [min, max] = std::minmax_element(data.begin(), data.end());
                 return QString::number(*max - *min, 'f', precision);
             }}
        };
    }

    QList<QPair<QString, QString>> calculateAllMetrics(const QList<QVector<double>>& rowsData) {
        const auto handlers = createMetricHandlers();
        QList<QPair<QString, QString>> metrics;

        for (const auto& handler : handlers) {
            QStringList values;
            for (const auto& rowData : rowsData) {
                values << handler.second(rowData);
            }
            metrics.append({handler.first, values.join(", ")});
        }
        return metrics;
    }

    bool processExportDialog(const QString& fileName,
                             const QList<QPair<QString, QString>>& metrics,
                             const QStringList& headers,
                             const QStringList& tableData) {
        if (fileName.isEmpty()) return false;
        return writeFileContent(fileName, metrics, headers, tableData);
    }

    void exportData(QTableWidget* table, const QList<QPair<QString, QString>>& /*metrics*/) {
        if (!table) {
            QMessageBox::critical(nullptr, "Ошибка", "Таблица не инициализирована!");
            return;
        }

        const auto rowsData = collectRowsData(table);
        if (rowsData.isEmpty()) {
            QMessageBox::warning(nullptr, "Ошибка", "Нет данных для экспорта!");
            return;
        }

        const auto metrics = calculateAllMetrics(rowsData);
        const TableMetrics tableMetrics = calculateTableMetrics(table);
        const auto headers = getHeaderLabels(table, tableMetrics.maxNonEmptyCols);
        const auto tableData = prepareTableRows(table, tableMetrics.maxNonEmptyCols);

        const QString fileName = QFileDialog::getSaveFileName(
            nullptr, "Экспорт данных", "", "Текстовый файл (*.txt);;CSV (*.csv)");

        if (processExportDialog(fileName, metrics, headers, tableData)) {
            QMessageBox::information(nullptr, "Успех", "Данные экспортированы!");
        } else if (!fileName.isEmpty()) {
            QMessageBox::critical(nullptr, "Ошибка", "Ошибка записи файла!");
        }
    }
}
