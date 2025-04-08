#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>

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
};

#endif // HELPERFUNCTIONS_H
