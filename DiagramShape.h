#pragma once

#include <QPainter>
#include <QRectF>
#include <QColor>
#include <QFont>
#include <QDataStream>
#include <memory>
#include <QString>

class DiagramShape {
public:
    enum Type {
        None,
        Rectangle,
        Ellipse,
        Diamond,
        Triangle,
        Connector,
        Text
    };

    DiagramShape(Type type);
    virtual ~DiagramShape() = default;

    virtual void paint(QPainter* painter) = 0;
    virtual bool contains(const QPointF& point) const = 0;
    virtual QRectF boundingRect() const = 0;
    virtual void moveBy(const QPointF& delta) = 0;
    virtual void setSize(const QSizeF& size) = 0;
    virtual QSizeF getSize() const = 0;
    virtual QString getText() const { return m_text; }
    virtual void setText(const QString& text) { m_text = text; }

    virtual void save(QDataStream& out) const;
    virtual void load(QDataStream& in);

    void setPos(const QPointF& pos) { position = pos; }
    QPointF getPos() const { return position; }

    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() const { return isSelected; }

    void setColor(const QColor& color) { shapeColor = color; }
    QColor getColor() const { return shapeColor; }

    void setLineColor(const QColor& color) { lineColor = color; }
    QColor getLineColor() const { return lineColor; }

    void setLineWidth(int width) { lineWidth = width; }
    int getLineWidth() const { return lineWidth; }

    Type getType() const { return type; }

    static std::shared_ptr<DiagramShape> createShape(Type type);

protected:
    QPointF position;
    QColor shapeColor;
    QColor lineColor;
    int lineWidth = 1;
    bool isSelected = false;
    Type type;
    QString m_text;

    void paintSelectionHandles(QPainter* painter, const QRectF& rect) const;
    void paintText(QPainter* painter, const QRectF& rect) const;
};

// 下面的四个形状你可以照这个格式再定义 EllipseShape / DiamondShape / TriangleShape

class RectangleShape : public DiagramShape {
public:
    RectangleShape();
    void paint(QPainter* painter) override;
    bool contains(const QPointF& point) const override;
    QRectF boundingRect() const override;
    void moveBy(const QPointF& delta) override;
    void setSize(const QSizeF& newSize) override;
    QSizeF getSize() const override;
    void save(QDataStream& out) const override;
    void load(QDataStream& in) override;

private:
    QSizeF size;
};

class EllipseShape : public DiagramShape {
public:
    EllipseShape();
    void paint(QPainter* painter) override;
    bool contains(const QPointF& point) const override;
    QRectF boundingRect() const override;
    void moveBy(const QPointF& delta) override;
    void setSize(const QSizeF& newSize) override;
    QSizeF getSize() const override;
    void save(QDataStream& out) const override;
    void load(QDataStream& in) override;

private:
    QSizeF size;
};

class DiamondShape : public DiagramShape {
public:
    DiamondShape();
    void paint(QPainter* painter) override;
    bool contains(const QPointF& point) const override;
    QRectF boundingRect() const override;
    void moveBy(const QPointF& delta) override;
    void setSize(const QSizeF& newSize) override;
    QSizeF getSize() const override;
    void save(QDataStream& out) const override;
    void load(QDataStream& in) override;

private:
    QSizeF size;
};

class TriangleShape : public DiagramShape {
public:
    TriangleShape();
    void paint(QPainter* painter) override;
    bool contains(const QPointF& point) const override;
    QRectF boundingRect() const override;
    void moveBy(const QPointF& delta) override;
    void setSize(const QSizeF& newSize) override;
    QSizeF getSize() const override;
    void save(QDataStream& out) const override;
    void load(QDataStream& in) override;

private:
    QSizeF size;
};
