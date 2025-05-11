/**
 * @file FlowIO.cpp
 * @brief Implementation of file IO operations
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "FlowIO.h"
#include "DiagramCanvas.h"
#include "DiagramShape.h"
#include <QFile>
#include <QDataStream>
#include <QColor>
#include <QSize>

bool FlowIO::save(const QString& filename, DiagramCanvas* canvas)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_5_12);

    // Write file header
    stream << QString("FLOWCHART");
    stream << 1; // version

    // Write canvas properties
    QColor backgroundColor = canvas->backgroundColor();  // assumes getter exists
    QSize canvasSize = canvas->canvasSize();             // assumes getter exists

    stream << backgroundColor;
    stream << canvasSize;

    // Write number of shapes
    auto& shapes = canvas->allShapes();
    stream << shapes.size();

    // Write each shape
    for (const auto& shape : shapes) {
        shape->save(stream);
    }

    file.close();
    return true;
}

bool FlowIO::load(const QString& filename, DiagramCanvas* canvas)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_5_12);

    // Read file header
    QString header;
    stream >> header;
    if (header != "FLOWCHART") {
        file.close();
        return false;
    }

    // Read version
    int version;
    stream >> version;

    // Read canvas properties
    QColor backgroundColor;
    QSize canvasSize;

    stream >> backgroundColor;
    stream >> canvasSize;

    // Set canvas background and size (assumes public setter methods)
    canvas->setBackgroundColor(backgroundColor);
    canvas->setCanvasSize(canvasSize);

    // Clear current canvas
    canvas->clear();

    // Read number of shapes
    int shapeCount;
    stream >> shapeCount;

    // Read each shape
    for (int i = 0; i < shapeCount; ++i) {
        int type;
        stream >> type;

        auto shape = DiagramShape::createShape((DiagramShape::Type)type);
        if (shape) {
            shape->load(stream);
            canvas->addShape(shape);
        }
    }

    file.close();
    return true;
}