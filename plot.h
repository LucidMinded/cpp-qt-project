#ifndef PLOT_H
#define PLOT_H

#include <QtDataVisualization>

using namespace QtDataVisualization;

class Plot : public QObject
{
    Q_OBJECT

    QSurfaceDataProxy *m_sincDistanceProxy, *m_sincSincProxy;
    float m_xMin, m_xMax, m_zMin, m_zMax;
    int m_stepsX, m_stepsZ;
    static constexpr float DEFAULT_X_MIN = -10, DEFAULT_X_MAX = 10, DEFAULT_Z_MIN = -10,
                           DEFAULT_Z_MAX = 10;
    static constexpr int DEFAULT_STEPS_X = 50, DEFAULT_STEPS_Z = 50;

    void fillSincDistance();
    void fillSincSinc();

public:
    explicit Plot(QObject *parent = nullptr);

    QSurfaceDataProxy *getSincDistanceProxy() const;
    QSurfaceDataProxy *getSincSincProxy() const;

public slots:
    void xStepsChangedSlot(int value);
    void zStepsChangedSlot(int value);
};

#endif // PLOT_H
