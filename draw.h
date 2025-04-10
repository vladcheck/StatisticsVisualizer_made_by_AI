#ifndef DRAW_H
#define DRAW_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QTableWidget>

const unsigned int buttonSize = 36;
const unsigned int buttonIconSize = static_cast<int>(buttonSize * 0.7);

struct TableActions
{
    QPushButton *addRowBtn;
    QPushButton *addColBtn;
    QPushButton *delRowBtn;
    QPushButton *delColBtn;
    QPushButton *clearButton;
    QPushButton *autoSizeBtn;
    QTableWidget *table;
    QSpinBox *rowSpin;
    QSpinBox *colSpin;
};

namespace Draw
{
    void setSizePolicyExpanding(QWidget *w)
    {
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void setSizePolicyFixed(QWidget *w)
    {
        w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

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

    // Улучшенный разделитель (horizontal = false для вертикального)
    QWidget *createSeparator(bool horizontal = true)
    {
        QFrame *line = new QFrame();
        line->setFrameShape(horizontal ? QFrame::HLine : QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("color: #ddd;");
        line->setFixedHeight(horizontal ? 1 : 20);
        return line;
    }

    QSpinBox *createSpinBox(QWidget *parent, int max, int value = 1, int min = 1)
    {
        QSpinBox *spinBox = new QSpinBox(parent);
        spinBox->setMinimum(min);
        spinBox->setMaximum(max);
        spinBox->setValue(value);
        return spinBox;
    }

    QHBoxLayout *createSpinBoxWithLabel(QWidget *parent, const std::string text, int max, int min = 1)
    {
        auto *container = new QHBoxLayout();
        auto *spinBox = createSpinBox(parent, max, min);
        auto *label = new QLabel(QString::fromStdString(text), parent);

        container->addWidget(label);
        container->addWidget(spinBox);

        return container;
    }

    QPushButton *createToolButton(const QString &tooltip, const QString &iconName)
    {
        QPushButton *btn = new QPushButton();

        // Загрузка изображения через QPixmap
        QPixmap pixmap(":/" + iconName + ".png");
        btn->setIcon(QIcon(pixmap));
        btn->setIconSize(QSize(buttonIconSize, buttonIconSize)); // Размер иконки

        // Настройка внешнего вида
        btn->setToolTip(tooltip);
        btn->setFixedSize(buttonSize, buttonSize); // Размер кнопки

        return btn;
    }

    QWidget *createStatRow(QWidget *parent,
                                  const QString &title,
                                  const QString &value = "—",
                                  const QString &objectName = "")
    {
        QWidget *row = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout(row);
        layout->setContentsMargins(0, 0, 0, 0);

        QLabel *titleLabel = new QLabel(title + ":", row);
        QLabel *valueLabel = new QLabel(value, row);
        valueLabel->setObjectName(objectName.isEmpty() ? title + "Value" : objectName);

        layout->addWidget(titleLabel);
        layout->addWidget(valueLabel, 1, Qt::AlignRight);
        return row;
    }

    QLabel *createAndRegisterStatRow(QWidget *parent, QLayout *layout,
                                     const QString &title, const QString &defaultValue, const QString &objectName)
    {
        QWidget *row = createStatRow(parent, title, defaultValue, objectName);
        QLabel *label = row->findChild<QLabel *>(objectName);
        layout->addWidget(row);
        return label;
    }

    // Создание секции с заголовком
    QWidget *createStatSection(QWidget *parent, const QString &title)
    {
        QWidget *section = new QWidget(parent);
        QVBoxLayout *layout = new QVBoxLayout(section); // Создаем layout сразу

        QLabel *header = new QLabel(title, section);
        header->setStyleSheet("font-weight: 600; font-size: 20px; color: #ddd;");

        layout->addWidget(header);
        layout->addWidget(createSeparator(true));

        return section; // Виджет УЖЕ имеет layout
    }

    static void addStatRows(QWidget *parent, QLayout *layout, const std::initializer_list<QPair<QString, QString>> &rows)
    {
        for (const auto &[title, value] : rows)
        {
            layout->addWidget(createStatRow(parent, title, value));
        }
    }

    void updateStatValue(QWidget *section, const QString &title, const QString &value)
    {
        QList<QLabel *> labels = section->findChildren<QLabel *>();
        for (QLabel *label : labels)
        {
            if (label->text() == title)
            {
                QWidget *row = label->parentWidget();
                QLabel *valueLabel = qobject_cast<QLabel *>(row->layout()->itemAt(1)->widget());
                valueLabel->setText(value);
                break;
            }
        }
    }
};

#endif // DRAW_H
