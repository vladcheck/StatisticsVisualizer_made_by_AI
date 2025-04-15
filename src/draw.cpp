#include "draw.h"

namespace Draw {
    QRegularExpression csvRegex("[;, \\t-]+");

    void setSizePolicyExpanding(QWidget *w)
    {
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void setSizePolicyFixed(QWidget *w)
    {
        w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    QWidget *setupHeader(QWidget *parent, const int fontSize)
    {
        const QFont headerFont(fontName, fontSize);

        QWidget *header = new QWidget(parent);
        header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QLabel *h1 = new QLabel("Statistics Visualizer", header);
        h1->setFont(headerFont);

        QHBoxLayout *headerAlignment = new QHBoxLayout(header);
        headerAlignment->setContentsMargins(0, 0, 0, 0);
        headerAlignment->addWidget(h1, 0, Qt::AlignCenter);
        return header;
    }

    QTableWidget *setupTable(QWidget *parent) {
        // Правая часть - таблица
        QTableWidget *table = new QTableWidget(initialRowCount, initialColCount, parent);
        Draw::setSizePolicyExpanding(table);
        table->verticalHeader()->setVisible(false);
        table->resizeColumnsToContents();
        return table;
    }

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

    void addStatRows(QWidget *parent, QLayout *layout, const std::initializer_list<QPair<QString, QString>> &rows)
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

    QGroupBox* createChartSettingsPanel(QWidget* parent) {
        QGroupBox* settingsGroup = new QGroupBox("Настройки визуализации", parent);
        QVBoxLayout* settingsLayout = new QVBoxLayout(settingsGroup);
        settingsLayout->setAlignment(Qt::AlignTop);

        // Заглушка для элементов управления
        QLabel* settingsPlaceholder = new QLabel("Параметры будут здесь");
        settingsLayout->addWidget(settingsPlaceholder);

        settingsGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        return settingsGroup;
    }

    QValueAxis* setupAxis(QString name, int a = 0, int b = 10) {
        QValueAxis* axis = new QValueAxis();
        axis->setTitleText(name);
        axis->setRange(a,b);
        return axis;
    }

    QScatterSeries* setupScatterSeries(float size = 12.0, QColor pointColor = Qt::blue, QColor borderColor = Qt::white) {
        QScatterSeries* scatterSeries = new QScatterSeries();
        scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        scatterSeries->setMarkerSize(size);
        scatterSeries->setColor(pointColor);
        scatterSeries->setBorderColor(borderColor);
        return scatterSeries;
    }

    QWidget* createChartWidget(QWidget* parent = nullptr) {
        QWidget* chartContainer = new QWidget(parent);
        QVBoxLayout* chartLayout = new QVBoxLayout(chartContainer);
        chartLayout->setContentsMargins(0, 0, 0, 0);

        // Создаем два типа серий: линии и точки
        QLineSeries* lineSeries = new QLineSeries();    // Для соединения точек линиями
        auto* scatterSeries = setupScatterSeries();     // Для отображения точек

        // Настройка линий
        lineSeries->setColor(Qt::blue);
        QPen pen = lineSeries->pen();
        pen.setWidth(2);
        lineSeries->setPen(pen);

        // Здесь можно добавить данные через сигналы/слоты
        // Пока оставляем пустым для демонстрации

        QChart* chart = new QChart();
        chart->addSeries(lineSeries);
        chart->addSeries(scatterSeries);
        chart->setTitle("Точечный график");
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setBackgroundBrush(Qt::white);
        chart->legend()->setVisible(false);

        // Настройка осей
        QValueAxis* axisX = setupAxis("Ось X",0,10);
        QValueAxis* axisY = setupAxis("Ось Y",0,10);

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        // Привязываем оси к обеим сериям
        lineSeries->attachAxis(axisX);
        lineSeries->attachAxis(axisY);
        scatterSeries->attachAxis(axisX);
        scatterSeries->attachAxis(axisY);

        QChartView* chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        chartLayout->addWidget(chartView);
        setSizePolicyExpanding(chartContainer);
        return chartContainer;
    }

    QSplitter* addSplitter(QWidget* parent, QWidget* w1, QWidget* w2, int stretch1 = 1, int stretch2 = 1) {
        QSplitter* splitter = new QSplitter(Qt::Horizontal, parent);
        splitter->setHandleWidth(10);
        splitter->setChildrenCollapsible(false);

        // Настройка политик размеров
        w1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        w1->setMinimumWidth(400);
        setSizePolicyExpanding(w2);

        splitter->addWidget(w1);
        splitter->addWidget(w2);

        // Правильное использование stretch factors
        splitter->setStretchFactor(0, stretch1);
        splitter->setStretchFactor(1, stretch2);

        // Автоматическое начальное распределение
        QTimer::singleShot(0, [=](){
            int total = splitter->width() - splitter->handleWidth();
            QList<int> sizes;
            sizes << total * stretch1 / (stretch1 + stretch2)
                  << total * stretch2 / (stretch1 + stretch2);
            splitter->setSizes(sizes);
        });

        return splitter;
    }

    QWidget* setupGraphSection(QWidget* parent) {
        QWidget* widget = new QWidget(parent);
        QHBoxLayout* mainLayout = new QHBoxLayout(widget);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        auto* chartSettings = createChartSettingsPanel(widget);
        auto* chartWidget = createChartWidget(widget);

        QSplitter* splitter = Draw::addSplitter(widget, chartSettings, chartWidget, 1, 2);
        mainLayout->addWidget(splitter);

        return widget;
    }
}
