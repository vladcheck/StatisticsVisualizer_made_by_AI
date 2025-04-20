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

        // Проверка на пустую таблицу
        if (table->rowCount() == 0 || table->columnCount() == 0) {
            QMessageBox::warning(nullptr, "Ошибка", "Таблица не содержит данных!");
            return;
        }

        // Подготовка данных
        QStringList tableRows;
        for (int row = 0; row < table->rowCount(); ++row) {
            QStringList rowData;
            for (int col = 0; col < table->columnCount(); ++col) {
                QTableWidgetItem* item = table->item(row, col);
                // Заменяем пустые ячейки на тире
                QString cellText = (item && !item->text().isEmpty()) ? item->text().trimmed() : "-";
                rowData << cellText;
            }
            tableRows << rowData.join(" ");
        }

        // Диалог сохранения
        QString fileName = QFileDialog::getSaveFileName(
            nullptr,
            "Экспорт данных",
            "",
            "Текстовый файл (*.txt);;CSV (*.csv)"
            );
        if (fileName.isEmpty()) return;

        // Запись в файл
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setEncoding(QStringConverter::Utf8);

            // Метрики
            out << "# Метрики\n";
            for (const QString &metric : metrics) {
                out << metric << "\n";
            }

            // Заголовки (также заменяем пустые на тире)
            QStringList headers;
            for (int col = 0; col < table->columnCount(); ++col) {
                QTableWidgetItem* header = table->horizontalHeaderItem(col);
                QString headerText = (header && !header->text().isEmpty()) ? header->text() : "-";
                headers << headerText;
            }
            out << "\n" << headers.join(" ") << "\n";

            // Данные
            out << tableRows.join("\n");
            file.close();
            QMessageBox::information(nullptr, "Успех", "Данные экспортированы!");
        } else {
            QMessageBox::critical(nullptr, "Ошибка", "Ошибка записи файла!");
        }
    }
}
