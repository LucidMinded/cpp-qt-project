#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QtWidgets>

class MainWindow; // errors circular include if included, seems like a bug in qtcreator

class ControlsWidget : public QWidget
{
    Q_OBJECT

    QRadioButton *sincDist, *sincSinc;
    QRadioButton *itemSelection, *noSelection;
    QCheckBox *showGrid, *showLabel, *showLabelBorder;
    QSlider *xRangeFrom, *xRangeTo, *zRangeFrom, *zRangeTo, *xSteps, *zSteps;
    QPushButton *gradient1Button, *gradient2Button;

public:
    QLinearGradient *gradient1, *gradient2;
    explicit ControlsWidget(QWidget *parent, MainWindow *mainWindow);

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
};

#endif // CONTROLS_H
