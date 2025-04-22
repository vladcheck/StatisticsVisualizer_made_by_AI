#ifndef NUMERICDELEGATE_H
#define NUMERICDELEGATE_H

#include <QTableWidget>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>

class NumericDelegate : public QStyledItemDelegate {
public:
    NumericDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex&) const override {
        QLineEdit* editor = new QLineEdit(parent);
        QDoubleValidator* validator = new QDoubleValidator(editor);
        validator->setLocale(QLocale::C);
        editor->setValidator(validator);
        return editor;
    }
};

#endif // NUMERICDELEGATE_H
