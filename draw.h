#ifndef DRAW_H
#define DRAW_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QTableWidget>

namespace Draw
{
    // Для QAction
    template <typename Func>
    static void connect(QAction *action, Func &&callback)
    {
        QObject::connect(action, &QAction::triggered,
                         action, std::forward<Func>(callback));
    }
    // Для QSpinBox
    template <typename Func>
    static void connect(QSpinBox *spinBox, Func &&callback)
    {
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                         spinBox, std::forward<Func>(callback));
    }
    // Для QPushButton
    template <typename Func>
    static void connect(QPushButton *button, Func &&callback)
    {
        QObject::connect(button, &QPushButton::clicked,
                         button, std::forward<Func>(callback));
    }
    void setSizePolicyExpanding(QWidget *w);
    void setSizePolicyFixed(QWidget *w);
    QWidget *createSeparator(bool horizontal);
    QSpinBox *createSpinBox(QWidget *parent, int max, int value, int min);
    QHBoxLayout *createSpinBoxWithLabel(QWidget *parent, const std::string text, int max, int min);
    QPushButton *createToolButton(const QString &tooltip, const QString &iconName);
    QWidget *createStatRow(QWidget *parent, const QString &title, const QString &value, const QString &objectName);
    QLabel *createAndRegisterStatRow(QWidget *parent, QLayout *layout, const QString &title, const QString &defaultValue, const QString &objectName);
    QWidget *createStatSection(QWidget *parent, const QString &title);     // Создание секции с заголовком
    void addStatRows(QWidget *parent, QLayout *layout, const std::initializer_list<QPair<QString, QString>> &rows);
    void updateStatValue(QWidget *section, const QString &title, const QString &value);
};

#endif // DRAW_H
