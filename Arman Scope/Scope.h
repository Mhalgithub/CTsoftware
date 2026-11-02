#ifndef SCOPE_H
#define SCOPE_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include "qcustomplot.h"
#include "mainwindow.h"

// Cursor structure to hold all cursor elements
struct Cursor {
    QCPItemTracer* tracer;
    QCPItemStraightLine* verticalLine;
    QCPItemStraightLine* horizontalLine;
    QCPItemText* label;
    QPointF position;
    bool isActive;
};

class Scope : public QWidget
{
    Q_OBJECT

public:
    friend class mainwindow;

    explicit Scope(QWidget *parent = nullptr,QCustomPlot* plot=nullptr);
    ~Scope();

    void addSignal(const QVector<double> &xData, const QVector<double> &yData,
                  const QString &name, QColor color = Qt::blue);
    void enableCursors(bool enable);
    void setSnapToData(bool snap);
    void autoZoom();
    void clear();

protected:
    void leaveEvent(QEvent *event) override;

private:
    QCustomPlot *m_customPlot;
    QList<Cursor> m_cursors;
    QList<QSharedPointer<QCPGraphDataContainer>> m_signalData;

    // Delta display items
    QCPItemText *m_deltaXLabel;
    QCPItemText *m_deltaYLabel;

    bool m_cursorVisible;
    bool m_snapToData;
    bool m_addCursorMode;

    // Helper methods
    void setupCursors();
    void updateCursorVisuals();
    void updateDeltaDisplay();
    QPointF findNearestDataPoint(double x, double y, QCPGraph *graph = nullptr);
    QCPGraph* getSelectedGraph();

public slots:
    void onMouseMove(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);
    void onPlottableClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void onAddCursorToggled();
    void onRemoveCursor();
    void onPlottableDoubleClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
};

#endif // SCOPE_H
