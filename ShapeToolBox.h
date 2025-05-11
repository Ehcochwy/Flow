/**
 * @file ShapeToolBox.h
 * @brief Toolbox widget for shape selection
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include <QWidget>
#include <QToolBar>
#include <QButtonGroup>
#include "DiagramShape.h"

class ShapeToolBox : public QWidget
{
    Q_OBJECT
public:
    ShapeToolBox(QWidget* parent = nullptr);
    ~ShapeToolBox() = default;
    
    void addToolBarActions(QToolBar* toolbar);
    
signals:
    void shapeToolSelected(int type);
    
private slots:
    void onButtonClicked(int id);
    
private:
    QButtonGroup* m_buttonGroup;
};