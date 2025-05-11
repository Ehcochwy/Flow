/**
 * @file DiagramShape.cpp
 * @brief Implementation of diagram shapes
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "DiagramShape.h"
#include "ConnectorShape.h"
#include "TextShape.h"
#include <QPainterPath>
#include <QPolygonF>
#include <QFont>
#include <QFontMetrics>

DiagramShape::DiagramShape(Type t)
    : type(t)
    , isSelected(false)
    , shapeColor(Qt::white)
    , lineColor(Qt::black)
    , lineWidth(1)
    , m_text("")
{
}

void DiagramShape::save(QDataStream &out) const
{
    out << (int)type;
    out << position;
    out << shapeColor;
    out << lineColor;
    out << lineWidth;
    out << isSelected;
    out << m_text;
}

void DiagramShape::load(QDataStream &in)
{
    int t;
    in >> t;
    type = (Type)t;
    in >> position;
    in >> shapeColor;
    in >> lineColor;
    in >> lineWidth;
    in >> isSelected;
    in >> m_text;
}

void DiagramShape::paintSelectionHandles(QPainter* painter, const QRectF& rect) const
{
    painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
    const int handleSize = 8;
    painter->setPen(QPen(Qt::blue, 1));
    painter->setBrush(Qt::white);
    
    QVector<QPointF> handlePositions = {
        rect.topLeft(),
        QPointF(rect.center().x(), rect.top()),
        rect.topRight(),
        QPointF(rect.right(), rect.center().y()),
        rect.bottomRight(),
        QPointF(rect.center().x(), rect.bottom()),
        rect.bottomLeft(),
        QPointF(rect.left(), rect.center().y())
    };
    
    for (const QPointF& point : handlePositions) {
        QRectF handleRect(
            point.x() - handleSize/2, 
            point.y() - handleSize/2,
            handleSize, 
            handleSize
        );
        painter->drawRect(handleRect);
    }
}

void DiagramShape::paintText(QPainter* painter, const QRectF& rect) const
{
    if (m_text.isEmpty()) {
        return;
    }
    
    painter->save();
    painter->setPen(QPen(Qt::black));
    
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);
    
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, m_text);
    painter->restore();
}

std::shared_ptr<DiagramShape> DiagramShape::createShape(Type type)
{
    switch (type) {
        case Rectangle:
            return std::make_shared<RectangleShape>();
        case Ellipse:
            return std::make_shared<EllipseShape>();
        case Diamond:
            return std::make_shared<DiamondShape>();
        case Triangle:
            return std::make_shared<TriangleShape>();
        case Connector:
            return std::make_shared<ConnectorShape>();
        case Text:
            return std::make_shared<TextShape>();
        default:
            return nullptr;
    }
}

// RectangleShape 实现
RectangleShape::RectangleShape()
    : DiagramShape(Rectangle)
    , size(QSizeF(120, 80))
{
}

void RectangleShape::paint(QPainter *painter)
{
    painter->save();
    painter->setPen(QPen(lineColor, lineWidth));
    painter->setBrush(shapeColor);
    
    QRectF rect(position, size);
    painter->drawRect(rect);
    
    paintText(painter, rect);
    
    if (isSelected) {
        paintSelectionHandles(painter, rect);
    }
    
    painter->restore();
}

bool RectangleShape::contains(const QPointF &point) const
{
    return QRectF(position, size).contains(point);
}

QRectF RectangleShape::boundingRect() const
{
    return QRectF(position, size);
}

void RectangleShape::moveBy(const QPointF& delta)
{
    position += delta;
}

void RectangleShape::setSize(const QSizeF& newSize)
{
    size = newSize;
}

QSizeF RectangleShape::getSize() const
{
    return size;
}

void RectangleShape::save(QDataStream &out) const
{
    DiagramShape::save(out);
    out << size;
}

void RectangleShape::load(QDataStream &in)
{
    DiagramShape::load(in);
    in >> size;
}

// EllipseShape 实现
EllipseShape::EllipseShape()
    : DiagramShape(Ellipse)
    , size(QSizeF(120, 80))
{
}

void EllipseShape::paint(QPainter *painter)
{
    painter->save();
    painter->setPen(QPen(lineColor, lineWidth));
    painter->setBrush(shapeColor);
    
    QRectF rect(position, size);
    painter->drawEllipse(rect);
    
    paintText(painter, rect);
    
    if (isSelected) {
        paintSelectionHandles(painter, rect);
    }
    
    painter->restore();
}

bool EllipseShape::contains(const QPointF &point) const
{
    QPainterPath path;
    path.addEllipse(QRectF(position, size));
    return path.contains(point);
}

QRectF EllipseShape::boundingRect() const
{
    return QRectF(position, size);
}

void EllipseShape::moveBy(const QPointF& delta)
{
    position += delta;
}

void EllipseShape::setSize(const QSizeF& newSize)
{
    size = newSize;
}

QSizeF EllipseShape::getSize() const
{
    return size;
}

void EllipseShape::save(QDataStream &out) const
{
    DiagramShape::save(out);
    out << size;
}

void EllipseShape::load(QDataStream &in)
{
    DiagramShape::load(in);
    in >> size;
}

// DiamondShape 实现
DiamondShape::DiamondShape()
    : DiagramShape(Diamond)
    , size(QSizeF(120, 80))
{
}

void DiamondShape::paint(QPainter *painter)
{
    painter->save();
    painter->setPen(QPen(lineColor, lineWidth));
    painter->setBrush(shapeColor);
    
    QRectF rect(position, size);
    QPolygonF diamond;
    diamond << QPointF(rect.center().x(), rect.top())
           << QPointF(rect.right(), rect.center().y())
           << QPointF(rect.center().x(), rect.bottom())
           << QPointF(rect.left(), rect.center().y());
    
    painter->drawPolygon(diamond);
    
    paintText(painter, rect);
    
    if (isSelected) {
        paintSelectionHandles(painter, rect);
    }
    
    painter->restore();
}

bool DiamondShape::contains(const QPointF &point) const
{
    QRectF rect(position, size);
    QPolygonF diamond;
    diamond << QPointF(rect.center().x(), rect.top())
           << QPointF(rect.right(), rect.center().y())
           << QPointF(rect.center().x(), rect.bottom())
           << QPointF(rect.left(), rect.center().y());
    
    return diamond.containsPoint(point, Qt::OddEvenFill);
}

QRectF DiamondShape::boundingRect() const
{
    return QRectF(position, size);
}

void DiamondShape::moveBy(const QPointF& delta)
{
    position += delta;
}

void DiamondShape::setSize(const QSizeF& newSize)
{
    size = newSize;
}

QSizeF DiamondShape::getSize() const
{
    return size;
}

void DiamondShape::save(QDataStream &out) const
{
    DiagramShape::save(out);
    out << size;
}

void DiamondShape::load(QDataStream &in)
{
    DiagramShape::load(in);
    in >> size;
}

// TriangleShape 实现
TriangleShape::TriangleShape()
    : DiagramShape(Triangle)
    , size(QSizeF(120, 80))
{
}

void TriangleShape::paint(QPainter *painter)
{
    painter->save();
    painter->setPen(QPen(lineColor, lineWidth));
    painter->setBrush(shapeColor);
    
    QRectF rect(position, size);
    QPolygonF triangle;
    triangle << QPointF(rect.center().x(), rect.top())
            << QPointF(rect.right(), rect.bottom())
            << QPointF(rect.left(), rect.bottom());
    
    painter->drawPolygon(triangle);
    
    paintText(painter, rect);
    
    if (isSelected) {
        paintSelectionHandles(painter, rect);
    }
    
    painter->restore();
}

bool TriangleShape::contains(const QPointF &point) const
{
    QRectF rect(position, size);
    QPolygonF triangle;
    triangle << QPointF(rect.center().x(), rect.top())
            << QPointF(rect.right(), rect.bottom())
            << QPointF(rect.left(), rect.bottom());
    
    return triangle.containsPoint(point, Qt::OddEvenFill);
}

QRectF TriangleShape::boundingRect() const
{
    return QRectF(position, size);
}

void TriangleShape::moveBy(const QPointF& delta)
{
    position += delta;
}

void TriangleShape::setSize(const QSizeF& newSize)
{
    size = newSize;
}

QSizeF TriangleShape::getSize() const
{
    return size;
}

void TriangleShape::save(QDataStream &out) const
{
    DiagramShape::save(out);
    out << size;
}

void TriangleShape::load(QDataStream &in)
{
    DiagramShape::load(in);
    in >> size;
}