/**
 * @file TextShape.cpp
 * @brief Implementation of text shape class
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "TextShape.h"
#include <QFontMetrics>
#include <QPainter>

TextShape::TextShape()
    : DiagramShape(Text)
    , size(QSizeF(100, 30))
    , textColor(Qt::black)
{
    font = QFont("Arial", 10);
    shapeColor = Qt::transparent; // Default transparent background
}

void TextShape::paint(QPainter* painter)
{
    painter->save();

    QRectF rect(position, size);

    // Draw background if not transparent
    if (shapeColor != Qt::transparent) {
        painter->setBrush(shapeColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
    }

    // Draw text
    painter->setFont(font);
    painter->setPen(textColor);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, m_text);

    // Draw selection handles if selected
    if (isSelected) {
        paintSelectionHandles(painter, rect);
    }

    painter->restore();
}

bool TextShape::contains(const QPointF& point) const
{
    return QRectF(position, size).contains(point);
}

QRectF TextShape::boundingRect() const
{
    return QRectF(position, size);
}

void TextShape::moveBy(const QPointF& delta)
{
    position += delta;
}

void TextShape::setSize(const QSizeF& newSize)
{
    size = newSize;
}

QSizeF TextShape::getSize() const
{
    return size;
}

void TextShape::setFont(const QFont& newFont)
{
    font = newFont;
    // Optionally auto-resize based on new font
    if (!m_text.isEmpty()) {
        size = calculateTextSize();
    }
}

QFont TextShape::getFont() const
{
    return font;
}

void TextShape::setTextColor(const QColor& color)
{
    textColor = color;
}

QColor TextShape::getTextColor() const
{
    return textColor;
}

QSizeF TextShape::calculateTextSize() const
{
    if (m_text.isEmpty()) {
        return QSizeF(100, 30); // Default size
    }

    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(QRect(0, 0, 1000, 1000), Qt::TextWordWrap, m_text);

    // Add some padding
    return QSizeF(textRect.width() + 20, textRect.height() + 10);
}

void TextShape::save(QDataStream& out) const
{
    DiagramShape::save(out);
    out << size;
    out << font;
    out << textColor;
}

void TextShape::load(QDataStream& in)
{
    DiagramShape::load(in);
    in >> size;
    in >> font;
    in >> textColor;
}