/**
 * @file PropertyPanel.cpp
 * @brief Implementation of property panel
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "PropertyPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QColorDialog>
#include <QFontDatabase>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>

 // Constructor
PropertyPanel::PropertyPanel(QWidget* parent)
    : QWidget(parent)
    , m_shape(nullptr)
{
    setupUI();
}

// Build the UI
void PropertyPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Text editing area
    QGroupBox* textGroup = new QGroupBox(tr("Text"), this);
    QVBoxLayout* textLayout = new QVBoxLayout(textGroup);

    m_textEdit = new QLineEdit(this);
    m_textEdit->setEnabled(false);
    textLayout->addWidget(m_textEdit);

    QHBoxLayout* fontLayout = new QHBoxLayout();
    m_fontCombo = new QComboBox(this);
    QFontDatabase fontDB;
    m_fontCombo->addItems(fontDB.families());
    m_fontCombo->setEnabled(false);

    m_fontSizeSpin = new QSpinBox(this);
    m_fontSizeSpin->setRange(8, 72);
    m_fontSizeSpin->setValue(10);
    m_fontSizeSpin->setEnabled(false);

    m_textColorBtn = new QPushButton(tr("Text Color"), this);
    m_textColorBtn->setEnabled(false);

    fontLayout->addWidget(m_fontCombo);
    fontLayout->addWidget(m_fontSizeSpin);
    fontLayout->addWidget(m_textColorBtn);

    textLayout->addLayout(fontLayout);
    mainLayout->addWidget(textGroup);

    // Color and line style settings
    QGroupBox* styleGroup = new QGroupBox(tr("Style"), this);
    QFormLayout* styleLayout = new QFormLayout(styleGroup);

    m_fillColorBtn = new QPushButton(tr("Fill Color"), this);
    m_fillColorBtn->setEnabled(false);

    m_lineColorBtn = new QPushButton(tr("Line Color"), this);
    m_lineColorBtn->setEnabled(false);

    m_lineWidthSpin = new QSpinBox(this);
    m_lineWidthSpin->setRange(1, 10);
    m_lineWidthSpin->setValue(1);
    m_lineWidthSpin->setEnabled(false);

    styleLayout->addRow(new QLabel(tr("Fill:")), m_fillColorBtn);
    styleLayout->addRow(new QLabel(tr("Line:")), m_lineColorBtn);
    styleLayout->addRow(new QLabel(tr("Width:")), m_lineWidthSpin);

    mainLayout->addWidget(styleGroup);

    // Position and size info
    QGroupBox* infoGroup = new QGroupBox(tr("Info"), this);
    QFormLayout* infoLayout = new QFormLayout(infoGroup);

    QLabel* posLabel = new QLabel(tr("Position: --"), this);
    QLabel* sizeLabel = new QLabel(tr("Size: --"), this);

    infoLayout->addRow(posLabel);
    infoLayout->addRow(sizeLabel);

    mainLayout->addWidget(infoGroup);

    // Add stretch to push controls to top
    mainLayout->addStretch();

    // Connect signals
    connect(m_textEdit, &QLineEdit::textChanged, this, &PropertyPanel::onTextChanged);
    connect(m_fillColorBtn, &QPushButton::clicked, this, &PropertyPanel::onFillColorClicked);
    connect(m_lineColorBtn, &QPushButton::clicked, this, &PropertyPanel::onLineColorClicked);
    connect(m_textColorBtn, &QPushButton::clicked, this, &PropertyPanel::onTextColorClicked);
    connect(m_lineWidthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &PropertyPanel::onLineWidthChanged);
}

// Set the current shape to display/edit properties for
void PropertyPanel::setShape(std::shared_ptr<DiagramShape> shape)
{
    m_shape = shape;
    updateUI();
}

// Update the UI controls to match the selected shape
void PropertyPanel::updateUI()
{
    bool hasShape = (m_shape != nullptr);

    m_textEdit->setEnabled(hasShape);
    m_fillColorBtn->setEnabled(hasShape);
    m_lineColorBtn->setEnabled(hasShape);
    m_lineWidthSpin->setEnabled(hasShape);
    m_fontCombo->setEnabled(hasShape);
    m_fontSizeSpin->setEnabled(hasShape);
    m_textColorBtn->setEnabled(hasShape);

    if (hasShape) {
        m_textEdit->setText(m_shape->getText());
        m_lineWidthSpin->setValue(m_shape->getLineWidth());

        // Set button background color to indicate current color
        QString fillStyleSheet = QString("background-color: %1;").arg(m_shape->getColor().name());
        m_fillColorBtn->setStyleSheet(fillStyleSheet);

        QString lineStyleSheet = QString("background-color: %1;").arg(m_shape->getLineColor().name());
        m_lineColorBtn->setStyleSheet(lineStyleSheet);
    }
    else {
        m_textEdit->clear();
        m_lineWidthSpin->setValue(1);
        m_fillColorBtn->setStyleSheet("");
        m_lineColorBtn->setStyleSheet("");
    }
}

// Handler: Fill color button clicked
void PropertyPanel::onFillColorClicked()
{
    if (!m_shape) return;

    QColor color = QColorDialog::getColor(m_shape->getColor(), this, tr("Select fill color"));
    if (color.isValid()) {
        m_shape->setColor(color);
        QString styleSheet = QString("background-color: %1;").arg(color.name());
        m_fillColorBtn->setStyleSheet(styleSheet);
        emit shapeChanged();
    }
}

// Handler: Line color button clicked
void PropertyPanel::onLineColorClicked()
{
    if (!m_shape) return;

    QColor color = QColorDialog::getColor(m_shape->getLineColor(), this, tr("Select line color"));
    if (color.isValid()) {
        m_shape->setLineColor(color);
        QString styleSheet = QString("background-color: %1;").arg(color.name());
        m_lineColorBtn->setStyleSheet(styleSheet);
        emit shapeChanged();
    }
}

// Handler: Line width changed
void PropertyPanel::onLineWidthChanged(int width)
{
    if (!m_shape) return;

    m_shape->setLineWidth(width);
    emit shapeChanged();
}

// Handler: Text changed
void PropertyPanel::onTextChanged()
{
    if (!m_shape) return;

    m_shape->setText(m_textEdit->text());
    emit shapeChanged();
}

// Handler: Text color button clicked
void PropertyPanel::onTextColorClicked()
{
    if (!m_shape) return;

    // Special handling for text shapes
    if (m_shape->getType() == DiagramShape::Text) {
        // Custom handling for text shape's text color here
    }

    emit shapeChanged();
}