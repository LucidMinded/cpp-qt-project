#include "mainwindow.h"

#include <QtWidgets>

using namespace QtDataVisualization;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1200, 800);

    m_graph = new Q3DSurface;
    QWidget *graphContainer = QWidget::createWindowContainer(m_graph);
    setCentralWidget(graphContainer);

    plot = new Plot(this);
    m_sincDistSeries = new QSurface3DSeries(plot->getSincDistanceProxy());
    m_sincSincSeries = new QSurface3DSeries(plot->getSincSincProxy());

    QDockWidget *controlsDockWidget = new QDockWidget;
    addDockWidget(Qt::RightDockWidgetArea, controlsDockWidget);
    controlsDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures); // disable moving
    controlsDockWidget->setTitleBarWidget(new QWidget); // a crutch to hide title bar from dockWidget
    // https://doc.qt.io/qt-6/qdockwidget.html#setTitleBarWidget

    m_controls = new ControlsWidget(nullptr, this);
    controlsDockWidget->setWidget(m_controls);

    statusBar();

    save = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), this);
    save->setShortcut(QKeySequence::Save);
    save->setStatusTip(tr("Save settings to file"));
    connect(save, &QAction::triggered, this, &MainWindow::saveSettingsSlot);

    revert = new QAction(style()->standardIcon(QStyle::SP_BrowserReload), tr("&Revert"), this);
    revert->setShortcut(QKeySequence::Refresh);
    revert->setStatusTip(tr("Revert settings to last saved"));
    connect(revert, &QAction::triggered, this, &MainWindow::revertSettingsSlot);

    QMenu *settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(save);
    settingsMenu->addAction(revert);

    QToolBar *toolBar = addToolBar(tr("Toolbar"));
    toolBar->addAction(save);
    toolBar->addAction(revert);

    connect(this, SIGNAL(sincDistSelectedSignal()), SLOT(sincDistSelectedSlot()));
    connect(this, SIGNAL(sincSincSelectedSignal()), SLOT(sincSincSelectedSlot()));
    connect(this, SIGNAL(itemSelectionSelectedSignal()), SLOT(itemSelectionSelectedSlot()));
    connect(this, SIGNAL(noSelectionSelectedSignal()), SLOT(noSelectionSelectedSlot()));
    connect(this, SIGNAL(showGridChangedSignal(bool)), SLOT(showGridChangedSlot(bool)));
    connect(this, SIGNAL(showLabelChangedSignal(bool)), SLOT(showLabelChangedSlot(bool)));
    connect(this,
            SIGNAL(showLabelBorderChangedSignal(bool)),
            SLOT(showLabelBorderChangedSlot(bool)));
    connect(this, SIGNAL(xRangeFromChangedSignal(int)), SLOT(xRangeFromChangedSlot(int)));
    connect(this, SIGNAL(xRangeToChangedSignal(int)), SLOT(xRangeToChangedSlot(int)));
    connect(this, SIGNAL(zRangeFromChangedSignal(int)), SLOT(zRangeFromChangedSlot(int)));
    connect(this, SIGNAL(zRangeToChangedSignal(int)), SLOT(zRangeToChangedSlot(int)));
    connect(this, SIGNAL(xStepsChangedSignal(int)), SLOT(xStepsChangedSlot(int)));
    connect(this, SIGNAL(zStepsChangedSignal(int)), SLOT(zStepsChangedSlot(int)));
    connect(this, SIGNAL(revertSettingsSignal()), SLOT(revertSettingsSlot()));
    connect(this, SIGNAL(saveSettingsSignal()), SLOT(saveSettingsSlot()));
    connect(this, SIGNAL(gradient1ButtonClickedSignal()), SLOT(gradient1ButtonClickedSlot()));
    connect(this, SIGNAL(gradient2ButtonClickedSignal()), SLOT(gradient2ButtonClickedSlot()));

    connect(this, SIGNAL(xStepsChangedSignal(int)), plot, SLOT(xStepsChangedSlot(int)));
    connect(this, SIGNAL(zStepsChangedSignal(int)), plot, SLOT(zStepsChangedSlot(int)));

    m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
    connect(this, SIGNAL(revertSettingsSignal()), SLOT(revertSettingsSlot()));
    connect(this, SIGNAL(saveSettingsSignal()), SLOT(saveSettingsSlot()));
    emit revertSettingsSignal();
}

MainWindow::~MainWindow() {}

void MainWindow::emitSettingsChangedSignals()
{
    GraphSettings settings = m_sincDistSelected ? m_settingsSincDist : m_settingsSincSinc;
    if (settings.itemSelectionSelected) {
        emit itemSelectionSelectedSignal();
    } else {
        emit noSelectionSelectedSignal();
    }
    emit showGridChangedSignal(settings.showGrid);
    emit showLabelChangedSignal(settings.showLabel);
    emit showLabelBorderChangedSignal(settings.showLabelBorder);
    emit xRangeFromChangedSignal(settings.xRangeFrom);
    emit xRangeToChangedSignal(settings.xRangeTo);
    emit zRangeFromChangedSignal(settings.zRangeFrom);
    emit zRangeToChangedSignal(settings.zRangeTo);
    emit xStepsChangedSignal(settings.xSteps);
    emit zStepsChangedSignal(settings.zSteps);
    if (settings.gradient1Selected) {
        emit gradient1ButtonClickedSignal();
    } else {
        emit gradient2ButtonClickedSignal();
    }
}

void MainWindow::sincDistSelectedSlot()
{
    m_sincDistSelected = true;
    m_graph->removeSeries(m_sincSincSeries);
    m_graph->addSeries(m_sincDistSeries);

    m_graph->axisX()->setRange(m_settingsSincDist.xRangeFrom, m_settingsSincDist.xRangeTo);
    m_graph->axisZ()->setRange(m_settingsSincDist.zRangeFrom, m_settingsSincDist.zRangeTo);

    emitSettingsChangedSignals();
}

void MainWindow::sincSincSelectedSlot()
{
    m_sincDistSelected = false;
    m_graph->removeSeries(m_sincDistSeries);
    m_graph->addSeries(m_sincSincSeries);

    m_graph->axisX()->setRange(m_settingsSincSinc.xRangeFrom, m_settingsSincSinc.xRangeTo);
    m_graph->axisZ()->setRange(m_settingsSincSinc.zRangeFrom, m_settingsSincSinc.zRangeTo);

    emitSettingsChangedSignals();
}

void MainWindow::itemSelectionHelper(bool enabled)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.itemSelectionSelected = enabled;
    } else {
        m_settingsSincSinc.itemSelectionSelected = enabled;
    }
    m_graph->setSelectionMode(enabled ? Q3DSurface::SelectionItem : Q3DSurface::SelectionNone);
}

void MainWindow::itemSelectionSelectedSlot()
{
    itemSelectionHelper(true);
}

void MainWindow::noSelectionSelectedSlot()
{
    itemSelectionHelper(false);
}

void MainWindow::showGridChangedSlot(bool show)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.showGrid = show;
    } else {
        m_settingsSincSinc.showGrid = show;
    }
    m_graph->activeTheme()->setGridEnabled(show);
}

void MainWindow::showLabelChangedSlot(bool show)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.showLabel = show;
    } else {
        m_settingsSincSinc.showLabel = show;
    }
    m_graph->axisX()->setLabelFormat(show ? "%.2f" : "");
    m_graph->axisY()->setLabelFormat(show ? "%.2f" : "");
    m_graph->axisZ()->setLabelFormat(show ? "%.2f" : "");
}

void MainWindow::showLabelBorderChangedSlot(bool show)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.showLabelBorder = show;
    } else {
        m_settingsSincSinc.showLabelBorder = show;
    }
    m_graph->activeTheme()->setLabelBorderEnabled(show);
}

void MainWindow::xRangeFromChangedSlot(int value)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.xRangeFrom = value;
    } else {
        m_settingsSincSinc.xRangeFrom = value;
    }
    if (value >= m_settingsSincDist.xRangeTo) {
        emit xRangeToChangedSignal(value + 1);
    }
    m_graph->axisX()->setRange(value, m_graph->axisX()->max());
}

void MainWindow::xRangeToChangedSlot(int value)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.xRangeTo = value;
    } else {
        m_settingsSincSinc.xRangeTo = value;
    }
    if (value <= m_settingsSincDist.xRangeFrom) {
        emit xRangeFromChangedSignal(value - 1);
    }
    m_graph->axisX()->setRange(m_graph->axisX()->min(), value);
}

void MainWindow::zRangeFromChangedSlot(int value)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.zRangeFrom = value;
    } else {
        m_settingsSincSinc.zRangeFrom = value;
    }
    if (value >= m_settingsSincDist.zRangeTo) {
        emit zRangeToChangedSignal(value + 1);
    }
    m_graph->axisZ()->setRange(value, m_graph->axisZ()->max());
}

void MainWindow::zRangeToChangedSlot(int value)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.zRangeTo = value;
    } else {
        m_settingsSincSinc.zRangeTo = value;
    }
    if (value <= m_settingsSincDist.zRangeFrom) {
        emit zRangeFromChangedSignal(value - 1);
    }
    m_graph->axisZ()->setRange(m_graph->axisZ()->min(), value);
}

void MainWindow::xStepsChangedSlot(int value)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.xSteps = value;
    } else {
        m_settingsSincSinc.xSteps = value;
    }
}

void MainWindow::zStepsChangedSlot(int value)
{
    if (m_sincDistSelected) {
        m_settingsSincDist.zSteps = value;
    } else {
        m_settingsSincSinc.zSteps = value;
    }
}

void MainWindow::gradient1ButtonClickedSlot()
{
    if (m_sincDistSelected) {
        m_settingsSincDist.gradient1Selected = true;
    } else {
        m_settingsSincSinc.gradient1Selected = true;
    }
    QLinearGradient rotatedGradient;
    rotatedGradient.setColorAt(1, Qt::darkGreen);
    rotatedGradient.setColorAt(0.6, Qt::yellow);
    rotatedGradient.setColorAt(0.3, Qt::darkYellow);
    rotatedGradient.setColorAt(0, Qt::darkRed);
    m_graph->seriesList().at(0)->setBaseGradient(rotatedGradient);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void MainWindow::gradient2ButtonClickedSlot()
{
    if (m_sincDistSelected) {
        m_settingsSincDist.gradient1Selected = false;
    } else {
        m_settingsSincSinc.gradient1Selected = false;
    }
    QLinearGradient rotatedGradient;
    rotatedGradient.setColorAt(1, Qt::darkGreen);
    rotatedGradient.setColorAt(0.6, Qt::darkCyan);
    rotatedGradient.setColorAt(0.3, Qt::blue);
    rotatedGradient.setColorAt(0, Qt::darkMagenta);
    m_graph->seriesList().at(0)->setBaseGradient(rotatedGradient);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void MainWindow::loadFunctionSettings(GraphSettings &settings, const QString &functionSettingsPrefix)
{
    m_settings->beginGroup(functionSettingsPrefix);

    settings.itemSelectionSelected = m_settings->value("itemSelectionSelected", true).toBool();
    settings.showGrid = m_settings->value("showGrid", true).toBool();
    settings.showLabel = m_settings->value("showLabel", true).toBool();
    settings.showLabelBorder = m_settings->value("showLabelBorder", true).toBool();
    settings.xRangeFrom = m_settings->value("xRangeFrom", -10).toInt();
    settings.xRangeTo = m_settings->value("xRangeTo", 10).toInt();
    settings.zRangeFrom = m_settings->value("zRangeFrom", -10).toInt();
    settings.zRangeTo = m_settings->value("zRangeTo", 10).toInt();
    settings.xSteps = m_settings->value("xSteps", 50).toInt();
    settings.zSteps = m_settings->value("zSteps", 50).toInt();
    settings.gradient1Selected = m_settings->value("gradient1Selected", true).toBool();

    m_settings->endGroup();
}

void MainWindow::revertSettingsSlot()
{
    m_sincDistSelected = m_settings->value("sincDistSelected", true).toBool();
    loadFunctionSettings(m_settingsSincDist, "sincDist");
    loadFunctionSettings(m_settingsSincSinc, "sincSinc");
    if (m_sincDistSelected) {
        emit sincDistSelectedSignal();
    } else {
        emit sincSincSelectedSignal();
    }
}

void MainWindow::uploadFunctionSettings(GraphSettings &settings,
                                        const QString &functionSettingsPrefix)
{
    m_settings->beginGroup(functionSettingsPrefix);

    m_settings->setValue("itemSelectionSelected", settings.itemSelectionSelected);
    m_settings->setValue("showGrid", settings.showGrid);
    m_settings->setValue("showLabel", settings.showLabel);
    m_settings->setValue("showLabelBorder", settings.showLabelBorder);
    m_settings->setValue("xRangeFrom", settings.xRangeFrom);
    m_settings->setValue("xRangeTo", settings.xRangeTo);
    m_settings->setValue("zRangeFrom", settings.zRangeFrom);
    m_settings->setValue("zRangeTo", settings.zRangeTo);
    m_settings->setValue("xSteps", settings.xSteps);
    m_settings->setValue("zSteps", settings.zSteps);
    m_settings->setValue("gradient1Selected", settings.gradient1Selected);

    m_settings->endGroup();
}

void MainWindow::saveSettingsSlot()
{
    m_settings->setValue("sincDistSelected", m_sincDistSelected);
    uploadFunctionSettings(m_settingsSincDist, "sincDist");
    uploadFunctionSettings(m_settingsSincSinc, "sincSinc");
}
