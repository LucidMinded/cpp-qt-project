#include "controls.h"

#include "mainwindow.h"

ControlsWidget::ControlsWidget(QWidget *parent, MainWindow *mainWindow)
    : QWidget(parent)
{
    QGroupBox *funcGroup = new QGroupBox("Function");
    QVBoxLayout *funcLayout = new QVBoxLayout(funcGroup);
    funcLayout->addWidget(sincDist = new QRadioButton("sinc(distance_from_zero)"));
    funcLayout->addWidget(sincSinc = new QRadioButton("sinc(x) * sinc(z)"));

    QGroupBox *selGroup = new QGroupBox("Selection");
    QVBoxLayout *selLayout = new QVBoxLayout(selGroup);
    selLayout->addWidget(itemSelection = new QRadioButton("Item selection"));
    selLayout->addWidget(noSelection = new QRadioButton("No selection"));

    QGroupBox *gridLabelGroup = new QGroupBox();
    QVBoxLayout *gridLabelLayout = new QVBoxLayout(gridLabelGroup);
    gridLabelLayout->addWidget(showGrid = new QCheckBox("Show grid"));
    gridLabelLayout->addWidget(showLabel = new QCheckBox("Show label"));
    gridLabelLayout->addWidget(showLabelBorder = new QCheckBox("Show label border"));

    QVBoxLayout *xRangeLayout = new QVBoxLayout;
    xRangeLayout->addWidget(xRangeFrom = new QSlider(Qt::Horizontal));
    xRangeLayout->addWidget(xRangeTo = new QSlider(Qt::Horizontal));

    QVBoxLayout *zRangeLayout = new QVBoxLayout;
    zRangeLayout->addWidget(zRangeFrom = new QSlider(Qt::Horizontal));
    zRangeLayout->addWidget(zRangeTo = new QSlider(Qt::Horizontal));

    QVBoxLayout *stepsLayout = new QVBoxLayout;
    stepsLayout->addWidget(xSteps = new QSlider(Qt::Horizontal));
    stepsLayout->addWidget(zSteps = new QSlider(Qt::Horizontal));

    xRangeFrom->setMinimum(-10);
    xRangeFrom->setMaximum(9);
    xRangeFrom->setTickInterval(1);
    xRangeFrom->setTickPosition(QSlider::TicksBelow);

    xRangeTo->setMinimum(-9);
    xRangeTo->setMaximum(10);
    xRangeTo->setTickInterval(1);
    xRangeTo->setTickPosition(QSlider::TicksBelow);

    zRangeFrom->setMinimum(-10);
    zRangeFrom->setMaximum(9);
    zRangeFrom->setTickInterval(1);
    zRangeFrom->setTickPosition(QSlider::TicksBelow);

    zRangeTo->setMinimum(-9);
    zRangeTo->setMaximum(10);
    zRangeTo->setTickInterval(1);
    zRangeTo->setTickPosition(QSlider::TicksBelow);

    xSteps->setMinimum(10);
    xSteps->setMaximum(100);
    xSteps->setTickInterval(10);
    xSteps->setTickPosition(QSlider::TicksBelow);

    zSteps->setMinimum(10);
    zSteps->setMaximum(100);
    zSteps->setTickInterval(10);
    zSteps->setTickPosition(QSlider::TicksBelow);

    QPixmap pixmap(40, 100);
    QPainter painter(&pixmap);

    gradient1 = new QLinearGradient(0, 0, 1, 100);
    gradient1->setColorAt(0, Qt::darkGreen);
    gradient1->setColorAt(0.3, Qt::yellow);
    gradient1->setColorAt(0.6, Qt::darkYellow);
    gradient1->setColorAt(1, Qt::darkRed);
    painter.setBrush(*gradient1);
    painter.drawRect(0, 0, 40, 100);
    gradient1Button = new QPushButton;
    gradient1Button->setIcon(pixmap);
    gradient1Button->setIconSize(pixmap.size());

    gradient2 = new QLinearGradient(0, 0, 1, 100);
    gradient2->setColorAt(0, Qt::darkGreen);
    gradient2->setColorAt(0.3, Qt::darkCyan);
    gradient2->setColorAt(0.6, Qt::blue);
    gradient2->setColorAt(1, Qt::darkMagenta);
    painter.setBrush(*gradient2);
    painter.drawRect(0, 0, 40, 100);
    gradient2Button = new QPushButton;
    gradient2Button->setIcon(pixmap);
    gradient2Button->setIconSize(pixmap.size());

    QGroupBox *gradientGroup = new QGroupBox(tr("Gradient"));
    QHBoxLayout *gradientLayout = new QHBoxLayout(gradientGroup);
    gradientLayout->addWidget(gradient1Button);
    gradientLayout->addWidget(gradient2Button);

    QVBoxLayout *controlsLayout = new QVBoxLayout(this);
    controlsLayout->setAlignment(Qt::AlignTop);
    controlsLayout->addWidget(funcGroup);
    controlsLayout->addWidget(selGroup);
    controlsLayout->addWidget(gridLabelGroup);
    controlsLayout->addWidget(new QLabel("X range"));
    controlsLayout->addLayout(xRangeLayout);
    controlsLayout->addWidget(new QLabel("Z range"));
    controlsLayout->addLayout(zRangeLayout);
    controlsLayout->addWidget(new QLabel("Steps"));
    controlsLayout->addLayout(stepsLayout);
    controlsLayout->addWidget(gradientGroup);

    connect(sincDist, SIGNAL(clicked()), mainWindow, SIGNAL(sincDistSelectedSignal()));
    connect(sincSinc, SIGNAL(clicked()), mainWindow, SIGNAL(sincSincSelectedSignal()));
    connect(itemSelection, SIGNAL(clicked()), mainWindow, SIGNAL(itemSelectionSelectedSignal()));
    connect(noSelection, SIGNAL(clicked()), mainWindow, SIGNAL(noSelectionSelectedSignal()));
    connect(showGrid, SIGNAL(clicked(bool)), mainWindow, SIGNAL(showGridChangedSignal(bool)));
    connect(showLabel, SIGNAL(clicked(bool)), mainWindow, SIGNAL(showLabelChangedSignal(bool)));
    connect(showLabelBorder,
            SIGNAL(clicked(bool)),
            mainWindow,
            SIGNAL(showLabelBorderChangedSignal(bool)));
    connect(xRangeFrom, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(xRangeFromChangedSignal(int)));
    connect(xRangeTo, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(xRangeToChangedSignal(int)));
    connect(zRangeFrom, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(zRangeFromChangedSignal(int)));
    connect(zRangeTo, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(zRangeToChangedSignal(int)));
    connect(xSteps, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(xStepsChangedSignal(int)));
    connect(zSteps, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(zStepsChangedSignal(int)));
    connect(gradient1Button, SIGNAL(clicked()), mainWindow, SIGNAL(gradient1ButtonClickedSignal()));
    connect(gradient2Button, SIGNAL(clicked()), mainWindow, SIGNAL(gradient2ButtonClickedSignal()));

    connect(mainWindow, SIGNAL(sincDistSelectedSignal()), SLOT(sincDistSelectedSlot()));
    connect(mainWindow, SIGNAL(sincSincSelectedSignal()), SLOT(sincSincSelectedSlot()));
    connect(mainWindow, SIGNAL(itemSelectionSelectedSignal()), SLOT(itemSelectionSelectedSlot()));
    connect(mainWindow, SIGNAL(noSelectionSelectedSignal()), SLOT(noSelectionSelectedSlot()));
    connect(mainWindow, SIGNAL(showGridChangedSignal(bool)), SLOT(showGridChangedSlot(bool)));
    connect(mainWindow, SIGNAL(showLabelChangedSignal(bool)), SLOT(showLabelChangedSlot(bool)));
    connect(mainWindow,
            SIGNAL(showLabelBorderChangedSignal(bool)),
            SLOT(showLabelBorderChangedSlot(bool)));
    connect(mainWindow, SIGNAL(xRangeFromChangedSignal(int)), SLOT(xRangeFromChangedSlot(int)));
    connect(mainWindow, SIGNAL(xRangeToChangedSignal(int)), SLOT(xRangeToChangedSlot(int)));
    connect(mainWindow, SIGNAL(zRangeFromChangedSignal(int)), SLOT(zRangeFromChangedSlot(int)));
    connect(mainWindow, SIGNAL(zRangeToChangedSignal(int)), SLOT(zRangeToChangedSlot(int)));
    connect(mainWindow, SIGNAL(xStepsChangedSignal(int)), SLOT(xStepsChangedSlot(int)));
    connect(mainWindow, SIGNAL(zStepsChangedSignal(int)), SLOT(zStepsChangedSlot(int)));
    connect(mainWindow, SIGNAL(gradient1ButtonClickedSignal()), SLOT(gradient1ButtonClickedSlot()));
    connect(mainWindow, SIGNAL(gradient2ButtonClickedSignal()), SLOT(gradient2ButtonClickedSlot()));
}

void ControlsWidget::sincDistSelectedSlot()
{
    sincDist->setChecked(true);
}

void ControlsWidget::sincSincSelectedSlot()
{
    sincSinc->setChecked(true);
}

void ControlsWidget::itemSelectionSelectedSlot()
{
    itemSelection->setChecked(true);
}

void ControlsWidget::noSelectionSelectedSlot()
{
    noSelection->setChecked(true);
}

void ControlsWidget::showGridChangedSlot(bool enabled)
{
    showGrid->setChecked(enabled);
}

void ControlsWidget::showLabelChangedSlot(bool enabled)
{
    showLabel->setChecked(enabled);
    showLabelBorder->setEnabled(enabled);
}

void ControlsWidget::showLabelBorderChangedSlot(bool enabled)
{
    showLabelBorder->setChecked(enabled);
}

void ControlsWidget::xRangeFromChangedSlot(int value)
{
    xRangeFrom->setValue(value);
    xRangeTo->setValue(qMax(xRangeTo->value(), value + 1));
}

void ControlsWidget::xRangeToChangedSlot(int value)
{
    xRangeTo->setValue(value);
    xRangeFrom->setValue(qMin(xRangeFrom->value(), value - 1));
}

void ControlsWidget::zRangeFromChangedSlot(int value)
{
    zRangeFrom->setValue(value);
    zRangeTo->setValue(qMax(zRangeTo->value(), value + 1));
}

void ControlsWidget::zRangeToChangedSlot(int value)
{
    zRangeTo->setValue(value);
    zRangeFrom->setValue(qMin(zRangeFrom->value(), value - 1));
}

void ControlsWidget::xStepsChangedSlot(int value)
{
    xSteps->setValue(value);
}

void ControlsWidget::zStepsChangedSlot(int value)
{
    zSteps->setValue(value);
}

void ControlsWidget::gradient1ButtonClickedSlot()
{
    gradient1Button->setChecked(true);
}

void ControlsWidget::gradient2ButtonClickedSlot()
{
    gradient2Button->setChecked(true);
}
