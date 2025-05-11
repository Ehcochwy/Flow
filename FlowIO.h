/**
 * @file FlowIO.h
 * @brief File IO operations for diagram files
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include <QString>

class DiagramCanvas;

class FlowIO
{
public:
    static bool save(const QString& filename, DiagramCanvas* canvas);
    static bool load(const QString& filename, DiagramCanvas* canvas);
};