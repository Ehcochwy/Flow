/**
 * @file ConnectorShape.cpp
 * @brief Implementation of connector shape class
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "ConnectorShape.h"
#include <QPainterPath>
#include <QtMath>
#include <limits>

ConnectorShape::ConnectorShape()
    : DiagramShape(Connector)
    , arrowStyle(End)
{
}

void ConnectorShape::paint(QPainter* painter)
{
    painter->save();

    // Set line style
    QPen pen(lineColor, lineWidth);
    if (isSelected) {
        pen.setWidth(lineWidth + 1);
        pen.setColor(Qt::blue);
    }
    painter->setPen(pen);

    // Draw connector line (straight or with control points)
    if (controlPoints.isEmpty()) {
        // Straight line
        painter->drawLine(startPoint, endPoint);

        // Draw arrows
        if (arrowStyle == Start || arrowStyle == Both) {
            drawArrow(painter, startPoint, endPoint);
        }
        if (arrowStyle == End || arrowStyle == Both) {
            drawArrow(painter, endPoint, startPoint);
        }
    }
    else {
        // Polyline or curve
        QPainterPath path;
        path.moveTo(startPoint);
        for (const QPointF& point : controlPoints) {
            path.lineTo(point);
        }
        path.lineTo(endPoint);
        painter->drawPath(path);

        // Draw arrows
        if (arrowStyle == Start || arrowStyle == Both) {
            QPointF dir = controlPoints.isEmpty() ? endPoint : controlPoints.first();
            drawArrow(painter, startPoint, dir);
        }
        if (arrowStyle == End || arrowStyle == Both) {
            QPointF dir = controlPoints.isEmpty() ? startPoint : controlPoints.last();
            drawArrow(painter, endPoint, dir);
        }
    }

    // Draw handles if selected
    if (isSelected) {
        painter->setBrush(Qt::white);
        painter->setPen(QPen(Qt::blue, 1));
        const int handleSize = 6;
        QRectF handleRect(-handleSize / 2, -handleSize / 2, handleSize, handleSize);

        // Start and end points
        painter->drawEllipse(startPoint, handleSize / 2, handleSize / 2);
        painter->drawEllipse(endPoint, handleSize / 2, handleSize / 2);

        // Control points
        for (const QPointF& point : controlPoints) {
            painter->drawRect(handleRect.translated(point));
        }
    }

    // Draw text at midpoint if any
    if (!m_text.isEmpty()) {
        QPointF midPoint;
        if (controlPoints.isEmpty()) {
            midPoint = (startPoint + endPoint) / 2;
        }
        else {
            int midIndex = controlPoints.size() / 2;
            midPoint = controlPoints[midIndex];
        }
        QRectF textRect(midPoint.x() - 50, midPoint.y() - 20, 100, 40);
        painter->setPen(Qt::black);
        painter->drawText(textRect, Qt::AlignCenter, m_text);
    }

    painter->restore();
}

bool ConnectorShape::contains(const QPointF& point) const
{
    // Check if the point is near the connector
    const qreal threshold = 5.0; // tolerance

    if (controlPoints.isEmpty()) {
        // Straight line
        QLineF line(startPoint, endPoint);
        qreal distance = QLineF(line.p1(), point).length() + QLineF(line.p2(), point).length();
        qreal lineLength = line.length();
        return qAbs(distance - lineLength) < threshold;
    }
    else {
        // Polyline
        QPointF prevPoint = startPoint;
        for (const QPointF& currPoint : controlPoints) {
            QLineF segment(prevPoint, currPoint);
            qreal distance = QLineF(segment.p1(), point).length() + QLineF(segment.p2(), point).length();
            qreal segmentLength = segment.length();
            if (qAbs(distance - segmentLength) < threshold) {
                return true;
            }
            prevPoint = currPoint;
        }
        // Last segment to endPoint
        QLineF segment(prevPoint, endPoint);
        qreal distance = QLineF(segment.p1(), point).length() + QLineF(segment.p2(), point).length();
        qreal segmentLength = segment.length();
        return qAbs(distance - segmentLength) < threshold;
    }
}

QRectF ConnectorShape::boundingRect() const
{
    // Calculate bounding rect for all points
    QVector<QPointF> allPoints = controlPoints;
    allPoints.prepend(startPoint);
    allPoints.append(endPoint);

    qreal minX = std::numeric_limits<qreal>::max();
    qreal minY = std::numeric_limits<qreal>::max();
    qreal maxX = std::numeric_limits<qreal>::lowest();
    qreal maxY = std::numeric_limits<qreal>::lowest();

    for (const QPointF& point : allPoints) {
        minX = qMin(minX, point.x());
        minY = qMin(minY, point.y());
        maxX = qMax(maxX, point.x());
        maxY = qMax(maxY, point.y());
    }

    // Add margin
    const qreal margin = 10.0;
    return QRectF(minX - margin, minY - margin, maxX - minX + 2 * margin, maxY - minY + 2 * margin);
}

void ConnectorShape::moveBy(const QPointF& delta)
{
    startPoint += delta;
    endPoint += delta;
    for (QPointF& point : controlPoints) {
        point += delta;
    }
}

void ConnectorShape::setSize(const QSizeF& newSize)
{
    // For connectors, use size to set distance between start and end
    QPointF direction = endPoint - startPoint;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length > 0) {
        direction /= length;
        endPoint = startPoint + direction * newSize.width();
    }
}

QSizeF ConnectorShape::getSize() const
{
    QPointF diff = endPoint - startPoint;
    return QSizeF(qSqrt(diff.x() * diff.x() + diff.y() * diff.y()), 0);
}

void ConnectorShape::setStartPoint(const QPointF& point)
{
    startPoint = point;
}

void ConnectorShape::setEndPoint(const QPointF& point)
{
    endPoint = point;
}

QPointF ConnectorShape::getStartPoint() const
{
    return startPoint;
}

QPointF ConnectorShape::getEndPoint() const
{
    return endPoint;
}

void ConnectorShape::setArrowStyle(ArrowStyle style)
{
    arrowStyle = style;
}

ConnectorShape::ArrowStyle ConnectorShape::getArrowStyle() const
{
    return arrowStyle;
}

void ConnectorShape::addControlPoint(const QPointF& point)
{
    controlPoints.append(point);
}

void ConnectorShape::clearControlPoints()
{
    controlPoints.clear();
}

QVector<QPointF> ConnectorShape::getControlPoints() const
{
    return controlPoints;
}

void ConnectorShape::drawArrow(QPainter* painter, const QPointF& tip, const QPointF& from) const
{
    const qreal arrowSize = 10.0; // arrow size
    QLineF line(from, tip);
    qreal angle = std::atan2(-line.dy(), -line.dx());
    QPointF arrowP1 = tip + QPointF(qSin(angle + M_PI / 3) * arrowSize,
        qCos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = tip + QPointF(qSin(angle + M_PI - M_PI / 3) * arrowSize,
        qCos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead << tip << arrowP1 << arrowP2;
    painter->setBrush(lineColor);
    painter->drawPolygon(arrowHead);
}

void ConnectorShape::save(QDataStream& out) const
{
    DiagramShape::save(out);
    out << startPoint;
    out << endPoint;
    out << (int)arrowStyle;
    out << controlPoints.size();
    for (const QPointF& point : controlPoints) {
        out << point;
    }
}

void ConnectorShape::load(QDataStream& in)
{
    DiagramShape::load(in);
    in >> startPoint;
    in >> endPoint;
    int style;
    in >> style;
    arrowStyle = (ArrowStyle)style;
    int pointCount;
    in >> pointCount;
    controlPoints.clear();
    for (int i = 0; i < pointCount; ++i) {
        QPointF point;
        in >> point;
        controlPoints.append(point);
    }
}