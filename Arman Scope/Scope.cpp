#include "scope.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QDebug>
#include <limits>
#include <cmath>

Scope::Scope(QWidget *parent,QCustomPlot* plot) :
    QWidget(parent),
    m_cursorVisible(false),
    m_snapToData(true),
    m_addCursorMode(false)
{

    // Setup plot
    m_customPlot = plot;

    // Configure plot
    m_customPlot->xAxis->setLabel("Voltage (V)");
    m_customPlot->yAxis->setLabel("Current (mA)");
    m_customPlot->legend->setVisible(true);
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom|Qt::AlignRight);

    // Delta display
    m_deltaXLabel = new QCPItemText(m_customPlot);
    m_deltaYLabel = new QCPItemText(m_customPlot);
    for (auto label : {m_deltaXLabel, m_deltaYLabel}) {
        label->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
        label->setBrush(QBrush(QColor(255, 255, 255, 200)));
        label->setPen(QPen(Qt::black));
        label->setPadding(QMargins(5, 5, 5, 5));
        label->setVisible(false);
    }

    // Connect signals
    connect(m_customPlot, &QCustomPlot::mouseMove, this, &Scope::onMouseMove);
    connect(m_customPlot, &QCustomPlot::mousePress, this, &Scope::onMousePress);
    connect(m_customPlot, &QCustomPlot::plottableClick, this, &Scope::onPlottableClick);
    connect(m_customPlot, &QCustomPlot::plottableDoubleClick, this, &Scope::onPlottableDoubleClick);

    // Add initial cursor
    setupCursors();
}

Scope::~Scope() {
    // Cleanup handled by Qt's parent-child system
}

void Scope::addSignal(const QVector<double> &xData, const QVector<double> &yData,
                     const QString &name, QColor color) {
    QCPGraph *graph = m_customPlot->addGraph();
    graph->setData(xData, yData);
    graph->setName(name);
    graph->setPen(QPen(color, 2));
    graph->setSelectable(QCP::stDataRange);

    // Store data for snapping
    auto dataContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    for (int i = 0; i < xData.size(); ++i) {
        dataContainer->add(QCPGraphData(xData[i], yData[i]));
    }
    m_signalData.append(dataContainer);

    autoZoom();
}

void Scope::enableCursors(bool enable) {
    m_cursorVisible = enable;
    for (auto &cursor : m_cursors) {
        cursor.tracer->setVisible(enable);
        cursor.verticalLine->setVisible(enable);
        cursor.horizontalLine->setVisible(enable);
        cursor.label->setVisible(enable);
    }
    m_deltaXLabel->setVisible(enable && m_cursors.size() > 1);
    m_deltaYLabel->setVisible(enable && m_cursors.size() > 1);
    m_customPlot->replot();
}

void Scope::setSnapToData(bool snap) {
    m_snapToData = snap;
}

void Scope::autoZoom() {
    m_customPlot->rescaleAxes();
    m_customPlot->replot();
}

void Scope::clear() {
    m_customPlot->clearGraphs();
    m_signalData.clear();
    m_customPlot->replot();
}

void Scope::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    // Keep cursors visible when mouse leaves
}

void Scope::onMouseMove(QMouseEvent *event) {
    if (!m_cursorVisible || m_cursors.isEmpty()) return;

    double mouseX = m_customPlot->xAxis->pixelToCoord(event->pos().x());
    double mouseY = m_customPlot->yAxis->pixelToCoord(event->pos().y());

    // Find active cursor
    int activeIdx = -1;
    for (int i = 0; i < m_cursors.size(); ++i) {
        if (m_cursors[i].isActive) {
            activeIdx = i;
            break;
        }
    }

    if (activeIdx == -1) return;

    // Update cursor position with optional snapping
    QCPGraph *selectedGraph = getSelectedGraph();
    m_cursors[activeIdx].position = m_snapToData ?
        findNearestDataPoint(mouseX, mouseY, selectedGraph) : QPointF(mouseX, mouseY);

    updateCursorVisuals();
}

void Scope::onMousePress(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_cursorVisible) {
        double mouseX = m_customPlot->xAxis->pixelToCoord(event->pos().x());
        double mouseY = m_customPlot->yAxis->pixelToCoord(event->pos().y());

        // Check if we're in add cursor mode
        if (m_addCursorMode) {
            // Add new cursor at mouse position
            Cursor newCursor;
            int cursorIdx = m_cursors.size();
            QColor color = QColor::fromHsv((cursorIdx * 60) % 360, 255, 255);

            // Create cursor elements
            newCursor.tracer = new QCPItemTracer(m_customPlot);
            newCursor.verticalLine = new QCPItemStraightLine(m_customPlot);
            newCursor.horizontalLine = new QCPItemStraightLine(m_customPlot);
            newCursor.label = new QCPItemText(m_customPlot);

            // Style cursor elements
            newCursor.tracer->setStyle(QCPItemTracer::tsCircle);
            newCursor.tracer->setSize(8);
            newCursor.tracer->setPen(QPen(color));
            newCursor.tracer->setBrush(QBrush(color));

            newCursor.verticalLine->setPen(QPen(color, 1, Qt::DashLine));
            newCursor.horizontalLine->setPen(QPen(color, 1, Qt::DashLine));

            newCursor.label->setBrush(QBrush(QColor(255, 255, 255, 200)));
            newCursor.label->setPen(QPen(Qt::black));
            newCursor.label->setPadding(QMargins(5, 5, 5, 5));

            // Set initial position
            QCPGraph *selectedGraph = getSelectedGraph();
            newCursor.position = m_snapToData ?
                findNearestDataPoint(mouseX, mouseY, selectedGraph) : QPointF(mouseX, mouseY);
            newCursor.isActive = true;

            // Deactivate other cursors
            for (auto &cursor : m_cursors) {
                cursor.isActive = false;
            }

            m_cursors.append(newCursor);
            updateCursorVisuals();
            m_addCursorMode = false;
        } else {
            // Select existing cursor
            for (int i = 0; i < m_cursors.size(); ++i) {
                double dist = QPointF(m_cursors[i].position.x() - mouseX,
                                    m_cursors[i].position.y() - mouseY).manhattanLength();

                if (dist < 0.5) { // Selection threshold
                    // Deactivate all cursors first
                    for (auto &cursor : m_cursors) {
                        cursor.isActive = false;
                    }
                    // Activate this cursor
                    m_cursors[i].isActive = true;
                    updateCursorVisuals();
                    break;
                }
            }
        }
    }
}

void Scope::onPlottableClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
    Q_UNUSED(dataIndex);
    Q_UNUSED(event);

    // Deselect all other graphs
    for (int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->selectionChanged(false);
    }

    // Select the clicked graph
    plottable->selectionChanged(true);
}

void Scope::onAddCursorToggled() {
    m_addCursorMode = true;
}

void Scope::onRemoveCursor() {
    if (m_cursors.size() <= 1) return;

    // Find and remove active cursor
    for (int i = 0; i < m_cursors.size(); ++i) {
        if (m_cursors[i].isActive) {
            // Remove cursor elements
            m_customPlot->removeItem(m_cursors[i].tracer);
            m_customPlot->removeItem(m_cursors[i].verticalLine);
            m_customPlot->removeItem(m_cursors[i].horizontalLine);
            m_customPlot->removeItem(m_cursors[i].label);

            m_cursors.removeAt(i);

            // Activate first cursor if none active
            if (m_cursors.size() > 0 &&
                std::none_of(m_cursors.begin(), m_cursors.end(),
                            [](const Cursor &c) { return c.isActive; })) {
                m_cursors[0].isActive = true;
            }

            updateCursorVisuals();
            break;
        }
    }
}

void Scope::onPlottableDoubleClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Create a custom dialog
        QDialog dialog(this);
        dialog.setWindowTitle("Trace Properties");

        // Create widgets
        QLabel *nameLabel = new QLabel("Name:", &dialog);
        QLineEdit *nameEdit = new QLineEdit(plottable->name(), &dialog);
        QPushButton *colorButton = new QPushButton("Change Color", &dialog);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        QPushButton *deleteButton = new QPushButton("Delete", &dialog);
        buttonBox->addButton(deleteButton, QDialogButtonBox::DestructiveRole);
        // Set up layout
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QHBoxLayout *nameLayout = new QHBoxLayout();
        nameLayout->addWidget(nameLabel);
        nameLayout->addWidget(nameEdit);
        layout->addLayout(nameLayout);
        layout->addWidget(colorButton);
        layout->addWidget(buttonBox);

        // Store the original color
        QColor originalColor = plottable->pen().color();
        QColor currentColor = originalColor;

        // Connect signals
        connect(colorButton, &QPushButton::clicked, [&]() {
            // Generate a random color
            currentColor = QColor::fromRgb(QRandomGenerator::global()->generate());
            // Update the button text to show the color
            colorButton->setStyleSheet(QString("background-color: %1; color: %2")
                                      .arg(currentColor.name())
                                      .arg(currentColor.lightness() > 128 ? "black" : "white"));
        });

        connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        connect(deleteButton, &QPushButton::clicked, [&]() {
            if (QMessageBox::question(this, "Confirm Delete",
                                     "Are you sure you want to delete this trace?",
                                     QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                dialog.done(2); // Use a custom return code for delete
            }
        });

        // Show the dialog
        int result = dialog.exec();

        // Handle the result
        if (result == QDialog::Accepted) {
            // Update the plottable with new name and color
            plottable->setName(nameEdit->text());
            QPen pen = plottable->pen();
            pen.setColor(currentColor);
            plottable->setPen(pen);
            m_customPlot->replot();
        } else if (result == 2) {
            // Delete the plottable
            m_customPlot->removePlottable(plottable);
            m_customPlot->replot();
        }
    }
}

void Scope::setupCursors() {
    // Add initial cursor
    Cursor initialCursor;
    initialCursor.tracer = new QCPItemTracer(m_customPlot);
    initialCursor.verticalLine = new QCPItemStraightLine(m_customPlot);
    initialCursor.horizontalLine = new QCPItemStraightLine(m_customPlot);
    initialCursor.label = new QCPItemText(m_customPlot);
    initialCursor.position = QPointF(0, 0);
    initialCursor.isActive = true;

    // Style initial cursor
    initialCursor.tracer->setStyle(QCPItemTracer::tsCircle);
    initialCursor.tracer->setSize(8);
    initialCursor.tracer->setPen(QPen(Qt::red));
    initialCursor.tracer->setBrush(QBrush(Qt::red));

    initialCursor.verticalLine->setPen(QPen(Qt::red, 1, Qt::DashLine));
    initialCursor.horizontalLine->setPen(QPen(Qt::red, 1, Qt::DashLine));

    initialCursor.label->setBrush(QBrush(QColor(255, 255, 255, 200)));
    initialCursor.label->setPen(QPen(Qt::black));
    initialCursor.label->setPadding(QMargins(5, 5, 5, 5));
    initialCursor.label->setPositionAlignment(Qt::AlignTop|Qt::AlignRight|Qt::AlignJustify|Qt::AlignBaseline);

    m_cursors.append(initialCursor);
    updateCursorVisuals();
}

void Scope::updateCursorVisuals() {
    for (int i = 0; i < m_cursors.size(); ++i) {
        Cursor &cursor = m_cursors[i];
        QColor color = QColor::fromHsv((i * 60) % 360, 255, 255);

        if (cursor.isActive) {
            color = color.darker(150); // Darker color for active cursor
        }

        // Update cursor elements
        cursor.tracer->position->setCoords(cursor.position);
        cursor.tracer->setPen(QPen(color));
        cursor.tracer->setBrush(QBrush(color));

        QCPRange yRange = m_customPlot->yAxis->range();
        cursor.verticalLine->point1->setCoords(cursor.position.x(), yRange.lower);
        cursor.verticalLine->point2->setCoords(cursor.position.x(), yRange.upper);
        cursor.verticalLine->setPen(QPen(color, 1, Qt::DashLine));

        QCPRange xRange = m_customPlot->xAxis->range();
        cursor.horizontalLine->point1->setCoords(xRange.lower, cursor.position.y());
        cursor.horizontalLine->point2->setCoords(xRange.upper, cursor.position.y());
        cursor.horizontalLine->setPen(QPen(color, 1, Qt::DashLine));

        cursor.label->setText(QString("C%1: X=%2, Y=%3")
                                 .arg(i+1)
                                 .arg(cursor.position.x(), 0, 'f', 3)
                                 .arg(cursor.position.y(), 0, 'f', 3));
        cursor.label->position->setType(QCPItemPosition::ptAxisRectRatio);
        cursor.label->position->setCoords(0.95, 0.05 + i * 0.12);
        cursor.label->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);

        // Set visibility
        bool visible = m_cursorVisible;
        cursor.tracer->setVisible(visible);
        cursor.verticalLine->setVisible(visible);
        cursor.horizontalLine->setVisible(visible);
        cursor.label->setVisible(visible);
    }

    // Update delta display
    updateDeltaDisplay();
    m_customPlot->replot();
}

void Scope::updateDeltaDisplay() {
    if (m_cursors.size() < 2 || !m_cursorVisible) {
        m_deltaXLabel->setVisible(false);
        m_deltaYLabel->setVisible(false);
        return;
    }

    // Calculate differences between first two cursors
    double deltaX = m_cursors[1].position.x() - m_cursors[0].position.x();
    double deltaY = m_cursors[1].position.y() - m_cursors[0].position.y();

    // Update labels
    m_deltaXLabel->setText(QString("ΔX: %1 s").arg(deltaX, 0, 'f', 3));
    m_deltaYLabel->setText(QString("ΔY: %1 V").arg(deltaY, 0, 'f', 3));

    // Position labels above the legend
    m_deltaXLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    m_deltaXLabel->position->setCoords(0.05, 0.05);

    m_deltaYLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    m_deltaYLabel->position->setCoords(0.05, 0.15);

    m_deltaXLabel->setVisible(true);
    m_deltaYLabel->setVisible(true);
}

QPointF Scope::findNearestDataPoint(double x, double y, QCPGraph *graph) {
    if (!graph && m_customPlot->graphCount() > 0) {
        graph = m_customPlot->graph(0); // Default to first graph
    }

    if (!graph) return QPointF(x, y);

    QSharedPointer<QCPGraphDataContainer> data = graph->data();
    double minDistance = std::numeric_limits<double>::max();
    QPointF nearestPoint(x, y);

    for (auto it = data->constBegin(); it != data->constEnd(); ++it) {
        double distance = qAbs(it->key - x);
        if (distance < minDistance) {
            minDistance = distance;
            nearestPoint.setX(it->key);
            nearestPoint.setY(it->value);
        }
    }

    return nearestPoint;
}

QCPGraph* Scope::getSelectedGraph() {
    for (int i = 0; i < m_customPlot->graphCount(); ++i) {
        if (m_customPlot->graph(i)->selected()) {
            return m_customPlot->graph(i);
        }
    }

    // Return first graph if none selected
    return m_customPlot->graphCount() > 0 ? m_customPlot->graph(0) : nullptr;
}