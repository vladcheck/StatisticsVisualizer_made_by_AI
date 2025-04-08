#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>

namespace Helper {
    void setSizePolicyExpanding(QWidget *w) {
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void setSizePolicyFixed(QWidget *w) {
        w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    QSpinBox *createSpinBox(QWidget *parent, int max, int min = 1) {
        QSpinBox *spinBox = new QSpinBox(parent);
        spinBox->setMinimum(min);
        spinBox->setMaximum(max);
        return spinBox;
    }

    QHBoxLayout *createSpinBoxWithLabel(QWidget *parent, const std::string text, int max, int min = 1) {
        auto *container = new QHBoxLayout();
        auto* spinBox = createSpinBox(parent,max,min);
        auto* label = new QLabel(QString::fromStdString(text), parent);

        container->addWidget(label);
        container->addWidget(spinBox);

        return container;
    }

    QPushButton* createToolButton(const QString& tooltip, const QString& iconName) {
        QPushButton *btn = new QPushButton();

        // Загрузка изображения через QPixmap
        QPixmap pixmap(":/icons/" + iconName + ".png");
        btn->setIcon(QIcon(pixmap));
        btn->setIconSize(QSize(28, 28));  // Размер иконки

        // Настройка внешнего вида
        btn->setToolTip(tooltip);
        btn->setFixedSize(32, 32);        // Размер кнопки

        return btn;
    }

    // Вертикальный разделитель
    QWidget* createSeparator() {
        QFrame *line = new QFrame();
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        return line;
    }

    // Кнопка очистки с иконкой
    QPushButton* createClearButton(QWidget* parent) {
        QPushButton *btn = new QPushButton("Очистить", parent);
        btn->setIcon(QIcon(":/icons/clear.png"));
        btn->setToolTip("Очистить всё содержимое таблицы");
        return btn;
    }
};

#endif // HELPERFUNCTIONS_H
