/**
 * @file ShapeToolBox.cpp
 * @brief Implementation of the shape toolbox
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "ShapeToolBox.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QAction>
#include <QIcon>

ShapeToolBox::ShapeToolBox(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);

    // Selection tool
    QPushButton* selectButton = new QPushButton(tr("Select"), this);
    selectButton->setCheckable(true);
    selectButton->setChecked(true);
    m_buttonGroup->addButton(selectButton, DiagramShape::None);
    layout->addWidget(selectButton);

    // Rectangle tool
    QPushButton* rectButton = new QPushButton(tr("Rectangle"), this);
    rectButton->setCheckable(true);
    m_buttonGroup->addButton(rectButton, DiagramShape::Rectangle);
    layout->addWidget(rectButton);

    // Ellipse tool
    QPushButton* ellipseButton = new QPushButton(tr("Ellipse"), this);
    ellipseButton->setCheckable(true);
    m_buttonGroup->addButton(ellipseButton, DiagramShape::Ellipse);
    layout->addWidget(ellipseButton);

    // Diamond tool
    QPushButton* diamondButton = new QPushButton(tr("Diamond"), this);
    diamondButton->setCheckable(true);
    m_buttonGroup->addButton(diamondButton, DiagramShape::Diamond);
    layout->addWidget(diamondButton);

    // Triangle tool
    QPushButton* triangleButton = new QPushButton(tr("Triangle"), this);
    triangleButton->setCheckable(true);
    m_buttonGroup->addButton(triangleButton, DiagramShape::Triangle);
    layout->addWidget(triangleButton);

    // Connector tool
    QPushButton* connectorButton = new QPushButton(tr("Connector"), this);
    connectorButton->setCheckable(true);
    m_buttonGroup->addButton(connectorButton, DiagramShape::Connector);
    layout->addWidget(connectorButton);

    // Text tool
    QPushButton* textButton = new QPushButton(tr("Text"), this);
    textButton->setCheckable(true);
    m_buttonGroup->addButton(textButton, DiagramShape::Text);
    layout->addWidget(textButton);

    // Add stretch to push buttons to the top
    layout->addStretch();

    // Connect button group signal
    connect(m_buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
        this, &ShapeToolBox::onButtonClicked);
}

void ShapeToolBox::addToolBarActions(QToolBar* toolbar)
{
    // Selection tool
    QAction* selectAction = toolbar->addAction(tr("Select"));
    selectAction->setCheckable(true);
    selectAction->setChecked(true);

    // Rectangle tool
    QAction* rectAction = toolbar->addAction(tr("Rectangle"));
    rectAction->setCheckable(true);

    // Ellipse tool
    QAction* ellipseAction = toolbar->addAction(tr("Ellipse"));
    ellipseAction->setCheckable(true);

    // Diamond tool
    QAction* diamondAction = toolbar->addAction(tr("Diamond"));
    diamondAction->setCheckable(true);

    // Triangle tool
    QAction* triangleAction = toolbar->addAction(tr("Triangle"));
    triangleAction->setCheckable(true);

    // Connector tool
    QAction* connectorAction = toolbar->addAction(tr("Connector"));
    connectorAction->setCheckable(true);

    // Text tool
    QAction* textAction = toolbar->addAction(tr("Text"));
    textAction->setCheckable(true);

    // Create an exclusive action group
    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(selectAction);
    actionGroup->addAction(rectAction);
    actionGroup->addAction(ellipseAction);
    actionGroup->addAction(diamondAction);
    actionGroup->addAction(triangleAction);
    actionGroup->addAction(connectorAction);
    actionGroup->addAction(textAction);

    // Connect signals
    connect(selectAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::None); });
    connect(rectAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::Rectangle); });
    connect(ellipseAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::Ellipse); });
    connect(diamondAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::Diamond); });
    connect(triangleAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::Triangle); });
    connect(connectorAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::Connector); });
    connect(textAction, &QAction::triggered, [this]() { emit shapeToolSelected(DiagramShape::Text); });
}

void ShapeToolBox::onButtonClicked(int id)
{
    emit shapeToolSelected(id);
}