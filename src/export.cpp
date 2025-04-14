// export.cpp
#include "export.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>

namespace Export {
void exportData(QTableWidget *table, const QStringList &metrics) {
    // 1. Подготовка данных таблицы
    QStringList tableRows;
    for (int row = 0; row < table->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            rowData << (item ? item->text().trimmed() : "");
        }
        tableRows << rowData.join(" "); // Разделитель - пробел
    }

    // 2. Диалог выбора файла
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        "Экспорт данных",
        "",
        "Текстовый файл (*.txt);;Файл CSV (*.csv)"
        );

    if (fileName.isEmpty()) return;

    // 3. Запись в файл
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось создать файл!");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Запись метрик
    out << "# Метрики\n";
    for (const QString &metric : metrics) {
        out << metric << "\n";
    }
    out << "\n";

    // Запись заголовков таблицы
    QStringList headers;
    for (int col = 0; col < table->columnCount(); ++col) {
        headers << table->horizontalHeaderItem(col)->text();
    }
    out << headers.join(" ") << "\n";

    // Запись данных
    out << tableRows.join("\n");
    file.close();

    QMessageBox::information(nullptr, "Успех", "Данные экспортированы!");
}
}
