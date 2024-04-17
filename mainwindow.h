#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>

#include "controls.h"
#include "plot.h"

using namespace QtDataVisualization;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSettings *m_settings;
    QAction *save, *revert;

    Plot *plot;
    Q3DSurface *m_graph;
    QSurface3DSeries *m_sincDistSeries, *m_sincSincSeries;
    ControlsWidget *m_controls;

    void itemSelectionHelper(bool enabled);

    struct GraphSettings
    {
        bool itemSelectionSelected;
        bool showGrid;
        bool showLabel;
        bool showLabelBorder;
        int xRangeFrom;
        int xRangeTo;
        int zRangeFrom;
        int zRangeTo;
        int xSteps;
        int zSteps;
        bool gradient1Selected = true;
    };

    bool m_sincDistSelected = true;
    GraphSettings m_settingsSincDist, m_settingsSincSinc;

    void loadFunctionSettings(GraphSettings &settings, const QString &functionSettingsPrefix);
    void uploadFunctionSettings(GraphSettings &settings, const QString &functionSettingsPrefix);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void emitSettingsChangedSignals();

signals:
    void sincDistSelectedSignal();
    void sincSincSelectedSignal();
    void itemSelectionSelectedSignal();
    void noSelectionSelectedSignal();
    void showGridChangedSignal(bool);
    void showLabelChangedSignal(bool);
    void showLabelBorderChangedSignal(bool);
    void xRangeFromChangedSignal(int);
    void xRangeToChangedSignal(int);
    void zRangeFromChangedSignal(int);
    void zRangeToChangedSignal(int);
    void xStepsChangedSignal(int);
    void zStepsChangedSignal(int);
    void gradient1ButtonClickedSignal();
    void gradient2ButtonClickedSignal();

    void revertSettingsSignal();
    void saveSettingsSignal();

public slots:
    void sincDistSelectedSlot();
    void sincSincSelectedSlot();
    void itemSelectionSelectedSlot();
    void noSelectionSelectedSlot();
    void showGridChangedSlot(bool show);
    void showLabelChangedSlot(bool show);
    void showLabelBorderChangedSlot(bool show);
    void xRangeFromChangedSlot(int value);
    void xRangeToChangedSlot(int value);
    void zRangeFromChangedSlot(int value);
    void zRangeToChangedSlot(int value);
    void xStepsChangedSlot(int value);
    void zStepsChangedSlot(int value);
    void gradient1ButtonClickedSlot();
    void gradient2ButtonClickedSlot();

    void revertSettingsSlot();
    void saveSettingsSlot();
};
#endif // MAINWINDOW_H
