/**
 * @file TextShape.h
 * @brief Text shape class for diagram labels
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include "DiagramShape.h"
#include <QFont>
#include <QColor>

 // Text shape for diagram labels
class TextShape : public DiagramShape {
public:
    TextShape();

    void paint(QPainter* painter) override;
    bool contains(const QPointF& point) const override;
    QRectF boundingRect() const override;
    void moveBy(const QPointF& delta) override;
    void setSize(const QSizeF& newSize) override;
    QSizeF getSize() const override;

    void setFont(const QFont& font);
    QFont getFont() const;

    void setTextColor(const QColor& color);
    QColor getTextColor() const;

    void save(QDataStream& out) const override;
    void load(QDataStream& in) override;

private:
    QSizeF size;
    QFont font;
    QColor textColor;

    QSizeF calculateTextSize() const;
};