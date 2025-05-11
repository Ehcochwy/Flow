/**
 * @file DiagramCanvas.h
 * @brief Canvas widget for diagram editing
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include <QWidget>
#include <QList>
#include <QColor>
#include <memory>
#include "DiagramShape.h"

class DiagramCanvas : public QWidget
{
    Q_OBJECT
public:
    DiagramCanvas(QWidget* parent = nullptr);
    ~DiagramCanvas() = default;
    
    void addShape(std::shared_ptr<DiagramShape> shape);
    void clear();
    bool exportToPng(const QString& filename);
    bool exportToSvg(const QString& filename);
    
    QList<std::shared_ptr<DiagramShape>>& allShapes() { return m_shapes; }
    void setAllShapes(const QList<std::shared_ptr<DiagramShape>>& shapes);
    
    bool isModified() const { return m_modified; }
    void setModified(bool modified) { m_modified = modified; }
    
    //LAYOUT
    void bringToFront();
    void sendToBack();
    void bringForward();
    void sendBackward();
    
    //PAGE
    void chooseBackgroundColor();
    void setCanvasSize();
    
    
    void deleteSelected();
public:
    // These getters/setters are needed for FlowIO serialization/deserialization
    QColor backgroundColor() const { return m_backgroundColor; }
    QSize canvasSize() const { return m_canvasSize; }
    void setBackgroundColor(const QColor& color) { m_backgroundColor = color; update(); }
    void setCanvasSize(const QSize& size) { m_canvasSize = size; resize(size); update(); }
    
    
    //CLIPERBOARD
public slots:
    void copySelectedToClipboard();
    void cutSelectedToClipboard();
    void pasteFromClipboard();
    void duplicateSelected();
    void setActiveShapeTool(int type);
    void refreshCanvas();
signals:
    void shapeSelected(std::shared_ptr<DiagramShape> shape);
    void selectionChanged(bool hasSelection);
    
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    
private:
    std::shared_ptr<DiagramShape> findShapeAt(const QPointF& pos);
    void createNewShape(DiagramShape::Type type, const QPointF& pos);
    void updateSelectionState();
    
    QList<std::shared_ptr<DiagramShape>> m_shapes;
    std::shared_ptr<DiagramShape> m_selectedShape;
    QList<std::shared_ptr<DiagramShape>> m_selectedShapes; //MULTI CHOOSE
    
    QColor m_backgroundColor;
    QSize m_canvasSize;
    bool m_modified;
    QPointF m_lastMousePos;
    bool m_isDragging;
    bool m_isCreating;
    bool m_isResizing;
    int m_resizeHandle;
    DiagramShape::Type m_activeShapeTool;
    bool m_isConnecting;
    std::shared_ptr<DiagramShape> m_startConnectShape;
    QPointF m_connectStartPoint;
};