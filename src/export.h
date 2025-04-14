#ifndef EXPORT_H
#define EXPORT_H

#include <QTableWidget>
#include <QStringList>

namespace Export {
    void exportData(QTableWidget *table, const QStringList &metrics);
}

#endif // EXPORT_H
