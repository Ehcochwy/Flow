/**
 * @file ConnectorShape.h
 * @brief Connector shape class for linking diagram elements
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include "DiagramShape.h"
#include <QPointF>
#include <QVector>

// 连接器形状，用于连接不同图形
class ConnectorShape : public DiagramShape {
public:
    enum ArrowStyle { None, Start, End, Both };
    
    ConnectorShape();
    
    void paint(QPainter *painter) override;
    bool contains(const QPointF &point) const override;
    QRectF boundingRect() const override;
    void moveBy(const QPointF& delta) override;
    void setSize(const QSizeF& newSize) override;
    QSizeF getSize() const override;
    
    void setStartPoint(const QPointF& point);
    void setEndPoint(const QPointF& point);
    QPointF getStartPoint() const;
    QPointF getEndPoint() const;
    
    void setArrowStyle(ArrowStyle style);
    ArrowStyle getArrowStyle() const;
    
    void addControlPoint(const QPointF& point);
    void clearControlPoints();
    QVector<QPointF> getControlPoints() const;
    
    void save(QDataStream &out) const override;
    void load(QDataStream &in) override;
    
private:
    QPointF startPoint;
    QPointF endPoint;
    QVector<QPointF> controlPoints;
    ArrowStyle arrowStyle;
    
    void drawArrow(QPainter* painter, const QPointF& start, const QPointF& end) const;
};