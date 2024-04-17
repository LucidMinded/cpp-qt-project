#include "plot.h"

using namespace QtDataVisualization;

Plot::Plot(QObject *parent)
    : QObject(parent)
    , m_sincDistanceProxy(new QSurfaceDataProxy)
    , m_sincSincProxy(new QSurfaceDataProxy)
    , m_xMin(DEFAULT_X_MIN)
    , m_xMax(DEFAULT_X_MAX)
    , m_zMin(DEFAULT_Z_MIN)
    , m_zMax(DEFAULT_Z_MAX)
    , m_stepsX(DEFAULT_STEPS_X)
    , m_stepsZ(DEFAULT_STEPS_Z)
{
    fillSincDistance();
    fillSincSinc();
}

float sinc(float value)
{
    return qAbs(value) < 0.0001f ? 1 : qSin(value) / value;
}

void Plot::fillSincDistance()
{
    float stepX = (m_xMax - m_xMin) / float(m_stepsX - 1);
    float stepZ = (m_zMax - m_zMin) / float(m_stepsZ - 1);

    QSurfaceDataArray *array = new QSurfaceDataArray;
    array->reserve(m_stepsZ);
    for (int i = 0; i < m_stepsZ; ++i) {
        QSurfaceDataRow *row = new QSurfaceDataRow(m_stepsX);
        float z = qMin(m_zMax, (i * stepZ + m_zMin));
        for (int j = 0; j < m_stepsX; ++j) {
            float x = qMin(m_xMax, (j * stepX + m_xMin));
            float distFromZero = qSqrt(x * x + z * z);
            float y = sinc(distFromZero);
            (*row)[j].setPosition(QVector3D(x, y, z));
        }
        *array << row;
    }

    m_sincDistanceProxy->resetArray(array);
}

void Plot::fillSincSinc()
{
    float stepX = (m_xMax - m_xMin) / float(m_stepsX - 1);
    float stepZ = (m_zMax - m_zMin) / float(m_stepsZ - 1);

    QSurfaceDataArray *array = new QSurfaceDataArray;
    array->reserve(m_stepsZ);
    for (int i = 0; i < m_stepsZ; ++i) {
        QSurfaceDataRow *row = new QSurfaceDataRow(m_stepsX);
        float z = qMin(m_zMax, (i * stepZ + m_zMin));
        for (int j = 0; j < m_stepsX; ++j) {
            float x = qMin(m_xMax, (j * stepX + m_xMin));
            float y = sinc(x) * sinc(z);
            (*row)[j].setPosition(QVector3D(x, y, z));
        }
        *array << row;
    }

    m_sincSincProxy->resetArray(array);
}

QSurfaceDataProxy *Plot::getSincDistanceProxy() const
{
    return m_sincDistanceProxy;
}

QSurfaceDataProxy *Plot::getSincSincProxy() const
{
    return m_sincSincProxy;
}

void Plot::xStepsChangedSlot(int value)
{
    m_stepsX = value;
    fillSincDistance();
    fillSincSinc();
}

void Plot::zStepsChangedSlot(int value)
{
    m_stepsZ = value;
    fillSincDistance();
    fillSincSinc();
}
