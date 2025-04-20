// export.cpp
#include "export.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>

namespace Export {
void exportData(QTableWidget *table, const QStringList &metrics) {
    if (!table) {
        QMessageBox::critical(nullptr, "Ошибка", "Таблица не инициализирована!");
        return;
    }

    // 1. Определение минимального количества значимых столбцов
    int maxNonEmptyCols = 0;
    bool allEmpty = true;

    for (int row = 0; row < table->rowCount(); ++row) {
        int lastNonEmptyCol = -1;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem* item = table->item(row, col);
            if (item && !item->text().isEmpty()) {
                lastNonEmptyCol = col;
                allEmpty = false;
            }
        }
        if (lastNonEmptyCol + 1 > maxNonEmptyCols) {
            maxNonEmptyCols = lastNonEmptyCol + 1;
        }
    }

    if (allEmpty || maxNonEmptyCols == 0) {
        QMessageBox::warning(nullptr, "Ошибка", "Нет данных для экспорта!");
        return;
    }

    // 2. Подготовка данных таблицы
    QStringList tableRows;
    for (int row = 0; row < table->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < maxNonEmptyCols; ++col) {
            QTableWidgetItem* item = table->item(row, col);
            QString cellText = (item && !item->text().isEmpty()) ?
                                   item->text().trimmed() : "-";
            rowData << cellText;
        }
        // Проверка на полностью пустую строку
        if (rowData.join("").remove("-").isEmpty()) continue;
        tableRows << rowData.join(" ");
    }

    // 3. Диалог сохранения
    QString fileName = QFileDialog::getSaveFileName(
        nullptr, "Экспорт данных", "", "Текстовый файл (*.txt);;CSV (*.csv)");
    if (fileName.isEmpty()) return;

    // 4. Запись в файл
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);

        // Запись метрик с названиями
        QStringList metricLabels = {
            "Количество элементов", "Сумма", "Среднее",
            "Геометрическое среднее", "Гармоническое среднее",
            "Медиана", "Мода", "Стандартное отклонение"
        };

        for (int i = 0; i < metrics.size(); ++i) {
            if (i < metricLabels.size()) {
                out << metricLabels[i] << ": " << metrics[i] << "\n";
            } else {
                out << "Метрика " << i+1 << ": " << metrics[i] << "\n";
            }
        }

        // Заголовки
        QStringList headers;
        for (int col = 0; col < maxNonEmptyCols; ++col) {
            QTableWidgetItem* header = table->horizontalHeaderItem(col);
            QString headerText = (header && !header->text().isEmpty()) ?
                                     header->text() : "Столбец " + QString::number(col+1);
            headers << headerText;
        }
        out << "\n# Заголовки\n" << headers.join(" | ") << "\n";

        // Данные
        file.close();
        QMessageBox::information(nullptr, "Успех", "Данные экспортированы!");
    } else {
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка записи файла!");
    }
}
}
